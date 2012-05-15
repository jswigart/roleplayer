#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "tool.h"

#include "roleplayer.h"
#include "gamescene.h"
#include "dialog_mapproperties.h"

//////////////////////////////////////////////////////////////////////////

QGameLayer::QGameLayer( QObject * parent ) :
	QObject( parent ),
	isHidden( false ),
	isLocked( false ) {
}

QGameLayer::~QGameLayer() {
}

//////////////////////////////////////////////////////////////////////////

QGameScene::QGameScene( RolePlayer * app, QObject * parent ) :
	QGraphicsScene( parent ),
	rootApp( app ),
	gridSize( 32.0f ), 
	gridRender( true ),
	gridColor( 127, 127, 127, 127 ) {

	// create some child objects to help with sorting
	rootLayers = new QObject( this );
	rootLayers->setObjectName( "layers" );

	// add a base layer
	getLayer( "Base", true );
	getLayer( "Collision", true );
}

QGameScene::~QGameScene() {
}

void QGameScene::getLayerNames( QStringList & names ) {
	const QObjectList & layers = rootLayers->children();
	for ( int i = 0; i < layers.count(); ++i ) {
		names.append( layers[ i ]->objectName() );
	}
}

QGameLayer * QGameScene::getLayer( const QString & name, const bool create ) {
	QGameLayer * layer = rootLayers->findChild<QGameLayer*>( name );
	if ( layer == NULL ) {
		layer = new QGameLayer( rootLayers );
		layer->setObjectName( name );

		emit layersChanged();
	}
	return layer;
}

void QGameScene::save( const QUrl & file ) {
	setSaveFileUrl( file );

	QFile saveFile( file.path() );
	if ( !saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate) ) {
		return;
	}

	QTextStream out( &saveFile );

	out << "Scene: " << objectName() << "\n";
	
	QList<QByteArray> dynProps = dynamicPropertyNames();
	for ( int i = 0; i < dynProps.count(); ++i ) {
		out << QString( dynProps[ i ] ) << "\n";
	}

	saveFile.close();
}

void QGameScene::load( const QUrl & file ) {
	setSaveFileUrl( file );

	// todo:
}

void QGameScene::setCurrentTool( QTool * tool ) {
	if ( currentTool != NULL ) {
		currentTool->unselect();
		currentTool->setScene( NULL );
	}
	currentTool = tool;
	if ( currentTool != NULL ) {
		currentTool->setScene( this );
		currentTool->select();
	}
}

void QGameScene::snapToGrid( QGraphicsItem * item, const QPointF & scenePos ) {
	const int cellSizeX = item->boundingRect().width() / getGridSize();
	const int cellSizeY = item->boundingRect().height() / getGridSize();

	const int mapSizeX = ( sceneRect().width() / getGridSize() ) - cellSizeX;
	const int mapSizeY = ( sceneRect().height() / getGridSize() ) - cellSizeY;

	const int cellX = scenePos.x() / getGridSize();
	const int cellY = scenePos.y() / getGridSize();

	item->setPos( QPointF( cellX, cellY ) * getGridSize() );
}

void QGameScene::drawBackground ( QPainter * painter, const QRectF & rect ) {
	painter->fillRect( rect, QBrush( Qt::gray ) );
	if ( getDrawGrid() ) {
		//painter->setPen( Qt::DashLine );
		//painter->setWorldMatrixEnabled(true);
		QPen gridPen( gridColor );
		painter->setPen( gridPen );
		
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
	QGraphicsScene::drawBackground( painter, rect );
}

void QGameScene::keyPressEvent ( QKeyEvent * keyEvent ) {
	if ( currentTool != NULL ) {
		if ( keyEvent->key() == Qt::Key_Escape ) {
			currentTool->unselect();
			currentTool = NULL;
			return;
		}
		if ( keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return ) {
			currentTool->complete();
			return;
		}
		currentTool->keyPressEvent( keyEvent );
	}

	if ( !keyEvent->isAccepted() ) {
		QGraphicsScene::keyPressEvent( keyEvent );
	}
}

void QGameScene::keyReleaseEvent( QKeyEvent * keyEvent ) {
	if ( currentTool != NULL ) {
		currentTool->keyReleaseEvent( keyEvent );
	}

	if ( !keyEvent->isAccepted() ) {
		QGraphicsScene::keyReleaseEvent( keyEvent );
	}
}

void QGameScene::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( currentTool != NULL ) {
		currentTool->mouseMoveEvent( mouseEvent );
	}

	if ( !mouseEvent->isAccepted() ) {
		QGraphicsScene::mouseMoveEvent( mouseEvent );
	}
}

void QGameScene::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( currentTool != NULL ) {
		currentTool->mousePressEvent( mouseEvent );
	}

	if ( !mouseEvent->isAccepted() ) {
		QGraphicsScene::mousePressEvent( mouseEvent );
	}
}

void QGameScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( currentTool != NULL ) {
		currentTool->mouseReleaseEvent( mouseEvent );
	}

	if ( !mouseEvent->isAccepted() ) {
		QGraphicsScene::mouseReleaseEvent( mouseEvent );
	}
}
