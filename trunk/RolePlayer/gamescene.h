#ifndef SCENE_H
#define SCENE_H

#include <QUrl>
#include <QPointer>
#include <QGraphicsScene>

#include "tool.h"

class RolePlayer;
class QGameTileMap;

//////////////////////////////////////////////////////////////////////////

class QGameLayer : public QObject {
	Q_OBJECT
	Q_PROPERTY( bool hidden READ getHidden WRITE setHidden DESIGNABLE true NOTIFY hiddenStatusChanged );
	Q_PROPERTY( bool locked READ getLocked WRITE setLocked DESIGNABLE true NOTIFY lockedStatusChanged );
public:
	bool	getHidden() const { return isHidden; }
	bool	getLocked() const { return isLocked; }
	
	QGameLayer( QObject * parent );
	~QGameLayer();
signals:
	void	hiddenStatusChanged();
	void	lockedStatusChanged();
public slots:
	void	setHidden( bool b ) { isHidden = b; }
	void	setLocked( bool b ) { isLocked = b; }
public:
	bool	isHidden;
	bool	isLocked;
};

//////////////////////////////////////////////////////////////////////////

class QGameScene : public QGraphicsScene {
	Q_OBJECT
	Q_PROPERTY( QUrl saveFileUrl READ getSaveFileUrl DESIGNABLE true NOTIFY saveFileUrlChanged );
public:
	RolePlayer *		getApp() { return rootApp; };

	QTool *				getCurrentTool() { return currentTool; }
	void				setCurrentTool( QTool * tool );
	
	const QUrl &		getSaveFileUrl() const { return saveFileUrl; }
	void				setSaveFileUrl( const QUrl & url ) { saveFileUrl = url; } 

	QGameTileMap *		getMapAtPosition( const QPointF & scenePos );

	//void				render( QPainter * painter, const QRectF & target, const QRectF & source, Qt::AspectRatioMode aspectRatioMode );
	
	void				getLayerNames( QStringList & names );

	QGameLayer *		getLayer( const QString & name, const bool create = false );
		
	void				save( const QUrl & file );
	void				load( const QUrl & file );

	QGameScene( RolePlayer * app, QObject * parent = 0 );
	~QGameScene();
private:
Q_SIGNALS:
	void				layersChanged();
	void				saveFileUrlChanged(QUrl);
private Q_SLOTS:
protected:
	//void	contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent );
	//void	dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
	//void	dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );
	//void	dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
	//void	drawBackground ( QPainter * painter, const QRectF & rect );
	//void	drawForeground ( QPainter * painter, const QRectF & rect );
	//void	dropEvent ( QGraphicsSceneDragDropEvent * event );
	//void	focusInEvent ( QFocusEvent * focusEvent );
	//void	focusOutEvent ( QFocusEvent * focusEvent );
	//void	helpEvent ( QGraphicsSceneHelpEvent * helpEvent );
	//void	inputMethodEvent ( QInputMethodEvent * event );
	void	keyPressEvent ( QKeyEvent * keyEvent );
	void	keyReleaseEvent ( QKeyEvent * keyEvent );
	//void	mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void	mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	//void	wheelEvent ( QGraphicsSceneWheelEvent * wheelEvent );
private:
	RolePlayer *			rootApp;
	QObject *				rootLayers;

	QPointer<QTool>			currentTool;

	QUrl					saveFileUrl;
};

#endif // MAP_OBJECT_H
