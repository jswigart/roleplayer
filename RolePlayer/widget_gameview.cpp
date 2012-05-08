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
void QGameView::keyPressEvent ( QKeyEvent * keyEvent ) {
	/*if ( keyEvent->key() == Qt::Key_Space && !keyEvent->isAutoRepeat() ) {
		if ( dragMode() != QGraphicsView::ScrollHandDrag ) {
			setDragMode( QGraphicsView::ScrollHandDrag );
		}		
		keyEvent->accept();
		return;
	}*/
	QDeclarativeView::keyPressEvent( keyEvent );
}
void QGameView::keyReleaseEvent ( QKeyEvent * keyEvent ) {	
	if ( keyEvent->key() == Qt::Key_Space ) {
		if ( dragMode() != QGraphicsView::NoDrag ) {
			setDragMode( QGraphicsView::NoDrag );
		} else {
			setDragMode( QGraphicsView::ScrollHandDrag );
		}		
		keyEvent->accept();
		return;
	}
	QDeclarativeView::keyReleaseEvent( keyEvent );
}
void QGameView::wheelEvent( QWheelEvent * event ) {
	const float scrollAmount = 1.15f;
	if ( event->delta() > 0 ) {
		scale( scrollAmount, scrollAmount );
	} else {
		scale( 1.0f / scrollAmount, 1.0f / scrollAmount );
	}
}



