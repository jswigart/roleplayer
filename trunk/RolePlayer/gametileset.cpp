#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "texturepacker.h"
#include "gametileset.h"

QGameTile::QGameTile( QDeclarativeItem *parent ) :
	QDeclarativeItem( parent ) {
	setFlag( QGraphicsItem::ItemHasNoContents, true );
}

QGameTile::~QGameTile() {
}

//////////////////////////////////////////////////////////////////////////

QGameTileSet::QGameTileSet( const QList< QImage > & tiles, QDeclarativeItem * parent ) :
	QDeclarativeItem( parent ) {
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	//setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptedMouseButtons( Qt::LeftButton );
	
	//////////////////////////////////////////////////////////////////////////

	using namespace TEXTURE_PACKER;
	TexturePacker * packer = createTexturePacker();

	packer->setTextureCount( tiles.count() );

	int totalArea = 0;
	for ( int i = 0; i < tiles.count(); ++i ) {
		packer->addTexture( tiles[ i ].width(), tiles[ i ].height() );
		totalArea += ( tiles[ i ].width() * tiles[ i ].height() );
	}
	
	TexturePacker::options_t options;
	options.allowRotationToFit = false;
	options.minWidth = 1024;

	int packedWidth = 0, packedHeight = 0;
	packer->packTextures( packedWidth, packedHeight, options );

	// create a fresh image
	atlas = QImage( packedWidth, packedHeight, QImage::Format_RGB32 );

	QPainter painter;
	painter.begin( &atlas );
	painter.fillRect( atlas.rect(), QColor( 0, 0, 0, 0 ) );

	for ( int i = 0; i < tiles.count(); ++i ) {
		int x, y, w, h;
		packer->getTextureLocation( i, x, y, w, h );
		painter.drawImage( x, y, tiles[ i ] );

		// create a proxy tile object?
		QGameTile * tile = new QGameTile( this );
		tile->setPos( x, y );
		tile->setSize( QSizeF( w, h ) );
	}

	painter.end();

	releaseTexturePacker( packer );

	setSize( QSizeF( atlas.width(), atlas.height() ) );
}

QGameTileSet::~QGameTileSet() {
}

void QGameTileSet::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	painter->drawImage( 0, 0, atlas );
	if ( selectedChild != NULL ) {
		painter->drawRect( selectedChild->boundingRect() );
	}
	QDeclarativeItem::paint( painter, option, widget );
}

void QGameTileSet::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	selectedChild = childAt( event->pos().x(), event->pos().y() );
	QDeclarativeItem::mousePressEvent( event );
}
