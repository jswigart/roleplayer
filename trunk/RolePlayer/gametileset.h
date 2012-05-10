#ifndef GAME_TILESET_H
#define GAME_TILESET_H

#include <QImage>
#include <QPointer>
#include <QFileInfo>
#include <QDeclarativeItem>

//////////////////////////////////////////////////////////////////////////

class QGameTileSet;
class QGameTile : public QDeclarativeItem {
Q_OBJECT
public:
	QPixmap		GetPixMap();

	QGameTile( QDeclarativeItem *parent = 0 );
	~QGameTile();
signals:
	void		TileSelected( QGameTile * tile );
protected:
	void		mousePressEvent( QGraphicsSceneMouseEvent *event );
	void		paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget );
private:
	QImage		cachedImage;
};

//////////////////////////////////////////////////////////////////////////

class QGameTileSet : public QDeclarativeItem {
	Q_OBJECT	
public:	
	const QImage & GetImage() const { return tileSetImage; }
	
	QGameTileSet( const QFileInfo & file, const QImage & image );
	~QGameTileSet();
signals:
	void	TileSelected( QGameTile * tile );
private slots:
	void	Slot_TileSelected( QGameTile * tile );
protected:
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget );
private:
	QFileInfo			tileSetFile;
	QImage				tileSetImage;
};

#endif // GAME_TILESET_H
