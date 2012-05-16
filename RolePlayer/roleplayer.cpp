#include <QtConcurrentMap>
#include <QDir>
#include <QScrollBar>
#include <QLabel>
#include <QCompleter>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QWindowsStyle.h>
#include <QDeclarativeEngine>
#include <QGraphicsWidget>
#include <QDeclarativeContext>

#include "tiletools.h"
#include "gamecharacter.h"
#include "gametileset.h"
#include "roleplayer.h"
#include "flowlayout.h"
#include "texturepacker.h"

#include "tool.h"

#include "widget_gameview.h"

#include "dialog_preferences.h"
#include "dialog_importtiles.h"
#include "dialog_mapproperties.h"

#include "tool.h"
#include "gamescene.h"

//////////////////////////////////////////////////////////////////////////
// TODO
// allow tiles database to be saved/loaded
// allow tiles to be assigned to arbitrary groups/layers
// add show/hide group/layers so that subsets of tiles can be displayed for faster use
// create a self sized tile map manager that manages the map in renderable chunks(atlas per chunk?)
// add tile paint/drag/flood functionality
// add hotkeyable 'palette' for favorite tiles
// build texture atlas from currently used tiles(find a lib)
// store tile source file? detect when file changes?
// add support for arbitrary collision(polygons)
// add support for marking collision cells, merge into larger collision primitives
//////////////////////////////////////////////////////////////////////////

// Helper functions
QImage RolePlayer::Async_LoadImages( const QFileInfo & file ) {
	return QImage( file.canonicalFilePath() );
}

RolePlayer::RolePlayer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags) {
	QApplication::setStyle(new QWindowsStyle);
	
	ui.setupUi(this);	
	
	importPaths.append( "./resources/plugins" );
	
	// always work from the executable path
	QDir::setCurrent( QApplication::applicationDirPath() );
	fileWatcher.addPath( "./resources/images/tiles/" );
	fileRefresh.setSingleShot( true );

	QStandardItemModel * layerModel = new QStandardItemModel( ui.treeViewLayers );
	layerModel->setColumnCount( 2 );
	layerModel->setHorizontalHeaderItem( 0, new QStandardItem( "Layer" ) );
	layerModel->setHorizontalHeaderItem( 1, new QStandardItem( "Visible" ) );
	layerModel->setHorizontalHeaderItem( 2, new QStandardItem( "Locked" ) );
	ui.treeViewLayers->setModel( layerModel );
	
	CacheIcons();
	SetupToolBars();
	ConnectSlots();
		
	Slot_PopulateTileSetList();

	InitObjectPallette();
	
	//statusBar()->showMessage( "Status", 10000 );

	// add a default edit window
	// todo: open last edited files from save info
	AddMapEditTab( "untitled1" );
}

RolePlayer::~RolePlayer() {
}

void RolePlayer::CacheIcons() {
	QStyle * style = QApplication::style();
	icons.critical = style->standardIcon( QStyle::SP_MessageBoxCritical );
	icons.folderOpen = style->standardIcon( QStyle::SP_DirOpenIcon );
	icons.folderClosed = style->standardIcon( QStyle::SP_DirClosedIcon );
	icons.folderNew = style->standardIcon( QStyle::SP_FileDialogNewFolder );
	icons.saveFile = style->standardIcon( QStyle::SP_DialogSaveButton );
}

void RolePlayer::SetupToolBars() {
	ui.mainToolBar->addAction( icons.folderNew, "New", this, SLOT(Action_NewFile()) );
	ui.mainToolBar->addAction( icons.saveFile, "Save", this, SLOT(Action_SaveFile()) );

	toolBarTools = new QToolBar( this );
	toolBarTools->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

	tools.select = new QToolSelector( this );
	tools.paintTile = new QToolPaintTile( this );
	tools.createPoly = new QToolCreatePolygon( this );

	tools.select->setupAction( toolBarTools, QKeySequence( "1" ) );
	tools.paintTile->setupAction( toolBarTools, QKeySequence( "2" ) );
	tools.createPoly->setupAction( toolBarTools, QKeySequence( "3" ) );
	
	addToolBar( Qt::LeftToolBarArea, toolBarTools );
}

