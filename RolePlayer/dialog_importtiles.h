#ifndef DIALOG_IMPORT_TILES
#define DIALOG_IMPORT_TILES

#include <QDialog>
#include <QString>

#include "ui_tileset_import.h"

class QDialogImportTiles : public QDialog
{
	Q_OBJECT
public:
	QDialogImportTiles(const QString & tileSetImg, QWidget *parent = 0, Qt::WindowFlags f = 0);
	~QDialogImportTiles();

	const QList< QImage > & getTiles() const { return tiles; }
private:
	Ui::ImportTileDialog	ui;

	QString					tileSetFile;
	QImage					tileSet;
	QList< QImage >			tiles;

private Q_SLOTS:
	void updateTileList();
};

#endif // DIALOG_IMPORT_TILES
