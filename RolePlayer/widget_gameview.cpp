#include <QDebug>
#include <QWheelEvent>

#include "widget_gameview.h"

QGameView::QGameView( QWidget *parent ) :
	QDeclarativeView( parent ) {
}

QGameView::QGameView( const QUrl &source, QWidget *parent ) :
	QDeclarativeView( source, parent ) {
}

QGameView::~QGameView() {
}

QGameScene * QGameView::getGameScene() {
	return qobject_cast<QGameScene*>( scene() );
}

void QGameView::contextMenuEvent( QContextMenuEvent * event ) {
	QDeclarativeView::contextMenuEvent( event );
}

void QGameView::dragEnterEvent( QDragEnterEvent * event ) {
	QDeclarativeView::dragEnterEvent( event );
}

void QGameView::dragLeaveEvent( QDragLeaveEvent * event ) {
	QDeclarativeView::dragLeaveEvent( event );
}

void QGameView::dragMoveEvent( QDragMoveEvent * event ) {
	QDeclarativeView::dragMoveEvent( event );
}

void QGameView::dropEvent( QDropEvent * event ) {
	QDeclarativeView::dropEvent( event );
}

void QGameView::focusInEvent( QFocusEvent * event ) {
	QDeclarativeView::focusInEvent( event );
}

bool QGameView::focusNextPrevChild( bool next ) {
	return QDeclarativeView::focusNextPrevChild( next );
}

void QGameView::focusOutEvent( QFocusEvent * event ) {
	QDeclarativeView::focusOutEvent( event );
}

void QGameView::inputMethodEvent( QInputMethodEvent * event ) {
	QDeclarativeView::inputMethodEvent( event );
}

void QGameView::keyPressEvent( QKeyEvent * event ) {
	QDeclarativeView::keyPressEvent( event );
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
	QDeclarativeView::keyReleaseEvent( event );
}

void QGameView::mouseDoubleClickEvent( QMouseEvent * event ) {
	QDeclarativeView::mouseDoubleClickEvent( event );
}

void QGameView::mouseMoveEvent( QMouseEvent * event ) {
	QDeclarativeView::mouseMoveEvent( event );
}

void QGameView::mousePressEvent( QMouseEvent * event ) {
	if ( event->button() == Qt::MiddleButton ) {
		setDragMode( QGraphicsView::ScrollHandDrag );
		return;
	}
	QDeclarativeView::mousePressEvent( event );
}

void QGameView::mouseReleaseEvent( QMouseEvent * event ) {
	if ( event->button() == Qt::MiddleButton ) {
		setDragMode( QGraphicsView::NoDrag );
		return;
	}
	QDeclarativeView::mouseReleaseEvent( event );
}

void QGameView::paintEvent( QPaintEvent * event ) {
	QDeclarativeView::paintEvent( event );
}

void QGameView::resizeEvent( QResizeEvent * event ) {
	QDeclarativeView::resizeEvent( event );
}

void QGameView::scrollContentsBy( int dx, int dy ) {
	QDeclarativeView::scrollContentsBy( dx, dy );
}

void QGameView::showEvent( QShowEvent * event ) {
	QDeclarativeView::showEvent( event );
}

bool QGameView::viewportEvent( QEvent * event ) {
	return QDeclarativeView::viewportEvent( event );
}

void QGameView::wheelEvent( QWheelEvent * event ) {
	const float scrollAmount = 1.15f;
	if( event->delta() > 0 ) {
		scale( scrollAmount, scrollAmount );
	} else {
		scale( 1.0f / scrollAmount, 1.0f / scrollAmount );
	}
}
