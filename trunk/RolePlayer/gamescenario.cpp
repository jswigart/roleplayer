#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "gamescenario.h"

QGameRangeIndicator::QGameRangeIndicator( QGraphicsItem * parent ) :
	QGraphicsObject( parent ),
	range( 0 ),
	rangeStep( 0 ) {
	connect( this, SIGNAL(rangeChanged()), this, SLOT(Slot_UpdateIndicator()) );
	connect( this, SIGNAL(rangeStepChanged()), this, SLOT(Slot_UpdateIndicator()) );
}

int QGameRangeIndicator::getRange() const { 
	return range; 
}
void QGameRangeIndicator::setRange( int r ) { 
	if ( range != r ) {
		range = r;
		emit rangeChanged();
	}
}

int QGameRangeIndicator::getRangeStep() const {
	return rangeStep;
}
void QGameRangeIndicator::setRangeStep( int r ) {
	if ( rangeStep != r ) {
		rangeStep = r;
		emit rangeStepChanged();
	}	
}

QRectF QGameRangeIndicator::boundingRect() const {
	return QRectF( 
		-( rangeStep + rangeStep * range ) * 0.5,
		-( rangeStep + rangeStep * range ) * 0.5,
		(rangeStep + rangeStep * range)+1,
		(rangeStep + rangeStep * range)+1);
}

void QGameRangeIndicator::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
	painter->drawPixmap( pos(), indicator );
}

void QGameRangeIndicator::Slot_UpdateIndicator() {
	indicator = QPixmap( rangeStep + rangeStep * range, rangeStep + rangeStep * range );
	indicator.fill( QColor( Qt::transparent ) );
	
	QPainter painter( &indicator );
	painter.setPen( QColor( Qt::black ) );
	painter.drawRect( boundingRect() );
	
	Expand_r( painter, indicator.rect().center(), 1 );
}

void QGameRangeIndicator::Expand_r( QPainter & painter, const QPointF & pt, const int currentRange ) {
	const qreal radius = 10.0f;
	
	if ( currentRange <= range ) {
		for ( int i = -1; i <= 1; i++ ) {
			if ( i == 0 ) {
				continue;
			}
			
			const qreal expandx = pt.x() + rangeStep * i;
			const qreal expandy = pt.y();
			QObject * existingItem = NULL;//root.childAt( expandx, expandy )
			if ( existingItem == NULL ) {
				QRectF rect( expandx, expandy, rangeStep, rangeStep );
				painter.drawRoundedRect( rect, radius, radius, Qt::AbsoluteSize );
				painter.drawText( rect, Qt::AlignCenter, QString( "%1" ).arg( currentRange ) );
			}
		}

		for ( int i = -1; i <= 1; i++ ) {
			if ( i == 0 ) {
				continue;
			}

			const qreal expandx = pt.x();
			const qreal expandy = pt.y() + rangeStep * i;
			QObject * existingItem = NULL;//root.childAt( expandx, expandy )
			if ( existingItem == NULL ) {
				QRectF rect( expandx, expandy, rangeStep, rangeStep );
				painter.drawRoundedRect( rect, radius, radius, Qt::AbsoluteSize );
				painter.drawText( rect, Qt::AlignCenter, QString( "%1" ).arg( currentRange ) );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

QGameScenario::QGameScenario( QObject * parent ) :
	QDeclarativeItem( NULL ) {

	/*setFlag( QGraphicsItem::ItemHasNoContents, false );
	setFlag( QGraphicsItem::ItemIsSelectable, false );
	setAcceptedMouseButtons( Qt::LeftButton );*/

	//setSize( QSizeF( tileSetImage.width(), tileSetImage.height() ) );
}
	
QGameScenario::~QGameScenario() {
}

void QGameScenario::addGameObject( QObject * obj ) {
	// add it to the master object list
	gameObjects.append( obj );

	// we want to know when they are destroyed
	connect( obj, SIGNAL(destroyed(QObject*)), this, SLOT(Slot_GameObjectDestroyed(QObject*)));
}

void QGameScenario::startBattle() {
	// reset turn state
	for ( int i = 0; i < gameObjects.count(); ++i ) {
		//gameObjects[ i ]->startBattle( this );
		QMetaObject::invokeMethod( gameObjects[ i ], "startBattle" );

		// we want notification when the object finishes its turn
		//connect( gameObjects[ i ], SIGNAL(turnEnded(QGameObject*)), this, SLOT(Slot_GameObjectTurnFinished(QGameObject*)) );
	}

	NotifyNextBestGameObjectOfTurn();
}

void QGameScenario::endBattle() {
	for ( int i = 0; i < gameObjects.count(); ++i ) {
		//gameObjects[ i ]->endBattle( this );
		QMetaObject::invokeMethod( gameObjects[ i ], "endBattle" );

		//disconnect( gameObjects[ i ], SIGNAL(turnEnded(QGameObject*)), );
	}
}

void QGameScenario::Slot_GameObjectDestroyed( QObject * obj ) {
	gameObjects.removeAll( obj );
}

//void QGameScenario::Slot_GameObjectTurnFinished( QGameObject * obj ) {
//	obj->setProperty( "turnTaken", true );
//
//	NotifyNextBestGameObjectOfTurn();
//}

void QGameScenario::NotifyNextBestGameObjectOfTurn() {
	float bestInitiative = 0.0f;
	QObject * bestObject = NULL;

	for ( int i = 0; i < gameObjects.count(); ++i ) {
		QVariant propInitiative = gameObjects[ i ]->property( "initiative" );

		bool good = false;
		const float initiative = propInitiative.toFloat( &good );
		if ( good ) {
			if ( initiative > bestInitiative ) {
				bestInitiative = initiative;
				bestObject = gameObjects[ i ];
			}
		}
	}

	if ( bestObject != NULL ) {
		QTimer::singleShot( 0, bestObject, SIGNAL(yourTurn()) );
	}
}
