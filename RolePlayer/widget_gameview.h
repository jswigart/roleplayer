#ifndef WIDGET_VIEW
#define WIDGET_VIEW

#include <QPointer>
#include <QDeclarativeView>

#include "gamescene.h"

class QGameView : public QGraphicsView
{
	Q_OBJECT
public:
	QGameScene *	getGameScene();

	explicit QGameView( QWidget *parent = 0 );
	explicit QGameView( QGraphicsScene * scene, QWidget * parent = 0 );
	~QGameView();
protected:
	virtual void	contextMenuEvent ( QContextMenuEvent * event );
	virtual void	dragEnterEvent ( QDragEnterEvent * event );
	virtual void	dragLeaveEvent ( QDragLeaveEvent * event );
	virtual void	dragMoveEvent ( QDragMoveEvent * event );
	virtual void	dropEvent ( QDropEvent * event );
	virtual void	focusInEvent ( QFocusEvent * event );
	virtual bool	focusNextPrevChild ( bool next );
	virtual void	focusOutEvent ( QFocusEvent * event );
	virtual void	inputMethodEvent ( QInputMethodEvent * event );
	virtual void	keyPressEvent ( QKeyEvent * event );
	virtual void	keyReleaseEvent ( QKeyEvent * event );
	virtual void	mouseDoubleClickEvent ( QMouseEvent * event );
	virtual void	mouseMoveEvent ( QMouseEvent * event );
	virtual void	mousePressEvent ( QMouseEvent * event );
	virtual void	mouseReleaseEvent ( QMouseEvent * event );
	virtual void	paintEvent ( QPaintEvent * event );
	virtual void	resizeEvent ( QResizeEvent * event );
	virtual void	scrollContentsBy ( int dx, int dy );
	virtual void	showEvent ( QShowEvent * event );
	virtual bool	viewportEvent ( QEvent * event );
	virtual void	wheelEvent ( QWheelEvent * event );
private:

};

#endif // WIDGET_VIEW
