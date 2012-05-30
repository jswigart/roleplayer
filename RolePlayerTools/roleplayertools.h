#ifndef ROLEPLAYERTOOLS_H
#define ROLEPLAYERTOOLS_H

#include <QDeclarativeExtensionPlugin>
#include <QDeclarativeItem>
#include <QGraphicsObject>
#include <QDebug>

#include "roleplayertools_global.h"

//////////////////////////////////////////////////////////////////////////
// http://xizhizhu.blogspot.com/2010/10/hybrid-application-using-qml-and-qt-c.html

class QRolePlayerQmlPlugin : public QDeclarativeExtensionPlugin {
	Q_OBJECT
public:
	void registerTypes( const char *uri );
};

//////////////////////////////////////////////////////////////////////////

class QGameRangeIndicator : public QGraphicsObject {
	Q_OBJECT
	Q_PROPERTY( int range READ getRange WRITE setRange DESIGNABLE true NOTIFY rangeChanged );
	Q_PROPERTY( int rangeStep READ getRangeStep WRITE setRangeStep DESIGNABLE true NOTIFY rangeStepChanged );
	Q_PROPERTY( QColor rangeColor READ getRangeColor WRITE setRangeColor DESIGNABLE true NOTIFY rangeColorChanged );
	Q_PROPERTY( qreal cellRadius READ getCellRadius WRITE setCellRadius DESIGNABLE true NOTIFY cellRadiusChanged );
	Q_PROPERTY( bool includeDiagonal READ getIncludeDiagonal WRITE setIncludeDiagonal DESIGNABLE true NOTIFY includeDiagonalChanged );
public:
	int				getRange() const;
	void			setRange( int r );

	int				getRangeStep() const;
	void			setRangeStep( int r );

	QColor			getRangeColor() const;
	void			setRangeColor( QColor color );

	qreal			getCellRadius() const;
	void			setCellRadius( qreal radius );

	bool			getIncludeDiagonal() const;
	void			setIncludeDiagonal( bool include );

	QGameRangeIndicator( QGraphicsItem * parent = 0 );
protected:
	QRectF			boundingRect() const;

	void			paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget );
private:
	int				range;
	int				rangeStep;
	QColor			rangeColor;
	qreal			cellRadius;
	bool			rangeIncludeDiagonal;

	QPixmap			indicator;
private:
	bool			initialized;
signals:
	void			rangeChanged();
	void			rangeStepChanged();
	void			rangeColorChanged();
	void			cellRadiusChanged();
	void			includeDiagonalChanged();
private slots:
	void			Slot_UpdateIndicator();
};

//////////////////////////////////////////////////////////////////////////

class QGameScenario : public QDeclarativeItem {
	Q_OBJECT
public:	
	Q_INVOKABLE void		addGameObject( QObject * obj );
	Q_INVOKABLE void		startBattle();
	Q_INVOKABLE void		endBattle();

	QGameScenario( QObject * parent = 0 );
	~QGameScenario();
signals:
private slots:
	void					Slot_GameObjectDestroyed( QObject * obj );
private:
	QObjectList				gameObjects;

	//QList< QPointer<QGraphicsObject> >		gameObjects;
	
	void					NotifyNextBestGameObjectOfTurn();
};

//////////////////////////////////////////////////////////////////////////

#endif // ROLEPLAYERTOOLS_H
