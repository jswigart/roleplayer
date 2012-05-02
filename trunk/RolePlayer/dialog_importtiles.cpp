#include <QFileInfo>
#include <QtConcurrentRun>
#include <QtConcurrentMap>
#include <QTime>

#include "dialog_importtiles.h"
#include "flowlayout.h"

//////////////////////////////////////////////////////////////////////////
// TODO
// create custom label for mouse manipulation of tiles
// allow mouse toggling of tiles on click events
// display selected, unselected, and dupe tiles with a graphic overlay
// add context menu to 'ignore tiles like' to ignore tiles with the same content
//////////////////////////////////////////////////////////////////////////

QImage Async_LoadImageFile( const QString & file ) {
	return QImage( file );
}

//////////////////////////////////////////////////////////////////////////

QDialogImportTiles::QDialogImportTiles( const QList< QImage > & existingTiles, const QString & tileSetFile, QWidget *parent, Qt::WindowFlags f ) :
	QDialog( parent, f ) {
	ui.setupUi( this );
	ui.scrollAreaWidgetContents->setLayout( new QFlowLayout( ui.scrollAreaWidgetContents ) );
	
	connect(ui.spinBoxTileWidth, SIGNAL(valueChanged(int)), this, SLOT(Slot_CreateTilesFromTileSet()));
	connect(ui.spinBoxTileHeight, SIGNAL(valueChanged(int)), this, SLOT(Slot_CreateTilesFromTileSet()));
	connect(ui.spinBoxTileSpacing, SIGNAL(valueChanged(int)), this, SLOT(Slot_CreateTilesFromTileSet()));

	connect( &async_LoadTileSet, SIGNAL(finished()), this, SLOT(Slot_CreateTilesFromTileSet()) );
	connect( &async_CreateTilesWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(Slot_TileCreatedAt(int)));
	connect( &async_CreateTilesWatcher, SIGNAL(finished()), this, SLOT(Slot_PopulateTileView()) );
	
	// load the tile set asynchronously
	async_LoadTileSet.setFuture( QtConcurrent::run( Async_LoadImageFile, tileSetFile ) );
}

QDialogImportTiles::~QDialogImportTiles() {
	async_LoadTileSet.cancel();
	async_CreateTiles.cancel();
	
	async_LoadTileSet.waitForFinished();
	async_CreateTiles.waitForFinished();
}

void QDialogImportTiles::Slot_CreateTilesFromTileSet() {
	async_LoadTileSet.cancel();
	async_CreateTiles.cancel();

	async_LoadTileSet.waitForFinished();
	async_CreateTiles.waitForFinished();

	const int tileWidth = ui.spinBoxTileWidth->value();
	const int tileHeight = ui.spinBoxTileHeight->value();

	QList< tileInfo_t > tiles;

	tileInfo_t tile;
	tile.set = async_LoadTileSet.result();

	for ( int y = 0; y < tile.set.height(); y += tileHeight ) {
		for ( int x = 0; x < tile.set.width(); x += tileWidth ) {
			tile.name.sprintf( "%d_%d", x / tileWidth, y / tileHeight );
			tile.coord = QPoint( x, y );
			tile.size = QPoint( tileWidth, tileHeight );
			tiles.append( tile );
		}
	}

	ui.tileImportProgress->setRange( 0, tiles.count() );
	ui.tileImportProgress->setValue( 0 );

	async_CreateTiles = QtConcurrent::mapped( tiles, &QDialogImportTiles::Async_CreateTile );
	async_CreateTilesWatcher.setFuture( async_CreateTiles );
}

QDialogImportTiles::tileInfo_t QDialogImportTiles::Async_CreateTile( const tileInfo_t & tileInfo ) {
	tileInfo_t tileOut = tileInfo;
	tileOut.tile = tileInfo.set.copy( tileInfo.coord.x(), tileInfo.coord.y(), tileInfo.size.x(), tileInfo.size.y() );
	return tileOut;
}

void QDialogImportTiles::Slot_TileCreatedAt( int index ) {
	qDebug() << "Slot_TileCreatedAt: " << index;
	ui.tileImportProgress->setValue( ui.tileImportProgress->value() + 1 );
}

void QDialogImportTiles::Slot_PopulateTileView() {
	qDebug() << "Slot_PopulateTileView";

	QList<QWidget *> widgets = ui.scrollAreaWidgetContents->findChildren<QWidget *>();
	foreach(QWidget * widget, widgets) {
		delete widget;
	}

	Q_ASSERT_X( async_CreateTiles.isFinished(), __FUNCTION__, "async_CreateTiles wasn't finished" );
	
	for ( int i = 0; i < async_CreateTiles.resultCount(); ++i ) {
		const tileInfo_t tileResult = async_CreateTiles.resultAt( i );

		bool dupeImage = false;

		if ( ui.checkBoxCheckForDupes->checkState() == Qt::Checked ) {
			for ( int e = 0; e < existingTiles.count(); ++e ) {
				if ( tileResult.tile == existingTiles[ e ] ) {
					dupeImage = true;
					break;
				}
			}
		}

		if ( dupeImage ) {
			continue;
		}

		QPixmap thumb = QPixmap::fromImage( tileResult.tile, Qt::AutoColor | Qt::NoOpaqueDetection );
		QLabel * label = new QLabel( ui.scrollAreaWidgetContents );
		label->setScaledContents( true );
		label->setPixmap( thumb );
		label->setAlignment( Qt::AlignCenter );
		label->setFixedSize( thumb.width(), thumb.height() );
		label->setToolTip( tileResult.name );

		ui.scrollAreaWidgetContents->layout()->addWidget( label );
	}
}

void QDialogImportTiles::getImportedTiles( QList< tileInfo_t > & tilesOut ) {
	for ( int i = 0; i < async_CreateTiles.resultCount(); ++i ) {
		const tileInfo_t tileResult = async_CreateTiles.resultAt( i );
		tilesOut.append( tileResult );
	}
}
