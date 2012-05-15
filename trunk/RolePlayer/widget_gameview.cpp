#include <QDebug>
#include <QWheelEvent>
#include <QVarLengthArray>

#include "widget_gameview.h"

QGameView::QGameView( QWidget *parent ) :
	QGraphicsView( parent ) {
}

QGameView::QGameView( QGraphicsScene * scene, QWidget * parent ) :
	QGraphicsView( scene, parent ) {
}

QGameView::~QGameView() {
}

QGameScene * QGameView::getGameScene() {
	return qobject_cast<QGameScene*>( scene() );
}

void QGameView::contextMenuEvent( QContextMenuEvent * event ) {
	QGraphicsView::contextMenuEvent( event );
}

void QGameView::dragEnterEvent( QDragEnterEvent * event ) {
	QGraphicsView::dragEnterEvent( event );
}

void QGameView::dragLeaveEvent( QDragLeaveEvent * event ) {
	QGraphicsView::dragLeaveEvent( event );
}

void QGameView::dragMoveEvent( QDragMoveEvent * event ) {
	QGraphicsView::dragMoveEvent( event );
}

void QGameView::dropEvent( QDropEvent * event ) {
	QGraphicsView::dropEvent( event );
}

void QGameView::focusInEvent( QFocusEvent * event ) {
	QGraphicsView::focusInEvent( event );
}

bool QGameView::focusNextPrevChild( bool next ) {
	return QGraphicsView::focusNextPrevChild( next );
}

void QGameView::focusOutEvent( QFocusEvent * event ) {
	QGraphicsView::focusOutEvent( event );
}

void QGameView::inputMethodEvent( QInputMethodEvent * event ) {
	QGraphicsView::inputMethodEvent( event );
}

void QGameView::keyPressEvent( QKeyEvent * event ) {
	QGraphicsView::keyPressEvent( event );
}

void QGameView::keyReleaseEvent( QKeyEvent * event ) {
	if( event->key() == Qt::Key_Space ) {
		if( dragMode() != QGraphicsView::NoDrag ) {
			setDragMode( QGraphicsView::NoDrag );
		} else {
			setDragMode( QGraphicsView::ScrollHandDrag );
		}		
		event->accept();
		return;
	}
	QGraphicsView::keyReleaseEvent( event );
}

void QGameView::mouseDoubleClickEvent( QMouseEvent * event ) {
	QGraphicsView::mouseDoubleClickEvent( event );
}

void QGameView::mouseMoveEvent( QMouseEvent * event ) {
	QGraphicsView::mouseMoveEvent( event );
}

void QGameView::mousePressEvent( QMouseEvent * event ) {
	if ( event->button() == Qt::MiddleButton ) {
		setDragMode( QGraphicsView::ScrollHandDrag );
		return;
	}
	QGraphicsView::mousePressEvent( event );
}

void QGameView::mouseReleaseEvent( QMouseEvent * event ) {
	if ( event->button() == Qt::MiddleButton ) {
		setDragMode( QGraphicsView::NoDrag );
		return;
	}
	QGraphicsView::mouseReleaseEvent( event );
}

void QGameView::paintEvent( QPaintEvent * event ) {
	QGraphicsView::paintEvent( event );
}

void QGameView::resizeEvent( QResizeEvent * event ) {
	QGraphicsView::resizeEvent( event );
}

void QGameView::scrollContentsBy( int dx, int dy ) {
	QGraphicsView::scrollContentsBy( dx, dy );
}

void QGameView::showEvent( QShowEvent * event ) {
	QGraphicsView::showEvent( event );
}

bool QGameView::viewportEvent( QEvent * event ) {
	return QGraphicsView::viewportEvent( event );
}

void QGameView::wheelEvent( QWheelEvent * event ) {
	const float scrollAmount = 1.15f;
	if( event->delta() > 0 ) {
		scale( scrollAmount, scrollAmount );
	} else {
		scale( 1.0f / scrollAmount, 1.0f / scrollAmount );
	}
}
