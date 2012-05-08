#include <QPen>
#include <QFile>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>

#include "gametilemap.h"
#include "dialog_mapproperties.h"

QGameTileMap::QGameTileMap( QDeclarativeItem *parent ) :
	QDeclarativeItem( parent ),
	gridSize( 32.0f ), 
	gridRender( true ),
	gridColor( 127, 127, 127, 127 ) {
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	//setFlag( QGraphicsItem::ItemIsSelectable, true );
	setAcceptedMouseButtons( Qt::LeftButton );
}

QGameTileMap::~QGameTileMap() {
}

//QGameTileMap * QGameTileMap::getTileAtPosition( const QPointF & scenePos ) {
//	QList<QGraphicsItem *> objects = items( scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder );
//	for ( int i = 0; i < objects.count(); ++i ) {
//		QGameTileMap * tileMap = qobject_cast<QGameTileMap*>( objects[ i ] );
//		if ( tileMap != NULL ) {
//			return tileMap;
//		}
//	}
//	return NULL;
//}

void QGameTileMap::snapToGrid( QGraphicsItem * item, const QPointF & scenePos ) {
	const int cellSizeX = item->boundingRect().width() / getGridSize();
	const int cellSizeY = item->boundingRect().height() / getGridSize();

	const int mapSizeX = ( boundingRect().width() / getGridSize() ) - cellSizeX;
	const int mapSizeY = ( boundingRect().height() / getGridSize() ) - cellSizeY;

	int cellX = scenePos.x() / getGridSize();
	int cellY = scenePos.y() / getGridSize();

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
	item->setPos( QPointF( cellX, cellY ) * getGridSize() );
}

void QGameTileMap::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	if ( getDrawGrid() ) {
		//painter->setPen( Qt::DashLine );
		//painter->setWorldMatrixEnabled(true);
		QPen gridPen( gridColor );
		painter->setPen( gridPen );

		const QRectF rect = boundingRect();

		qreal left = int( rect.left() ) - (int(rect.left()) % getGridSize() );
		qreal top = int( rect.top() ) - (int(rect.top()) % getGridSize() );

		QVarLengthArray<QLineF, 100> linesX;
		for ( qreal x = left; x < rect.right(); x += getGridSize() )
			linesX.append( QLineF(x, rect.top(), x, rect.bottom()) );

		QVarLengthArray<QLineF, 100> linesY;
		for ( qreal y = top; y < rect.bottom(); y += getGridSize() )
			linesY.append( QLineF(rect.left(), y, rect.right(), y) );

		painter->drawLines( linesX.data(), linesX.size() );
		painter->drawLines( linesY.data(), linesY.size() );
	}
	QDeclarativeItem::paint( painter, option, widget );
}

void QGameTileMap::redrawView() {
	scene()->invalidate();
}

void QGameTileMap::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	QDeclarativeItem::mousePressEvent( event );
}
