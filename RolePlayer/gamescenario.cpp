#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "gamescenario.h"

//QGameRangeIndicator::QGameRangeIndicator( QGraphicsItem * parent ) :
//	QGraphicsObject( parent ),
//	range( 0 ),
//	rangeStep( 0 ),
//	rangeColor( Qt::black ),
//	cellRadius( 10.0f ),
//	rangeIncludeDiagonal( false ) {
//	connect( this, SIGNAL(rangeChanged()), this, SLOT(Slot_UpdateIndicator()) );
//	connect( this, SIGNAL(rangeStepChanged()), this, SLOT(Slot_UpdateIndicator()) );
//	connect( this, SIGNAL(rangeColorChanged()), this, SLOT(Slot_UpdateIndicator()) );
//	connect( this, SIGNAL(cellRadiusChanged()), this, SLOT(Slot_UpdateIndicator()) );
//	connect( this, SIGNAL(includeDiagonalChanged()), this, SLOT(Slot_UpdateIndicator()) );	
//}
//
//int QGameRangeIndicator::getRange() const { 
//	return range; 
//}
//void QGameRangeIndicator::setRange( int r ) { 
//	if ( range != r ) {
//		range = r;
//		emit rangeChanged();
//	}
//}
//
//int QGameRangeIndicator::getRangeStep() const {
//	return rangeStep;
//}
//void QGameRangeIndicator::setRangeStep( int r ) {
//	if ( rangeStep != r ) {
//		rangeStep = r;
//		emit rangeStepChanged();
//	}	
//}
//
//QColor QGameRangeIndicator::getRangeColor() const {
//	return rangeColor;
//}
//
//void QGameRangeIndicator::setRangeColor( QColor color ) {
//	if ( rangeColor != color ) {
//		rangeColor = color;
//		emit rangeColorChanged();
//	}
//}
//
//qreal QGameRangeIndicator::getCellRadius() const {
//	return cellRadius;
//}
//
//void QGameRangeIndicator::setCellRadius( qreal radius ) {
//	if ( cellRadius != radius ) {
//		cellRadius = radius;
//		emit cellRadiusChanged();
//	}
//}
//
//bool QGameRangeIndicator::getIncludeDiagonal() const {
//	return rangeIncludeDiagonal;
//}
//
//void QGameRangeIndicator::setIncludeDiagonal( bool include ) {
//	if ( rangeIncludeDiagonal != include ) {
//		rangeIncludeDiagonal = include;
//		emit includeDiagonalChanged();
//	}
//}
//
//QRectF QGameRangeIndicator::boundingRect() const {
//	return QRectF( 
//		-( ( range + 1 ) * rangeStep ),
//		-( ( range + 1 ) * rangeStep ),
//		( rangeStep * 0.5f + ( range + 1 ) * rangeStep ) * 2.0f,
//		( rangeStep * 0.5f + ( range + 1 ) * rangeStep ) * 2.0f );
//}
//
//void QGameRangeIndicator::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget ) {
//	if ( !indicator.isNull() ) {
//		painter->drawPixmap( -indicator.rect().center() + QPointF( rangeStep*0.5f, rangeStep*0.5f ), indicator );
//	}
//}
//
//struct node_t {
//	QPoint pt;
//	int range;
//	bool operator==( const node_t & n ) const {
//		return pt == n.pt;
//	}
//};
//
//void QGameRangeIndicator::Slot_UpdateIndicator() {
//	if ( range <= 0 || rangeStep <=0 ) {
//		return;
//	}
//	
//	static float penWidth = 1.0f;
//
//	const float size = ( range + 1 ) * rangeStep * 2.0f + penWidth;
//	indicator = QPixmap( size, size );
//	indicator.fill( QColor( Qt::transparent ) );
//	
//	QPainter painter( &indicator );		
//	
//	QPointF basePt = indicator.rect().center() - QPointF( rangeStep*0.5f, rangeStep*0.5f );
//
//	QList<node_t> openList, closedList;
//
//	node_t rootNode;
//	rootNode.pt = QPoint( 0,0 );
//	rootNode.range = 0;
//
//	openList.append( rootNode );
//
//	while( !openList.isEmpty() ) {
//		node_t curNode = *openList.begin();
//		closedList.append( curNode );
//		openList.erase( openList.begin() );
//
//		QRectF rect( 
//			basePt.x() + curNode.pt.x() * rangeStep, 
//			basePt.y() + curNode.pt.y() * rangeStep, rangeStep, rangeStep );
//
//		painter.setPen( rangeColor ); 
//		painter.drawRoundedRect( rect, cellRadius, cellRadius, Qt::AbsoluteSize );
//		painter.drawText( rect, Qt::AlignCenter, QString( "%1" ).arg( curNode.range ) );
//
//		const QPoint expandOffsets[] = {
//			QPoint( -1,  0 ),
//			QPoint(  1,  0 ),
//			QPoint(  0, -1 ),
//			QPoint(  0,  1 ),
//			// diagonals
//			QPoint( -1, -1 ),
//			QPoint( -1,  1 ),
//			QPoint(  1,  1 ),			
//			QPoint(  1, -1 ),
//		};
//
//		const int numExpansions = rangeIncludeDiagonal ? 8 : 4;
//		for ( int i = 0; i < numExpansions; i++ ) {
//			node_t expandNode = curNode;
//			expandNode.pt += expandOffsets[ i ];
//			expandNode.range = curNode.range + 1;
//
//			if ( expandNode.range <= range ) {
//				if ( qFind( closedList, expandNode ) == closedList.end() && 
//					qFind( openList, expandNode ) == openList.end() ) {
//						openList.append( expandNode );
//				}
//			}
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////

