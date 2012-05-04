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
	uiTileGroup.setupUi( ui.tileTools );
	
	uiTileGroup.tileScrollAreaContents->setLayout( new QFlowLayout() );
	
	// cache some useful icons
	icons.critical = QApplication::style()->standardIcon( QStyle::SP_MessageBoxCritical );
	icons.folderOpen = QApplication::style()->standardIcon( QStyle::SP_DirOpenIcon );
	icons.folderClosed = QApplication::style()->standardIcon( QStyle::SP_DirClosedIcon );

	layers.model = new QStandardItemModel( uiTileGroup.tileScrollAreaContents );
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

	connect( ui.actionMapProperties, SIGNAL(triggered(bool)), this, SLOT(Action_MapProperties()) );
	connect( ui.actionNew, SIGNAL(triggered(bool)), this, SLOT(Action_NewEditTab()) );
	connect( ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(close()) );
	connect( ui.actionImport_Tiles, SIGNAL(triggered(bool)), this, SLOT(Action_ImportTiles()) );
	connect( ui.actionSave, SIGNAL(triggered(bool)), this, SLOT(Action_Save()) );
	connect( ui.actionSave_All, SIGNAL(triggered(bool)), this, SLOT(Action_SaveAll()) );	

	connect( ui.actionWindowTiles, SIGNAL(toggled(bool)), ui.dockWidgetTiles, SLOT(setVisible(bool)) );
	connect( ui.actionWindowResources, SIGNAL(toggled(bool)), ui.dockWidgetAssets, SLOT(setVisible(bool)) );
	connect( ui.dockWidgetTiles, SIGNAL(visibilityChanged(bool)), ui.actionWindowTiles, SLOT(setChecked(bool)) );
	connect( ui.dockWidgetAssets, SIGNAL(visibilityChanged(bool)), ui.actionWindowResources, SLOT(setChecked(bool)) );	
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

	QGraphicsTileView * editView = new QGraphicsTileView( ui.editorTabs );
	editView->setBackgroundBrush( QBrush( Qt::gray ) );
	editView->setDragMode( QGraphicsView::ScrollHandDrag );
	editView->setObjectName( "view" );
	editView->setDrawGrid( true );
	editView->setGridSize( 32 );
	editView->setScene( new QGraphicsScene );
	editView->setResizeAnchor( QGraphicsView::AnchorViewCenter );
	
	editView->scene()->addEllipse( rand() % 20000, rand() % 20000, 50 + rand() % 100, 50 + rand() % 100, QPen(), QBrush( QColor( "yellow" ) ) );
	editView->scene()->addEllipse( rand() % 20000, rand() % 20000, 50 + rand() % 100, 50 + rand() % 100, QPen(), QBrush( QColor( "yellow" ) ) );

	ui.editorTabs->addTab( editView, tabName );
}

void RolePlayer::Slot_TabChanged( int index ) {
	QGraphicsTileView * currentView = qobject_cast<QGraphicsTileView *>( ui.editorTabs->widget( index ) );
	if ( currentView != NULL ) {
		
	}	
}

void RolePlayer::Slot_TabCloseRequested( int index ) {
	QGraphicsTileView * currentView = qobject_cast<QGraphicsTileView *>( ui.editorTabs->widget( index ) );
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
			currentView->save();
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
	QList<QWidget *> widgets = uiTileGroup.tileScrollAreaContents->findChildren<QWidget *>();
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

			QLabel * label = new QLabel();

			QPixmap thumb = QPixmap::fromImage( image, Qt::AutoColor | Qt::NoOpaqueDetection );
			label->setScaledContents( true );
			label->setPixmap( thumb );
			label->setAlignment( Qt::AlignCenter );
			label->setFixedSize( thumb.width(), thumb.height() );
			label->setToolTip( fileList.tiles.at( i ).baseName() );

			uiTileGroup.tileScrollAreaContents->layout()->addWidget( label );
		}
	}

	uiTileGroup.tileScrollAreaContents->layout()->invalidate();
}

void RolePlayer::Action_NewEditTab() {
	AddMapEditTab( "untitled" );
}

void RolePlayer::Action_Save() {
	QGraphicsTileView * currentView = qobject_cast<QGraphicsTileView *>( ui.editorTabs->widget( ui.editorTabs->currentIndex() ) );
	if ( currentView != NULL ) {
		currentView->save();
	}
}

void RolePlayer::Action_SaveAll() {
	for ( int i = 0; i < ui.editorTabs->count(); ++i ) {
		QGraphicsTileView * currentView = qobject_cast<QGraphicsTileView *>( ui.editorTabs->widget( i ) );
		if ( currentView != NULL ) {
			currentView->save();
		}
	}
}

void RolePlayer::Action_MapProperties() {
	QGraphicsTileView * currentView = qobject_cast<QGraphicsTileView *>( ui.editorTabs->widget( ui.editorTabs->currentIndex() ) );
	if ( currentView != NULL ) {
		currentView->changeMapPropertyDialog();
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
