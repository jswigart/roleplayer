#ifndef GAME_SCENARIO_H
#define GAME_SCENARIO_H

#include <QPointer>
#include <QFileInfo>
#include <QDeclarativeItem>

// http://xizhizhu.blogspot.com/2010/10/hybrid-application-using-qml-and-qt-c.html

//////////////////////////////////////////////////////////////////////////

class QGameScenario;
class QGameObject : public QDeclarativeItem {
	Q_OBJECT
	//Q_PROPERTY( QString ruleset READ getRuleSet WRITE setRuleSet DESIGNABLE true NOTIFY ruleSetChanged );
public:

	void	startBattle( QGameScenario * scenario );
	void	endBattle( QGameScenario * scenario );
	void	turnNotify( QGameScenario * scenario );
	
	QGameObject( QDeclarativeItem * parent = 0 );
protected:
private:
	QUrl	componentUrl;
signals:
	void	startRound();
	void	turnEnded();
	void	yourTurn();
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
	void			Slot_GameObjectTurnFinished( QGameObject * obj );
private:
	//QString								ruleset;

	QObjectList							gameObjects;

	//QList< QPointer<QGraphicsObject> >		gameObjects;
	
	void			NotifyNextBestGameObjectOfTurn();
};

#endif // GAME_SCENARIO_H