//QGameScenario::QGameScenario( QObject * parent ) :
//	QDeclarativeItem( NULL ) {
//
//	/*setFlag( QGraphicsItem::ItemHasNoContents, false );
//	setFlag( QGraphicsItem::ItemIsSelectable, false );
//	setAcceptedMouseButtons( Qt::LeftButton );*/
//
//	//setSize( QSizeF( tileSetImage.width(), tileSetImage.height() ) );
//}
//	
//QGameScenario::~QGameScenario() {
//}
//
//void QGameScenario::addGameObject( QObject * obj ) {
//	// add it to the master object list
//	gameObjects.append( obj );
//
//	// we want to know when they are destroyed
//	connect( obj, SIGNAL(destroyed(QObject*)), this, SLOT(Slot_GameObjectDestroyed(QObject*)));
//}
//
//void QGameScenario::startBattle() {
//	// reset turn state
//	for ( int i = 0; i < gameObjects.count(); ++i ) {
//		//gameObjects[ i ]->startBattle( this );
//		QMetaObject::invokeMethod( gameObjects[ i ], "startBattle" );
//
//		// we want notification when the object finishes its turn
//		//connect( gameObjects[ i ], SIGNAL(turnEnded(QGameObject*)), this, SLOT(Slot_GameObjectTurnFinished(QGameObject*)) );
//	}
//
//	NotifyNextBestGameObjectOfTurn();
//}
//
//void QGameScenario::endBattle() {
//	for ( int i = 0; i < gameObjects.count(); ++i ) {
//		//gameObjects[ i ]->endBattle( this );
//		QMetaObject::invokeMethod( gameObjects[ i ], "endBattle" );
//
//		//disconnect( gameObjects[ i ], SIGNAL(turnEnded(QGameObject*)), );
//	}
//}
//
//void QGameScenario::Slot_GameObjectDestroyed( QObject * obj ) {
//	gameObjects.removeAll( obj );
//}
//
////void QGameScenario::Slot_GameObjectTurnFinished( QGameObject * obj ) {
////	obj->setProperty( "turnTaken", true );
////
////	NotifyNextBestGameObjectOfTurn();
////}
//
//void QGameScenario::NotifyNextBestGameObjectOfTurn() {
//	float bestInitiative = 0.0f;
//	QObject * bestObject = NULL;
//
//	for ( int i = 0; i < gameObjects.count(); ++i ) {
//		QVariant propInitiative = gameObjects[ i ]->property( "initiative" );
//
//		bool good = false;
//		const float initiative = propInitiative.toFloat( &good );
//		if ( good ) {
//			if ( initiative > bestInitiative ) {
//				bestInitiative = initiative;
//				bestObject = gameObjects[ i ];
//			}
//		}
//	}
//
//	if ( bestObject != NULL ) {
//		QTimer::singleShot( 0, bestObject, SIGNAL(yourTurn()) );
//	}
//}
