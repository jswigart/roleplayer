#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>
#include <QGraphicsSceneMouseEvent>

#include "gamecharacter.h"
#include "gamescene.h"
#include "dialog_mapproperties.h"

QGameCharacter::QGameCharacter( QDeclarativeItem * parent ) :
	QGameObject( parent ),
	showMoveHelper( false ) {
	setAcceptedMouseButtons( Qt::LeftButton );
	setFlag( QGraphicsItem::ItemHasNoContents, false );
	setFlag( QGraphicsItem::ItemIsMovable, true );
	setFlag( QGraphicsItem::ItemIsSelectable, true );

	memset( &widgets, 0, sizeof( widgets ) );
}

QGameCharacter::~QGameCharacter() {
}

void QGameCharacter::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	if ( isSelected() ) {		
		painter->setPen( QPen( QColor( Qt::blue ), Qt::DashLine ) );
		painter->drawRect( boundingRect() );
	}

	showMoveHelper = true;
	if ( showMoveHelper ) {
		/*QGameScene * scene = qobject_cast<QGameScene *>( parentItem() );
		if ( scene != NULL ) {
			if ( widgets.moveItem == NULL ) {
				QPainterPath path;
				path.addRect();				
				widgets.moveItem = scene->addPath();
			}			
		}*/
	}

	QDeclarativeItem::paint( painter, option, widget );	
}

void QGameCharacter::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
	scene()->clearSelection();
	setSelected( true );
	QDeclarativeItem::mousePressEvent( event );
}

void QGameCharacter::mouseMoveEvent( QGraphicsSceneMouseEvent * event ) {
	QGameScene * scene = qobject_cast<QGameScene *>( parentItem() );
	if ( scene != NULL ) {
		scene->snapToGrid( this, event->scenePos() );
	}
}
