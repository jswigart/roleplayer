#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QVarLengthArray>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "tool.h"

#include "roleplayer.h"
#include "gamescene.h"
#include "gametilemap.h"
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
	rootApp( app ) {

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

QGameTileMap * QGameScene::getMapAtPosition( const QPointF & scenePos ) {
	QList<QGraphicsItem *> objects = items( scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder );
	for ( int i = 0; i < objects.count(); ++i ) {
		QGameTileMap * tileMap = qobject_cast<QGameTileMap*>( objects[ i ] );
		if ( tileMap != NULL ) {
			return tileMap;
		}
	}
	return NULL;
}

//void QGameScene::render( QPainter * painter, const QRectF & target, const QRectF & source, Qt::AspectRatioMode aspectRatioMode ) {
//}

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
