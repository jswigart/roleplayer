#include <QtConcurrentMap>
#include <QDir>
#include <QScrollBar>
#include <QLabel>
#include <QCompleter>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QWindowsStyle.h>

#include "tiletools.h"
#include "gametilemap.h"
#include "roleplayer.h"
#include "flowlayout.h"

#include "dialog_importtiles.h"
#include "dialog_mapproperties.h"

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
	
	// cache some useful icons
	icons.critical = QApplication::style()->standardIcon( QStyle::SP_MessageBoxCritical );
	icons.folderOpen = QApplication::style()->standardIcon( QStyle::SP_DirOpenIcon );
	icons.folderClosed = QApplication::style()->standardIcon( QStyle::SP_DirClosedIcon );

	ui.mainToolBar->addAction( QApplication::style()->standardIcon( QStyle::SP_FileDialogNewFolder ), "New", 
		this, SLOT(Action_NewEditTab())  );
	ui.mainToolBar->addAction( QApplication::style()->standardIcon( QStyle::SP_DialogSaveButton ), "Save", 
		this, SLOT(Action_Save())  );
	
	layers.model = new QStandardItemModel( ui.tileScrollAreaContents );
	ui.treeViewLayers->setModel( layers.model );
	layers.model->setColumnCount( 2 );
	layers.model->setHorizontalHeaderItem( 0, new QStandardItem( "Layer" ) );
	layers.model->setHorizontalHeaderItem( 1, new QStandardItem( "Num Tiles" ) );

	// Create some default layers
	layers.rootItems.insert( "Base", new QStandardItem( icons.folderClosed, "Base" ) );
	
	(*layers.rootItems.find( "Base" ))->appendRow( new QStandardItem( "Test" ) );

	for ( ItemMap::iterator it = layers.rootItems.begin(); it != layers.rootItems.end(); ++it ) {
		layers.model->appendRow( *it );
	}

	connect( ui.action_MapProperties, SIGNAL(triggered(bool)), this, SLOT(Action_MapProperties()) );
	connect( ui.action_New, SIGNAL(triggered(bool)), this, SLOT(Action_NewEditTab()) );
	connect( ui.action_Exit, SIGNAL(triggered(bool)), this, SLOT(close()) );
	connect( ui.action_ImportTiles, SIGNAL(triggered(bool)), this, SLOT(Action_ImportTiles()) );
	connect( ui.action_Save, SIGNAL(triggered(bool)), this, SLOT(Action_Save()) );
	connect( ui.action_SaveMapImage, SIGNAL(triggered(bool)), this, SLOT(Action_SaveMapImage()) );
	connect( ui.action_SaveAll, SIGNAL(triggered(bool)), this, SLOT(Action_SaveAll()) );
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

	// always work from the executable
	QDir::setCurrent( QApplication::applicationDirPath() );
	fileWatcher.addPath( "./resources/images/tiles/" );
	fileRefresh.setSingleShot( true );
	
	Slot_PopulateTileList();

	AddMapEditTab( "untitled1" );
}

RolePlayer::~RolePlayer() {
}

void RolePlayer::AddMapEditTab( const QString & name ) {
	// handle name collisions
	int suffix = 1;
	QString tabName;
	while( true ) {
		QString testTabName = "untitled" + QString::number( suffix );
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
		++suffix;
	}	
	
	QDeclarativeView * editView = new QDeclarativeView( ui.editorTabs );
	editView->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	editView->setSource( QUrl( "./resources/maps/default.qml" ) );
	//editView->setResizeMode()
	editView->setBackgroundBrush( QBrush( Qt::gray ) );
	editView->setDragMode( QGraphicsView::NoDrag );
	editView->setObjectName( "view" );
	editView->setResizeAnchor( QGraphicsView::AnchorViewCenter );
	ui.editorTabs->addTab( editView, tabName );
}

void RolePlayer::Slot_TabChanged( int index ) {
	QDeclarativeView * currentView = qobject_cast<QDeclarativeView *>( ui.editorTabs->widget( index ) );
	if ( currentView != NULL ) {
		
	}	
}

void RolePlayer::Slot_TabCloseRequested( int index ) {
	QDeclarativeView * currentView = qobject_cast<QDeclarativeView *>( ui.editorTabs->widget( index ) );
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
			Action_Save();
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
	QStandardItem * treeItem = layers.model->itemFromIndex( index );
	if ( treeItem != NULL ) {
		treeItem->setIcon( icons.folderOpen );
	}
}

void RolePlayer::Slot_TreeItemCollapse( const QModelIndex & index ) {
	QStandardItem * treeItem = layers.model->itemFromIndex( index );
	if ( treeItem != NULL ) {
		treeItem->setIcon( icons.folderClosed );
	}
}

void RolePlayer::Slot_PopulateTileList() {
	QList<QWidget *> widgets = ui.tileScrollAreaContents->findChildren<QWidget *>();
	foreach(QWidget * widget, widgets) {
		delete widget;
	}

	fileList.tiles.clear();

	QStringList imageExtensions;
	imageExtensions << "*.png";
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

			connect( label, SIGNAL(clicked(QLabelClickable*)), this, SLOT(Slot_TileLabelClicked(QLabelClickable*)));

			ui.tileScrollAreaContents->layout()->addWidget( label );
		}
	}

	ui.tileScrollAreaContents->layout()->invalidate();
}

void RolePlayer::Action_NewEditTab() {
	AddMapEditTab( "untitled" );
}

void RolePlayer::Action_Save() {
	const int currentTab = ui.editorTabs->currentIndex();
	QDeclarativeView * currentView = qobject_cast<QDeclarativeView *>( ui.editorTabs->widget( currentTab ) );
	if ( currentView != NULL ) {
		QGameTileMap * map = qobject_cast<QGameTileMap *>( currentView->rootObject() );
		if ( map != NULL ) {
			QUrl saveFileUrl = map->getSaveFileUrl();			
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
			map->save( saveFileUrl );
		}
	}
}

void RolePlayer::Action_SaveAll() {
	const int currentTab = ui.editorTabs->currentIndex();
	for ( int i = 0; i < ui.editorTabs->count(); ++i ) {
		ui.editorTabs->setCurrentIndex( i );
		Action_Save();
	}
	ui.editorTabs->setCurrentIndex( currentTab );
}

void RolePlayer::Action_SaveMapImage() {
	const int currentTab = ui.editorTabs->currentIndex();
	QDeclarativeView * currentView = qobject_cast<QDeclarativeView *>( ui.editorTabs->widget( currentTab ) );
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
	QDeclarativeView * currentView = qobject_cast<QDeclarativeView *>( ui.editorTabs->widget( currentTab ) );
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

void RolePlayer::Slot_TileLabelClicked( QLabelClickable * label ) {
	if ( selectedTile == label ) {

	} else {
		if ( !selectedTile.isNull() ) {
			selectedTile->setOverlay( QPixmap() );
		}
		selectedTile = label;

		QPixmap overlay( label->pixmap()->size() );
		overlay.fill( QColor( 255, 0, 0, 100 ) );
		selectedTile->setOverlay( overlay );
	}
}
