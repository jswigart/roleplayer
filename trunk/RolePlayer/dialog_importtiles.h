#ifndef DIALOG_IMPORT_TILES
#define DIALOG_IMPORT_TILES

#include <QDialog>
#include <QString>
#include <QFuture>
#include <QFutureWatcher>

#include "ui_tileset_import.h"

class QDialogImportTiles : public QDialog
{
	Q_OBJECT
public:
	QDialogImportTiles( const QList< QImage > & existingTiles, const QString & tileSetFile, QWidget *parent = 0, Qt::WindowFlags f = 0 );
	~QDialogImportTiles();

	struct tileInfo_t {
		QString		name;
		QImage		set;
		QPoint		coord;
		QPoint		size;
		QImage		tile;
	};

	void getImportedTiles( QList< tileInfo_t > & tilesOut );

	//const QList< QImage > & getTiles() const { return async_CreateTiles.result(); }
private:
	Ui::ImportTileDialog			ui;
	
	QList< QImage >					existingTiles;

	QFutureWatcher< QImage >		async_LoadTileSet;

	QFuture< tileInfo_t	>			async_CreateTiles;
	QFutureWatcher< tileInfo_t >	async_CreateTilesWatcher;

	// functions called asycronously
	//QList< QImage >			Async_ExtractTiles( const extractTilesInfo_t tileInfo );	
	static tileInfo_t		Async_CreateTile( const tileInfo_t & tileInfo );
private Q_SLOTS:
	void					Slot_CreateTilesFromTileSet();
	void					Slot_TileCreatedAt( int index );
	void					Slot_PopulateTileView();
};

#endif // DIALOG_IMPORT_TILES
