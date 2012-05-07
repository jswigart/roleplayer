#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>
#include <QGraphicsSceneMouseEvent>

#include "gamecharacter.h"
#include "gametilemap.h"
#include "dialog_mapproperties.h"

QGameCharacter::QGameCharacter( QDeclarativeItem *parent ) :
	QDeclarativeItem( parent ) {
	setAcceptedMouseButtons( Qt::LeftButton );
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	setFlag( QGraphicsItem::ItemIsMovable, true );
	setFlag( QGraphicsItem::ItemIsSelectable, true );
}

QGameCharacter::~QGameCharacter() {
}

void QGameCharacter::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	QPen gridPen( QColor( "red" ) );
	painter->setPen( gridPen );
	painter->drawRect( boundingRect() );
	QDeclarativeItem::paint( painter, option, widget );	
}

void QGameCharacter::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	
}

void QGameCharacter::mouseMoveEvent( QGraphicsSceneMouseEvent * event ) {
	QGameTileMap * map = qobject_cast<QGameTileMap *>( parentItem() );
	if ( map != NULL ) {
		const int mapSizeX = ( map->boundingRect().width() / map->getGridSize() ) - 1;
		const int mapSizeY = ( map->boundingRect().height() / map->getGridSize() ) - 1;

		int cellX = event->scenePos().x() / map->getGridSize();
		int cellY = event->scenePos().y() / map->getGridSize();

		if ( cellX < 0 ) {
			cellX = 0;
		} else if ( cellX > mapSizeX ) {
			cellX = mapSizeX;
		}
		if ( cellY < 0 ) {
			cellY = 0;
		} else if ( cellY > mapSizeY ) {
			cellY = mapSizeY;
		}
		/*QPointF snappedPos( 
			floorf( event->scenePos().x() / map->getGridSize() ) * map->getGridSize(),
			floorf( event->scenePos().y() / map->getGridSize() ) * map->getGridSize() );
		
		const QRectF sceneRect = map->boundingRect();
		if ( snappedPos.x() < sceneRect.x() ) {
			snappedPos.setX( sceneRect.x() );
		}
		if ( snappedPos.y() < sceneRect.y() ) {
			snappedPos.setY( sceneRect.y() );
		}
		if ( snappedPos.x() > sceneRect.topRight().x() ) {
			snappedPos.setX( sceneRect.topRight().x() );
		}
		if ( snappedPos.y() > sceneRect.bottomRight().y() ) {
			snappedPos.setY( sceneRect.bottomRight().y() );
		}*/
		setPos( QPointF( cellX, cellY ) * map->getGridSize() );
	}
}