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

#include "tiletools.h"
#include "gametilemap.h"
#include "roleplayer.h"
#include "flowlayout.h"

#include "tool.h"

#include "widget_gameview.h"
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
	ui.tileScrollAreaContents->setLayout( new QFlowLayout() );
	
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
		
	Slot_PopulateTileList();

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

	QToolPaintTile * toolPaint = new QToolPaintTile( this );
	QToolCreatePolygon * toolPolygon = new QToolCreatePolygon( this );

	toolPaint->setupAction( toolBarTools );
	toolPolygon->setupAction( toolBarTools );
	
	// the tile tool must be notified when the tile selection changes
	connect( this, SIGNAL(TileSelected(QLabelClickable*)), toolPaint, SLOT(Slot_TileSelected(QLabelClickable*)));

	addToolBar( Qt::TopToolBarArea, toolBarTools );
}

void RolePlayer::ConnectSlots() {
	connect( ui.action_MapProperties, SIGNAL(triggered(bool)), this, SLOT(Action_MapProperties()) );
	connect( ui.action_New, SIGNAL(triggered(bool)), this, SLOT(Action_NewFile()) );
	connect( ui.action_Exit, SIGNAL(triggered(bool)), this, SLOT(close()) );
	connect( ui.action_ImportTiles, SIGNAL(triggered(bool)), this, SLOT(Action_ImportTiles()) );
	connect( ui.action_Save, SIGNAL(triggered(bool)), this, SLOT(Action_SaveFile()) );
	connect( ui.action_SaveMapImage, SIGNAL(triggered(bool)), this, SLOT(Action_SaveMapImage()) );
	connect( ui.action_SaveAll, SIGNAL(triggered(bool)), this, SLOT(Action_SaveFileAll()) );
	connect( ui.action_WindowTiles, SIGNAL(toggled(bool)), ui.dockWidgetTiles, SLOT(setVisible(bool)) );
	connect( ui.action_WindowResources, SIGNAL(toggled(bool)), ui.dockWidgetAssets, SLOT(setVisible(bool)) );

	connect( ui.dockWidgetTiles, SIGNAL(visibilityChanged(bool)), ui.action_WindowTiles, SLOT(setChecked(bool)) );
	connect( ui.dockWidgetAssets, SIGNAL(visibilityChanged(bool)), ui.action_WindowResources, SLOT(setChecked(bool)) );	
	connect( &fileList.async_LoadTiles, SIGNAL(resultReadyAt(int)), this, SLOT(Slot_ImageLoadedAt(int)) );
	connect( &fileList.async_LoadTiles, SIGNAL(finished()), this, SLOT(Slot_ImageLoadFinished()) );
	connect( ui.treeViewLayers, SIGNAL(expanded(QModelIndex)),this,SLOT(Slot_TreeItemExpanded(QModelIndex)) );
	connect( ui.treeViewLayers, SIGNAL(collapsed(QModelIndex)),this,SLOT(Slot_TreeItemCollapse(QModelIndex)) );
	connect( &fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(Slot_DirectoryChanged(QString)) );
	connect( &fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(Slot_FileChanged(QString)) );
	connect( &fileRefresh, SIGNAL(timeout()), this, SLOT(Slot_PopulateTileList()));
	connect( ui.editorTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_TabCloseRequested(int)));
	connect( ui.editorTabs, SIGNAL(currentChanged(int)), this, SLOT(Slot_TabChanged(int)));

	connect( toolBarTools, SIGNAL(actionTriggered(QAction*)), this, SLOT(Slot_ToolTriggered(QAction*)) );
}

void RolePlayer::AddMapEditTab( const QString & name ) {
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

	QStringList importPaths;
	importPaths.append( "./resources/plugins" );

	QGameView * editView = new QGameView( ui.editorTabs );
	editView->setMouseTracking( true );
	editView->setScene( new QGameScene( this, ui.editorTabs ) );
	editView->engine()->setImportPathList( importPaths );
	editView->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setSource( QUrl( "./resources/maps/default.qml" ) );
	//editView->setResizeMode()
	editView->setBackgroundBrush( QBrush( Qt::gray ) );
	//editView->setDragMode( QGraphicsView::ScrollHandDrag );
	editView->setObjectName( "view" );
	editView->setResizeAnchor( QGraphicsView::AnchorViewCenter );
	ui.editorTabs->addTab( editView, tabName );
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
	/*QStandardItem * treeItem = layers.model->itemFromIndex( index );
	if ( treeItem != NULL ) {
		treeItem->setIcon( icons.folderClosed );
	}*/
}

