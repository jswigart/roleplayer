#ifndef GAME_TILESET_H
#define GAME_TILESET_H

#include <QPointer>
#include <QDeclarativeItem>

//////////////////////////////////////////////////////////////////////////

class QGameTileSet;
class QGameTile : public QDeclarativeItem {
Q_OBJECT
public:
	QGameTile( QDeclarativeItem *parent = 0 );
	~QGameTile();
};

//////////////////////////////////////////////////////////////////////////

class QGameTileSet : public QDeclarativeItem {
	Q_OBJECT	
public:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
	
	QGameTileSet( const QList< QImage > & tiles, QDeclarativeItem *parent = 0 );
	~QGameTileSet();
private:
signals:
public slots:
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
private:
	QImage							atlas;
	QPointer<QDeclarativeItem>		selectedChild;
};

#endif // GAME_TILESET_H
