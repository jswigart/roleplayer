#include <QPainter>

#include "widget_labelclickable.h"

QLabelClickable::QLabelClickable( QWidget *parent, Qt::WindowFlags f ) :
	QLabel( parent, f ) {
}

QLabelClickable::QLabelClickable( const QString &text, QWidget *parent, Qt::WindowFlags f ) :
	QLabel( text, parent, f ) {
}

QLabelClickable::~QLabelClickable() {
}

void QLabelClickable::setOverlay( const QPixmap & pic ) { 
	overlay = pic;
	repaint();
}

void QLabelClickable::mousePressEvent( QMouseEvent * event ) {
	emit clicked( this );
}

void QLabelClickable::paintEvent( QPaintEvent * event ) {
	QLabel::paintEvent( event );

	if ( !overlay.isNull() ) {
		 QPainter painter( this );
		 painter.drawPixmap( rect(), overlay );
	}
}