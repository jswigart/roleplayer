#ifndef GAME_SCENARIO_H
#define GAME_SCENARIO_H

#include <QPointer>
#include <QFileInfo>
#include <QDeclarativeItem>

// http://xizhizhu.blogspot.com/2010/10/hybrid-application-using-qml-and-qt-c.html

//////////////////////////////////////////////////////////////////////////

class QGameScenario;
class QGameRangeIndicator : public QGraphicsObject {
	Q_OBJECT
	Q_PROPERTY( int range READ getRange WRITE setRange DESIGNABLE true NOTIFY rangeChanged );
	Q_PROPERTY( int rangeStep READ getRangeStep WRITE setRangeStep DESIGNABLE true NOTIFY rangeStepChanged );
public:
	int getRange() const;
	void setRange( int r );

	int getRangeStep() const;
	void setRangeStep( int r );

	QGameRangeIndicator( QGraphicsItem * parent = 0 );
protected:

	QRectF		boundingRect() const;

	void		paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget );
private:
	int		range;
	int		rangeStep;

	QPixmap	indicator;

	void	Expand_r( QPainter & painter, const QPointF & pt, const int range );
signals:
	void	rangeChanged();
	void	rangeStepChanged();
private slots:
	void	Slot_UpdateIndicator();
};

//////////////////////////////////////////////////////////////////////////

class QGameScenario : public QDeclarativeItem {
	Q_OBJECT
	//Q_PROPERTY( QString ruleset READ getRuleSet WRITE setRuleSet DESIGNABLE true NOTIFY ruleSetChanged );
public:
	/*const QString & getRuleSet() const { return ruleset; }
	void setRuleSet( const QString & rule ) { ruleset = rule; }*/
	
	Q_INVOKABLE void addGameObject( QObject * obj );
	Q_INVOKABLE void startBattle();
	Q_INVOKABLE void endBattle();
	
	QGameScenario( QObject * parent = 0 );
	~QGameScenario();
signals:
	void			ruleSetChanged();
private slots:
	void			Slot_GameObjectDestroyed( QObject * obj );
private:
	//QString								ruleset;

	QObjectList							gameObjects;

	//QList< QPointer<QGraphicsObject> >		gameObjects;
	
	void			NotifyNextBestGameObjectOfTurn();
};

#endif // GAME_SCENARIO_H
