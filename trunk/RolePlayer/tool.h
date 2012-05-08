#ifndef TOOL_H
#define TOOL_H

#include <QPointer>
#include <QWidget>
#include <QMetaType>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

#include "widget_labelclickable.h"

class QAction;
class QToolBar;
class QGameScene;

//////////////////////////////////////////////////////////////////////////

class QTool : public QObject {
	Q_OBJECT
public:
	virtual void	setScene( QGameScene * scene );

	virtual void	select() = 0;
	virtual void	unselect() = 0;
	virtual void	complete() = 0;

	virtual void	keyPressEvent ( QKeyEvent * keyEvent ) {}
	virtual void	keyReleaseEvent ( QKeyEvent * keyEvent ) {}

	virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent ) {}
	virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent ) {}
	virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent ) {}

	virtual QAction * setupAction( QToolBar * toolbar, const QObject *receiver = NULL, const char* member = NULL ) = 0;
	
	QTool( QObject * parent );
	~QTool();
signals:
public slots:
protected:
	QGameScene *	activeScene;
};

//////////////////////////////////////////////////////////////////////////

class QToolPaintTile : public QTool {
	Q_OBJECT
public:
	virtual void		select();
	virtual void		unselect();
	virtual void		complete();

	virtual void		mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent );

	virtual QAction *	setupAction( QToolBar * toolbar, const QObject * receiver = NULL, const char * member = NULL );

	QToolPaintTile( QObject * parent );
	~QToolPaintTile();
private:
	QGraphicsPixmapItem *		overlay;	
	QPointer<QLabelClickable>	selectedTile;
signals:
public slots:
	void Slot_TileSelected( QLabelClickable * label );
};

//////////////////////////////////////////////////////////////////////////

class QToolCreatePolygon : public QTool {
	Q_OBJECT
public:
	enum polyMode_t {
		MODE_POLYGON,
		MODE_CHAIN,
	};

	virtual void		select();
	virtual void		unselect();
	virtual void		complete();

	virtual void		mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void		mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent );

	virtual QAction *	setupAction( QToolBar * toolbar, const QObject *receiver = NULL, const char* member = NULL );

	QToolCreatePolygon( QObject * parent, polyMode_t mode = MODE_CHAIN );
	~QToolCreatePolygon();
private:
	void				updateOverlay();

	polyMode_t				polyMode;

	QGraphicsPathItem  *	overlay;

	QVector<QPointF>		vertices;
signals:
public slots:
};

Q_DECLARE_METATYPE(QTool*);

#endif // TOOL_H