void RolePlayer::ConnectSlots() {
	connect( ui.action_Preferences, SIGNAL(triggered(bool)), this, SLOT(Action_Preferences()) );
	connect( ui.action_MapProperties, SIGNAL(triggered(bool)), this, SLOT(Action_MapProperties()) );
	connect( ui.action_New, SIGNAL(triggered(bool)), this, SLOT(Action_NewFile()) );
	connect( ui.action_Open, SIGNAL(triggered(bool)), this, SLOT(Action_OpenFile()) );
	connect( ui.action_Exit, SIGNAL(triggered(bool)), this, SLOT(close()) );
	connect( ui.action_ImportTiles, SIGNAL(triggered(bool)), this, SLOT(Action_ImportTiles()) );
	connect( ui.action_Save, SIGNAL(triggered(bool)), this, SLOT(Action_SaveFile()) );
	connect( ui.action_SaveMapImage, SIGNAL(triggered(bool)), this, SLOT(Action_SaveFileMapImage()) );
	connect( ui.action_SaveAll, SIGNAL(triggered(bool)), this, SLOT(Action_SaveFileAll()) );
	connect( ui.action_WindowTiles, SIGNAL(toggled(bool)), ui.dockWidgetTiles, SLOT(setVisible(bool)) );
	connect( ui.action_WindowResources, SIGNAL(toggled(bool)), ui.dockWidgetAssets, SLOT(setVisible(bool)) );

	connect( ui.dockWidgetTiles, SIGNAL(visibilityChanged(bool)), ui.action_WindowTiles, SLOT(setChecked(bool)) );
	connect( ui.dockWidgetAssets, SIGNAL(visibilityChanged(bool)), ui.action_WindowResources, SLOT(setChecked(bool)) );	
	connect( &fileList.async_LoadTilesets, SIGNAL(resultReadyAt(int)), this, SLOT(Slot_TileSetLoadedAt(int)) );
	connect( &fileList.async_LoadTilesets, SIGNAL(finished()), this, SLOT(Slot_TileSetLoadFinished()) );
	connect( ui.treeViewLayers, SIGNAL(expanded(QModelIndex)),this,SLOT(Slot_TreeItemExpanded(QModelIndex)) );
	connect( ui.treeViewLayers, SIGNAL(collapsed(QModelIndex)),this,SLOT(Slot_TreeItemCollapse(QModelIndex)) );
	connect( &fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(Slot_DirectoryChanged(QString)) );
	connect( &fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(Slot_FileChanged(QString)) );
	//connect( &fileRefresh, SIGNAL(timeout()), this, SLOT(Slot_PopulateTileList()));
	connect( ui.editorTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_TabCloseRequested(int)));
	connect( ui.editorTabs, SIGNAL(currentChanged(int)), this, SLOT(Slot_TabChanged(int)));

	connect( toolBarTools, SIGNAL(actionTriggered(QAction*)), this, SLOT(Slot_ToolTriggered(QAction*)) );	
}

QDeclarativeComponent * RolePlayer::CacheQMLComponent( QDeclarativeEngine * engine, const QUrl & file ) {
	QDeclarativeComponent * component = new QDeclarativeComponent( engine, engine );
	component->setObjectName( file.path() );

	component->loadUrl( file );
	
	if ( component->errors().count() > 0 ) {
		qDebug() << component->errors();

		QMessageBox msgBox(this);
		msgBox.setText("Component Not Found.");
		msgBox.setInformativeText( component->errorString() );
		msgBox.setStandardButtons( QMessageBox::Ok );
		msgBox.exec();

		delete component;
		component = NULL;
	}
	return component;
}

void RolePlayer::InitObjectPallette() {
	QFileInfoList objectFiles;

	FindAllFileTypes( QDir( "./resources/ruleset/heroquest/objects/" ).path(), QStringList( "*.qml" ), objectFiles );

	const qreal spacing = 1;
	const qreal margin = 1;
	qreal x = margin;
	qreal y = margin;
	qreal biggestY = 0;
	
	for ( int i = 0; i < objectFiles.count(); ++i ) {
		QDeclarativeComponent * c = CacheQMLComponent( ui.viewObjectPalette->engine(), QUrl::fromLocalFile( objectFiles[ i ].canonicalFilePath() ) );
		if ( c != NULL ) {
			QDeclarativeItem * item = qobject_cast<QDeclarativeItem*>( c->create() );
			if ( item->property( "placeable" ).toBool() ) {
				ui.viewObjectPalette->scene()->addItem( item );

				item->setPos( x, y );

				x += item->width() + spacing;
				biggestY = ( item->height() > biggestY ) ? item->height() : biggestY;
				if ( x > ui.viewObjectPalette->rect().width() ) {
					x = margin;
					y += biggestY + spacing;
					biggestY = 0;
				}
			} else {
				delete item;
			}
		}
	};
}

