#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "gamescenario.h"

QGameObject::QGameObject( QDeclarativeItem * parent ) :
	QDeclarativeItem( parent ) {
}

void QGameObject::startBattle( QGameScenario * scenario ) {
	connect( this, SIGNAL(turnEnded(QGameObject*)), scenario, SLOT(Slot_GameObjectTurnFinished(QGameObject*)) );

	setProperty( "turnTaken", false );
	
	// let the QML calculate the round start
	emit startRound();
}

void QGameObject::endBattle( QGameScenario * scenario ) {
	disconnect( scenario, SLOT(Slot_GameObjectTurnFinished(QGameObject*)) );

	setProperty( "turnTaken", false );
}

void QGameObject::turnNotify( QGameScenario * scenario ) {
	/*QMetaObject::invokeMethod(this, "takeTurn",
		Q_RETURN_ARG(QVariant, returnedValue),
		Q_ARG(QVariant, msg));*/
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
		connect( gameObjects[ i ], SIGNAL(turnEnded(QGameObject*)), this, SLOT(Slot_GameObjectTurnFinished(QGameObject*)) );
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
	gameObjects.removeAll( qobject_cast<QGameObject*>( obj ) );
}

void QGameScenario::Slot_GameObjectTurnFinished( QGameObject * obj ) {
	obj->setProperty( "turnTaken", true );

	NotifyNextBestGameObjectOfTurn();
}

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
