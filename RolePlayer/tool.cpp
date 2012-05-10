#include <QAction>
#include <QStyle>
#include <QToolBar>
#include <QApplication>

#include "roleplayer.h"
#include "tool.h"
#include "gamescene.h"
#include "gametilemap.h"
#include "gametileset.h"

QTool::QTool( QObject * parent ) : 
	activeScene( NULL ) {
}

QTool::~QTool() {
}

void QTool::setScene( QGameScene * scene ) {
	activeScene = scene;
}

//////////////////////////////////////////////////////////////////////////

QToolSelector::QToolSelector( QObject * parent ) :
	QTool( parent ) {
}

QToolSelector::~QToolSelector() {
}

void QToolSelector::select() {
	overlay = activeScene->addRect( QRectF(), QPen( Qt::DashLine ), QBrush( QColor( 0, 50, 0, 50 ) ) );
}

void QToolSelector::unselect() {
	delete overlay;
	overlay = NULL;
}

void QToolSelector::complete() {
}

void QToolSelector::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( mouseEvent->buttons() & Qt::LeftButton ) {
		const QPointF p0 = mouseEvent->buttonDownScenePos( Qt::LeftButton );
		const QPointF p1 = mouseEvent->scenePos();

		qDebug() << p0 << p1;

		overlay->setRect( QRectF( p0, QSize( 1, 1 ) ).unite( QRectF( p1, QSize( 1, 1 ) ) ) );
	}	
}

void QToolSelector::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	mouseEvent->accept();
}

void QToolSelector::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay->rect().isValid() ) {
		QPainterPath selectPath;
		selectPath.addRect( overlay->rect() );
		activeScene->setSelectionArea( selectPath, Qt::IntersectsItemShape );

		// clear the selection overlay
		overlay->setRect( QRectF() );
	}
}

QAction * QToolSelector::setupAction( QToolBar * toolbar, const QKeySequence & key ) {
	QAction * action = toolbar->addAction( QApplication::style()->standardIcon( QStyle::SP_CommandLink ), "Select" );
	action->setToolTip( "Select" );
	action->setShortcut( key );
	action->setData( QVariant::fromValue<QTool*>( this ) );
	return action;
}

//////////////////////////////////////////////////////////////////////////

QToolPaintTile::QToolPaintTile( QObject * parent ) :
	QTool( parent ),
	overlay( NULL ) {
	
}

QToolPaintTile::~QToolPaintTile() {
}

void QToolPaintTile::select() {
	const QPixmap pixMap = selectedTile ? selectedTile->GetPixMap() : QPixmap();
	overlay = activeScene->addPixmap( pixMap );
}

void QToolPaintTile::unselect() {
	delete overlay;
	overlay = NULL;
}

void QToolPaintTile::complete() {
}

void QToolPaintTile::Slot_TileSelected( QGameTile * tile ) {
	if ( selectedTile == tile ) {
		
	} else {
		// clear the old overlay
		/*if ( !selectedTile.isNull() ) {
			selectedTile->setOverlay( QPixmap() );
		}*/

		selectedTile = tile;

		/*QPixmap highlight( label->pixmap()->size() );
		highlight.fill( QColor( 255, 0, 0, 100 ) );
		selectedTile->setOverlay( highlight );*/

		// if the tool is active, update the pixmap
		if ( overlay != NULL ) {
			overlay->setPixmap( selectedTile->GetPixMap() );
		}
	}
}

void QToolPaintTile::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay != NULL ) {
		overlay->setPos( mouseEvent->scenePos() );
		
		QGameTileMap * tileMap = activeScene->getMapAtPosition( mouseEvent->scenePos() );
		if ( tileMap != NULL ) {
			lastMap = tileMap;
		} 
		
		if ( lastMap != NULL ) {
			lastMap->snapToGrid( overlay, overlay->scenePos() );
		}

		// if button is held, we want to paint to new cells
		if ( mouseEvent->buttons() & Qt::LeftButton ) {
			
		}
	}
	mouseEvent->accept();
}

void QToolPaintTile::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay != NULL ) {
		QGameTileMap * tileMap = activeScene->getMapAtPosition( mouseEvent->scenePos() );
		if ( tileMap != NULL ) {
			lastMap = tileMap;
		} 

		if ( lastMap != NULL ) {
			QGraphicsPixmapItem * pix = activeScene->addPixmap( overlay->pixmap() );
			
			pix->setParentItem( lastMap );

			lastMap->snapToGrid( pix, mouseEvent->scenePos() );
		}
	}
	mouseEvent->accept();
}

QAction * QToolPaintTile::setupAction( QToolBar * toolbar, const QKeySequence & key ) {
	QAction * action = toolbar->addAction( QApplication::style()->standardIcon( QStyle::SP_CommandLink ), "Paint" );
	action->setToolTip( "Paint Tooltip" );
	action->setShortcut( key );
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

void QToolCreatePolygon::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent ) {
	if ( overlay == NULL ) {
		overlay = activeScene->addPath( QPainterPath(), QPen( QColor( "white" ), Qt::DashLine ), QBrush( QColor( 0, 0, 255, 50 ) ) );
	}

	vertices.append( mouseEvent->scenePos() );
	updateOverlay();
	mouseEvent->accept();
}

QAction * QToolCreatePolygon::setupAction( QToolBar * toolbar, const QKeySequence & key ) {
	QAction * action = toolbar->addAction( QApplication::style()->standardIcon( QStyle::SP_CommandLink ), "Polygon" );
	action->setToolTip( "Polygon Tooltip" );
	action->setShortcut( key );
	action->setData( QVariant::fromValue<QTool*>( this ) );
	return action;
}