int RolePlayer::AddMapEditTab( const QString & name ) {
	// handle name collisions
	int suffix = 1;
	QString tabName;
	while( true ) {
		QString testTabName = "untitled" + QString::number( suffix++ );
		bool nameConflict = false;
		for ( int i = 0; i < ui.editorTabs->count(); ++i ) {
			if ( ui.editorTabs->tabText( i ) == testTabName ) {
				nameConflict = true;
			}
		}
		if ( !nameConflict ) {
			tabName = testTabName;
			break;
		}
	}
	
	QGameView * editView = new QGameView( ui.editorTabs );	
	editView->setMouseTracking( true );
	editView->setScene( new QGameScene( this, ui.editorTabs ) );
	editView->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setResizeAnchor( QGraphicsView::AnchorViewCenter );
	
	QDeclarativeEngine * declEngine = new QDeclarativeEngine( editView );
	declEngine->setObjectName( "engine" );
	declEngine->setImportPathList( importPaths );

	// expose the context variables to QML
	QDeclarativeContext * rootContext = declEngine->rootContext();
	if ( rootContext != NULL ) {
		rootContext->setContextProperty( "scenario", new QGameScenario( editView->scene() ) );
		//rootContext->setContextObject
	}

	//////////////////////////////////////////////////////////////////////////
	
	QDeclarativeComponent * component = CacheQMLComponent( declEngine, QUrl::fromLocalFile("./resources/ruleset/heroquest/objects/BattleTest.qml") );
	if ( component != NULL ) {
		QObject * obj = component->create( rootContext ) ;
		QGraphicsItem * gfxItem = qobject_cast<QGraphicsItem*>( obj );
		if ( gfxItem != NULL ) {
			editView->scene()->addItem( gfxItem );
		} else if ( obj ) {
			delete obj;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	connect( editView->scene(), SIGNAL(selectionChanged()), this, SLOT(Slot_RefreshPropertyList()));

	return ui.editorTabs->addTab( editView, tabName );	
}

void RolePlayer::AddTileSetTab( const QFileInfo & file, const QImage & image, bool focus ) {
	QGraphicsView * view = new QGraphicsView( ui.editorTabs );
	view->setScene( new QGameScene( this, ui.editorTabs ) );
	view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	view->setObjectName( file.canonicalFilePath() );
	view->setResizeAnchor( QGraphicsView::AnchorViewCenter );
	view->setDragMode( QGraphicsView::ScrollHandDrag );
	
	/*QDeclarativeEngine * declEngine = new QDeclarativeEngine( view );
	declEngine->setImportPathList( importPaths );*/

	QGameTileSet * tileSet = new QGameTileSet( file, image );
	view->scene()->addItem( tileSet );	
	view->setBackgroundBrush( QBrush( Qt::gray ) );

	ui.tabWidgetTileSets->addTab( view, file.baseName() );

	connect( tileSet, SIGNAL(TileSelected(QGameTile*)),
		tools.paintTile, SLOT(Slot_TileSelected(QGameTile*)));
}

void RolePlayer::AppendToLog( const QString & msg ) {
	ui.textBrowserLog->append( msg );
}

void RolePlayer::Slot_ToolTriggered( QAction * action ) {
	const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QTool * selectedTool = action->data().value<QTool*>();
		if ( selectedTool != NULL ) {
			currentView->getGameScene()->setCurrentTool( selectedTool );
		}
	}
}

void RolePlayer::Slot_TabChanged( int index ) {
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( index ) );
	if ( currentView != NULL ) {
		QGameScene * scene = currentView->getGameScene();
		scene->setCurrentTool( NULL );

		// repopulate the layer list
		
		QStandardItemModel * layerModel = qobject_cast<QStandardItemModel*>( ui.treeViewLayers->model() );
		layerModel->removeRows( 0, layerModel->rowCount() );

		QStringList layers;
		scene->getLayerNames( layers );

		for ( int i = 0; i < layers.count(); ++i ) {
			QGameLayer * layer = scene->getLayer( layers[ i ] );
			
			QStandardItem * visible = new QStandardItem();
			visible->setCheckable( true );
			visible->setCheckState( !layer->getHidden() ? Qt::Checked : Qt::Unchecked );
			
			QStandardItem * locked = new QStandardItem();
			locked->setCheckable( true );
			locked->setCheckState( layer->getLocked() ? Qt::Checked : Qt::Unchecked );

			const int row = layerModel->rowCount();
			layerModel->setItem( row, 0, new QStandardItem( layers[ i ] ) );
			layerModel->setItem( row, 1, visible );
			layerModel->setItem( row, 2, locked );
		}
	}
	Slot_RefreshPropertyList();
}

