#ifndef CHARACTER_H
#define CHARACTER_H

#include <QDeclarativeItem>

class QGameCharacter : public QDeclarativeItem
{
	Q_OBJECT
	//Q_PROPERTY( bool drawGrid READ getDrawGrid WRITE setDrawGrid DESIGNABLE true NOTIFY gridEnabledChanged );
public:
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget );

	QGameCharacter( QDeclarativeItem *parent = 0 );
	~QGameCharacter();
private:
Q_SIGNALS:
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );


private:
};

#endif // MAP_OBJECT_H
