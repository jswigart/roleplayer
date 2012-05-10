#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "texturepacker.h"
#include "gametileset.h"

QGameTile::QGameTile( QDeclarativeItem *parent ) :
	QDeclarativeItem( parent ) {
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptedMouseButtons( Qt::LeftButton );
}

QGameTile::~QGameTile() {
}

QPixmap QGameTile::GetPixMap() {
	QGameTileSet * tileSet = qobject_cast<QGameTileSet*>( parentItem() );
	if ( tileSet != NULL ) {
		cachedImage = tileSet->GetImage().copy( pos().x(), pos().y(), width(), height() );
	}
	return QPixmap::fromImage( cachedImage );
}

void QGameTile::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	QGameTileSet * tileSet = qobject_cast<QGameTileSet*>( parentItem() );
	if ( tileSet != NULL ) {
		painter->drawImage( 0, 0, tileSet->GetImage(), pos().x(), pos().y(), width(), height() );
	}
	if ( isSelected() ) {		
		painter->setPen( QPen( QColor( "blue" ), Qt::DashLine ) );
		painter->drawRect( boundingRect() );
	}
	QDeclarativeItem::paint( painter, option, widget );	
}

void QGameTile::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	scene()->clearSelection();
	setSelected( true );

	emit TileSelected( this );
	QDeclarativeItem::mousePressEvent( event );
}

//////////////////////////////////////////////////////////////////////////

QGameTileSet::QGameTileSet( const QFileInfo & file, const QImage & image) :
	QDeclarativeItem( NULL ) {
	tileSetFile = file;
	tileSetImage = image;

	setFlag( QGraphicsItem::ItemHasNoContents, true );
	setFlag( QGraphicsItem::ItemIsSelectable, false );
	setAcceptedMouseButtons( Qt::LeftButton );

	setSize( QSizeF( tileSetImage.width(), tileSetImage.height() ) );

	// todo: load from metadata
	const int tileHeight = 32;
	const int tileWidth = 32;

	for ( int y = 0; y < tileSetImage.height(); y += tileHeight ) {
		for ( int x = 0; x < tileSetImage.width(); x += tileWidth ) {
			QString name = QString( "%1_%2" )
				.arg( x / tileWidth )
				.arg( y / tileHeight );

			// create a proxy tile object?
			QGameTile * tile = new QGameTile( this );
			tile->setObjectName( name );
			tile->setPos( x, y );
			tile->setSize( QSizeF( tileWidth, tileHeight ) );

			connect( tile, SIGNAL(TileSelected(QGameTile*)), this, SLOT(Slot_TileSelected(QGameTile*)));
		}
	}
}
	
QGameTileSet::~QGameTileSet() {
}

void QGameTileSet::Slot_TileSelected( QGameTile * tile ) {
	emit TileSelected( tile );
}

void QGameTileSet::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	//painter->drawImage( 0, 0, tileSetImage );	
	QDeclarativeItem::paint( painter, option, widget );	
}

//void QGameTileSet::RefreshImages( const QList< QImage > & tiles ) {
//	QList<QGameTile *> existingTiles = findChildren<QGameTile *>();
//	foreach(QGameTile * tile, existingTiles) {
//		delete tile;
//	}
//	
//	using namespace TEXTURE_PACKER;
//	TexturePacker * packer = createTexturePacker();
//
//	packer->setTextureCount( tiles.count() );
//
//	int totalArea = 0;
//	for ( int i = 0; i < tiles.count(); ++i ) {
//		packer->addTexture( tiles[ i ].width(), tiles[ i ].height() );
//		totalArea += ( tiles[ i ].width() * tiles[ i ].height() );
//	}
//
//	TexturePacker::options_t options;
//	options.onePixelBorder = false;
//	options.allowRotationToFit = false;
//	options.minWidth = 1024;
//
//	int packedWidth = 0, packedHeight = 0;
//	packer->packTextures( packedWidth, packedHeight, options );
//
//	// create a fresh image
//	atlas = QImage( packedWidth, packedHeight, QImage::Format_RGB32 );
//
//	QPainter painter;
//	painter.begin( &atlas );
//	painter.fillRect( atlas.rect(), QColor( 0, 0, 0, 0 ) );
//
//	for ( int i = 0; i < tiles.count(); ++i ) {
//		int x, y, w, h;
//		packer->getTextureLocation( i, x, y, w, h );
//		painter.drawImage( x, y, tiles[ i ] );
//
//		// create a proxy tile object?
//		QGameTile * tile = new QGameTile( this );
//		tile->setPos( x, y );
//		tile->setSize( QSizeF( w, h ) );
//	}
//
//	painter.end();
//
//	releaseTexturePacker( packer );
//
//	setSize( QSizeF( atlas.width(), atlas.height() ) );
//}