void RolePlayer::Slot_TabCloseRequested( int index ) {
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( index ) );
	if ( currentView != NULL ) {
		QPixmap mapThumbnail( 200, 200 );
		
		QPainter painter;
		painter.begin( &mapThumbnail );
		painter.fillRect( mapThumbnail.rect(), currentView->backgroundBrush() );
		currentView->scene()->render( &painter );
		painter.end();
		
		const QSizeF sceneSize = currentView->scene()->sceneRect().size();

		QString detailText;
		QTextStream detailStream( &detailText );
		detailStream << "File: " << "filename.map" << "\n";
		detailStream << "Size: " << sceneSize.width() << " x " << sceneSize.height() << " pixels\n";
		
		QMessageBox msgBox;
		msgBox.setIconPixmap( mapThumbnail );
		msgBox.setText( "The document has been modified." );
		msgBox.setDetailedText( detailText );
		msgBox.setInformativeText( "Do you want to save your changes?" );
		msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
		msgBox.setDefaultButton( QMessageBox::Save );
		switch( msgBox.exec() ) {
		case QMessageBox::Save:
			ui.editorTabs->setCurrentIndex( index );
			Action_SaveFile();
			break;
		case QMessageBox::Discard:
			// just remove the tab
			break;
		case QMessageBox::Cancel:
			// don't remove the tab
			return;
		}
	}
	ui.editorTabs->removeTab( index );

	Slot_RefreshPropertyList();
}

void RolePlayer::Slot_RefreshPropertyList() {
	ui.propTree->clear();

	const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QList<QGraphicsItem *> selected = currentView->getGameScene()->selectedItems();

		for( int i = 0; i < selected.count(); ++i ) {
			/*QGameCharacter * gfxObj = qgraphicsitem_cast<QGameCharacter*>( selected[ i ] );
			if ( gfxObj != NULL ) {
				ui.propTree->AddGroup( NULL, 
					gfxObj->objectName(), 
					gfxObj->objectName() );	
			}*/
		}
	}
}

void RolePlayer::Slot_DirectoryChanged( const QString & path ) {
	// delay the refresh to avoid repeated updates in cases
	// where the file watcher spams us with updates
	fileRefresh.stop();	
	fileRefresh.start( 1000 );
}

void RolePlayer::Slot_FileChanged( const QString & path ) {
	// delay the refresh to avoid repeated updates in cases
	// where the file watcher spams us with updates
	fileRefresh.stop();
	fileRefresh.start( 1000 );
}

void RolePlayer::Slot_TreeItemExpanded( const QModelIndex & index ) {
	/*QStandardItem * treeItem = layers.model->itemFromIndex( index );
	if ( treeItem != NULL ) {
		treeItem->setIcon( icons.folderOpen );
	}*/
}

void RolePlayer::Slot_TreeItemCollapse( const QModelIndex & index ) {
}

void RolePlayer::Slot_PopulateTileSetList() {
	fileList.tilesets.clear();

	QStringList imageExtensions;
	imageExtensions << "*.png" << "*.jpg" << "*.jpeg" << "*.tga";
	FindAllFileTypes( QDir( "./resources/images/tileset" ).path(), imageExtensions, fileList.tilesets );

	if ( !fileList.tilesets.isEmpty() ) {
		fileList.async_LoadTilesets.setFuture( QtConcurrent::mapped( fileList.tilesets, &RolePlayer::Async_LoadImages ) );
	}
}

void RolePlayer::FindAllFileTypes( const QString & path, const QStringList & fileTypes, QFileInfoList & files ) {
	QDir dir( path );
	if ( dir.exists() ) {
		QFileInfoList f = dir.entryInfoList( fileTypes, QDir::Files, QDir::Name );
		files.append( f );

		// recurse into sub folders
		QFileInfoList subdirs = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name );
		for ( int i = 0; i < subdirs.count(); ++i ) {
			FindAllFileTypes( subdirs.at( i ).filePath(), fileTypes, files );
		}
	}
}

