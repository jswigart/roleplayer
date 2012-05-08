#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <QDeclarativeItem>

class QGameTileMap : public QDeclarativeItem {
	Q_OBJECT
	Q_PROPERTY( bool gridRender READ getDrawGrid WRITE setDrawGrid DESIGNABLE true NOTIFY gridEnabledChanged );
	Q_PROPERTY( int gridSize READ getGridSize WRITE setGridSize DESIGNABLE true NOTIFY gridSizeChanged );
	Q_PROPERTY( QColor gridColor READ getGridColor WRITE setGridColor DESIGNABLE true NOTIFY gridColorChanged );
	Q_PROPERTY( QUrl saveFileUrl READ getSaveFileUrl DESIGNABLE true NOTIFY saveFileUrlChanged );
public:
	bool gridEnabled() const { return gridRender; }
	int getGridSize() const { return gridSize; }
	const QColor & getGridColor() const { return gridColor; }
	const QUrl & getSaveFileUrl() const { return saveFileUrl; }
	void setSaveFileUrl( const QUrl & url ) { saveFileUrl = url; } 

	bool getDrawGrid() const { return gridRender; }
	
	void snapToGrid( QDeclarativeItem * item, const QPointF & scenePos );

	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
	
	void save( const QUrl & file );

	QGameTileMap( QDeclarativeItem *parent = 0 );
	~QGameTileMap();
private:
Q_SIGNALS:
	void gridEnabledChanged(bool);
	void gridSizeChanged(int);
	void gridColorChanged(QColor);
	void saveFileUrlChanged(QUrl);
public Q_SLOTS:
	void setDrawGrid( bool b ) { gridRender = b; }
	void setGridSize( int size ) { gridSize = size; }
	void setGridColor( const QColor & clr ) { gridColor = clr; }
	void redrawView();
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void wheelEvent( QWheelEvent * event );
private:
	int			gridSize;
	bool		gridRender;
	QColor		gridColor;
	QUrl		saveFileUrl;
};

#endif // MAP_OBJECT_H
