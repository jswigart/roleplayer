#include <QAction>
#include <QStyle>
#include <QToolBar>
#include <QApplication>

#include "widget_labelclickable.h"

#include "roleplayer.h"
#include "tool.h"
#include "gamescene.h"
#include "gametilemap.h"

QTool::QTool( QObject * parent ) : 
	activeScene( NULL ) {
}

QTool::~QTool() {
}

void QTool::setScene( QGameScene * scene ) {
	activeScene = scene;
}

//////////////////////////////////////////////////////////////////////////

QToolPaintTile::QToolPaintTile( QObject * parent ) :
	QTool( parent ),
	overlay( NULL ) {
	
}

QToolPaintTile::~QToolPaintTile() {
}

void QToolPaintTile::Slot_TileSelected( QLabelClickable * label ) {
	if ( selectedTile == label ) {
		
	} else {
		// clear the old overlay
		if ( !selectedTile.isNull() ) {
			selectedTile->setOverlay( QPixmap() );
		}

		selectedTile = label;

		QPixmap highlight( label->pixmap()->size() );
		highlight.fill( QColor( 255, 0, 0, 100 ) );
		selectedTile->setOverlay( highlight );

		// if the tool is active, update the pixmap
		if ( overlay != NULL ) {
			overlay->setPixmap( *label->pixmap() );
		}
	}
}

void QToolPaintTile::select() {
	const QPixmap * pixMap = selectedTile ? selectedTile->pixmap() : NULL;
	overlay = activeScene->addPixmap( pixMap != NULL ? *pixMap : QPixmap() );
}

void QToolPaintTile::unselect() {
	delete overlay;
	overlay = NULL;
}

void QToolPaintTile::complete() {
}

void QToolPaintTile::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay != NULL ) {
		overlay->setPos( mouseEvent->scenePos() );
		
		QGameTileMap * tileMap = activeScene->getMapAtPosition( mouseEvent->scenePos() );
		if ( tileMap != NULL ) {
			tileMap->snapToGrid( overlay, overlay->scenePos() );
		}
	}
}

void QToolPaintTile::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay != NULL ) {
		QGraphicsPixmapItem * pix = activeScene->addPixmap( overlay->pixmap() );
		pix->setPos( overlay->pos() );

		QGameTileMap * tileMap = activeScene->getMapAtPosition( mouseEvent->scenePos() );
		if ( tileMap != NULL ) {
			tileMap->snapToGrid( pix, pix->scenePos() );
		}
	}
}

void QToolPaintTile::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
}

QAction * QToolPaintTile::setupAction( QToolBar * toolbar, const QObject * receiver, const char * member ) {
	QAction * action = toolbar->addAction( QApplication::style()->standardIcon( QStyle::SP_CommandLink ), "Paint", receiver, member );
	action->setToolTip( "Paint Tooltip" );
	action->setShortcut( QKeySequence( "1" ) );
	action->setData( QVariant::fromValue<QTool*>( this ) );
	return action;
}

//////////////////////////////////////////////////////////////////////////

QToolCreatePolygon::QToolCreatePolygon( QObject * parent, polyMode_t mode ) :
	QTool( parent ),
	overlay( NULL ),
	polyMode( mode ) {
}

QToolCreatePolygon::~QToolCreatePolygon() {
	delete overlay;
}

void QToolCreatePolygon::select() {
}

void QToolCreatePolygon::unselect() {
	if ( overlay != NULL ) {
		delete overlay;
		overlay = NULL;

		vertices.clear();
	}
}

void QToolCreatePolygon::complete() {
	if ( overlay != NULL ) {
		if ( vertices.count() > 2 ) {
			if ( polyMode == MODE_POLYGON ) {
				activeScene->addPolygon( QPolygonF( vertices ), QPen( QColor( "black" ) ), QBrush( "blue" ) );
			} else if ( polyMode == MODE_CHAIN ) {
				activeScene->addPath( overlay->path(), QPen( QColor( "black" ) ) );
			}
		}
		
		// delete the overlay so a new one must be started
		vertices.clear();
		updateOverlay();
	}
}

void QToolCreatePolygon::updateOverlay() {
	if ( overlay != NULL ) {
		QPainterPath path;
		if ( vertices.count() > 0 ) {
			switch( polyMode ) {
			case MODE_POLYGON:
				path.addPolygon( QPolygonF( vertices ) );
				break;
			case MODE_CHAIN:
				path = QPainterPath( vertices[ 0 ] );
				for ( int i = 1; i < vertices.count(); ++i ) {
					path.lineTo( vertices[ i ] );
				}
				break;
			}
		}
		overlay->setPath( path );
	}
}

void QToolCreatePolygon::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
}

void QToolCreatePolygon::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay == NULL ) {
		overlay = activeScene->addPath( QPainterPath(), QPen( QColor( "white" ), Qt::DashLine ), QBrush( QColor( 0, 0, 255, 50 ) ) );
	}

	vertices.append( mouseEvent->scenePos() );
	updateOverlay();
}

void QToolCreatePolygon::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
}

QAction * QToolCreatePolygon::setupAction( QToolBar * toolbar, const QObject *receiver, const char* member ) {
	QAction * action = toolbar->addAction( QApplication::style()->standardIcon( QStyle::SP_CommandLink ), "Polygon", receiver, member );
	action->setToolTip( "Polygon Tooltip" );
	action->setShortcut( QKeySequence( "2" ) );
	action->setData( QVariant::fromValue<QTool*>( this ) );
	return action;
}