void RolePlayer::Slot_TileSetLoadedAt( int index ) {
	qDebug() << "Slot_TileSetLoadedAt:( " << index << " / " << fileList.tilesets.count() <<  " ) : " << fileList.tilesets.at( index ).canonicalFilePath();

	AddTileSetTab( fileList.tilesets.at( index ), fileList.async_LoadTilesets.resultAt( index ) );	
}

void RolePlayer::Slot_TileSetLoadFinished() {
	qDebug() << "Slot_TileSetLoadFinished";	
}

void RolePlayer::Action_NewFile() {
	AddMapEditTab( "untitled" );
}

void RolePlayer::Action_OpenFile() {
	QString openFile = QFileDialog::getOpenFileName( this, "Open File", "./resources/maps/", tr("Map Files (*.map *.tmx)") );
	if ( !openFile.isEmpty() ) {
		QFileInfo file( openFile );
		
		const int tab = AddMapEditTab( file.baseName() );
		QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( tab ) );
		if ( currentView != NULL ) {
			currentView->getGameScene()->load( QUrl( openFile ) );
		}
	}
}

void RolePlayer::Action_SaveFile() {
	const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QGameScene * scene = currentView->getGameScene();
		if ( scene != NULL ) {
			QUrl saveFileUrl = scene->getSaveFileUrl();			
			if ( saveFileUrl.isEmpty() ) {
				const QString defaultSavePath = QString( "./resources/maps/%1.map" )
					.arg( ui.editorTabs->tabText( currentTab ) );

				saveFileUrl = QFileDialog::getSaveFileName( this, 
					tr("Save Map"), defaultSavePath, tr("Map Files (*.map)") );

				if ( saveFileUrl.isEmpty() ) {
					return;
				}

				QFileInfo saveFile( saveFileUrl.path() );
				ui.editorTabs->setTabText( currentTab, saveFile.baseName() );
			}
			scene->save( saveFileUrl );
		}
	}
}

void RolePlayer::Action_SaveFileAll() {
	const int currentTab = ui.editorTabs->currentIndex();
	for ( int i = 0; i < ui.editorTabs->count(); ++i ) {
		ui.editorTabs->setCurrentIndex( i );
		Action_SaveFile();
	}
	ui.editorTabs->setCurrentIndex( currentTab );
}

void RolePlayer::Action_SaveFileMapImage() {
	const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		const QSizeF sceneSize = currentView->scene()->sceneRect().size();

		QPixmap mapRender( sceneSize.toSize() );

		QPainter painter;
		painter.begin( &mapRender );
		currentView->scene()->render( &painter );
		painter.end();

		QString detailText;
		QTextStream detailStream( &detailText );
		detailStream << "File: " << "filename.map" << "\n";
		detailStream << "Size: " << sceneSize.width() << " x " << sceneSize.height() << " pixels\n";

		QMessageBox msgBox;
		msgBox.setIconPixmap( mapRender.scaled( 200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
		msgBox.setText( "The document has been modified." );
		msgBox.setDetailedText( detailText );
		msgBox.setInformativeText( "Do you want to save your changes?" );
		msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Cancel );
		msgBox.setDefaultButton( QMessageBox::Save );
		switch( msgBox.exec() ) {
		case QMessageBox::Save:				
			{
				QString saveFileName = QFileDialog::getSaveFileName( this, 
					tr("Save File"), "./resources/maps/image.jpg", tr("Images (*.png *.jpg)") );
				mapRender.save( saveFileName );
			}
			break;
		case QMessageBox::Cancel:
			// don't remove the tab
			return;
		}
	}
}

void RolePlayer::Action_MapProperties() {
	/*const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QGameTileMap * mapItem = currentView->findChild<QGameTileMap*>( "map" );
		if ( mapItem != NULL ) {
			QDialogMapProperties dlg( mapItem, this );
			dlg.exec();
		}		
	}*/
}

void RolePlayer::Action_Preferences() {
	QDialogPreferences dlg( this );
	dlg.exec();
}

void RolePlayer::Action_ImportTiles() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./", tr("Image Files (*.png *.jpg *.bmp *.tga)"));
	if ( !fileName.isEmpty() ) {
		QDialogImportTiles dlg( masterTileList, fileName, this );
		if ( dlg.exec() == QDialog::Accepted ) {
			QList< QDialogImportTiles::tileInfo_t > newTiles;
			dlg.getImportedTiles( newTiles );
			
			
		}
	}
}