void RolePlayer::Slot_PopulateTileList() {
	QList<QWidget *> widgets = ui.tileScrollAreaContents->findChildren<QWidget *>();
	foreach(QWidget * widget, widgets) {
		delete widget;
	}

	fileList.tiles.clear();

	QStringList imageExtensions;
	imageExtensions << "*.png" << "*.jpg" << "*.jpeg" << "*.tga";
	FindAllFileTypes( QDir( "./resources/images/tiles" ).path(), imageExtensions, fileList.tiles );

	if ( !fileList.tiles.isEmpty() ) {
		fileList.async_LoadTiles.setFuture( QtConcurrent::mapped( fileList.tiles, &RolePlayer::Async_LoadImages ) );
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

void RolePlayer::Slot_ImageLoadedAt( int index ) {
	qDebug() << "async_ImageLoadedAt:( " << index << " / " << fileList.tiles.count() <<  " ) : " << fileList.tiles.at( index ).canonicalFilePath();
}

void RolePlayer::Slot_ImageLoadFinished() {
	qDebug() << "async_ImageLoadFinished";
	RebuildTileThumbnails();
}

void RolePlayer::RebuildTileThumbnails() {
	masterTileList.clear();

	for ( int i = 0; i < fileList.async_LoadTiles.future().resultCount(); ++i ) {
		QImage image = fileList.async_LoadTiles.resultAt( i );
		if ( !image.isNull() ) {
			// ignore dupes
			int dupeIndex = -1;
			for ( int m = 0; m < masterTileList.count(); ++m ) {
				if ( masterTileList[ m ] == image ) {
					dupeIndex = m;
					break;
				}
			}
			
			if ( dupeIndex != -1 ) {
				QString error = QString( "Dupe Image <a href=\"%1\">%2</a> same as <a href=\"%3\">%4</a>, ignoring" )
					.arg( fileList.tiles.at( i ).dir().canonicalPath() )
					.arg( fileList.tiles.at( i ).canonicalFilePath() )
					.arg( fileList.tiles.at( dupeIndex ).dir().canonicalPath() )
					.arg( fileList.tiles.at( dupeIndex ).canonicalFilePath() );
				ui.textBrowserLog->append( error );

				ui.tabWidgetResources->setTabIcon( 0, icons.critical );
				continue;
			}

			masterTileList.append( image );

			QLabelClickable * label = new QLabelClickable( this );
			QPixmap thumb = QPixmap::fromImage( image, Qt::AutoColor | Qt::NoOpaqueDetection );
			label->setScaledContents( true );
			label->setPixmap( thumb );
			label->setAlignment( Qt::AlignCenter );
			label->setFixedSize( thumb.width(), thumb.height() );
			label->setToolTip( fileList.tiles.at( i ).baseName() );

			connect( label, SIGNAL(clicked(QLabelClickable*)), this, SLOT(Slot_TileSelected(QLabelClickable*)));

			ui.tileScrollAreaContents->layout()->addWidget( label );
		}
	}

	ui.tileScrollAreaContents->layout()->invalidate();
}

void RolePlayer::Action_NewFile() {
	AddMapEditTab( "untitled" );
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
	const int currentTab = ui.editorTabs->currentIndex();
	QGameView * currentView = qobject_cast<QGameView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QGameTileMap * mapItem = currentView->findChild<QGameTileMap*>( "map" );
		if ( mapItem != NULL ) {
			QDialogMapProperties dlg( mapItem, this );
			dlg.exec();
		}		
	}
}

void RolePlayer::Action_ImportTiles() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "./", tr("Image Files (*.png *.jpg *.bmp *.tga)"));
	if ( !fileName.isEmpty() ) {
		QDialogImportTiles dlg( masterTileList, fileName, this );
		if ( dlg.exec() == QDialog::Accepted ) {
			QList< QDialogImportTiles::tileInfo_t > newTiles;
			dlg.getImportedTiles( newTiles );
			
			// save the imported tiles out as individual tiles
			for ( int i = 0; i < newTiles.count(); ++i ) {
				QString tileFile = "./resources/images/tiles/" + QFileInfo( fileName ).completeBaseName() + "_" + newTiles[ i ].name + ".png";
				newTiles[ i ].tile.save( tileFile );
			}

			Slot_PopulateTileList();
		}
	}
}

void RolePlayer::Slot_TileSelected( QLabelClickable * label ) {
	emit TileSelected( label );
}
