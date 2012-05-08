#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>
#include <QGraphicsSceneMouseEvent>

#include "gamecharacter.h"
#include "gametilemap.h"
#include "dialog_mapproperties.h"

QGameCharacter::QGameCharacter( QDeclarativeItem * parent ) :
	QDeclarativeItem( parent ) {
	setAcceptedMouseButtons( Qt::LeftButton );
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	setFlag( QGraphicsItem::ItemIsMovable, true );
	setFlag( QGraphicsItem::ItemIsSelectable, true );
}

QGameCharacter::~QGameCharacter() {
}

void QGameCharacter::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	/*QPen gridPen( QColor( "red" ) );
	painter->setPen( gridPen );
	painter->drawRect( boundingRect() );*/
	QDeclarativeItem::paint( painter, option, widget );	
}

void QGameCharacter::mousePressEvent( QGraphicsSceneMouseEvent *event ) {	
}

void QGameCharacter::mouseMoveEvent( QGraphicsSceneMouseEvent * event ) {
	QGameTileMap * map = qobject_cast<QGameTileMap *>( parentItem() );
	if ( map != NULL ) {
		map->snapToGrid( this, event->scenePos() );
	}
}
