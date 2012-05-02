#ifndef ROLEPLAYER_H
#define ROLEPLAYER_H

#include <QtGui/QMainWindow>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QFileInfoList>
#include <QFileSystemWatcher>

#include "ui_roleplayer.h"
#include "ui_tilesettools.h"

class QFileSystemWatcher;

class RolePlayer : public QMainWindow
{
	Q_OBJECT

public:
	RolePlayer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RolePlayer();

private:
	Ui::RolePlayer			ui;
	Ui::TileSetGroup		uiTileGroup;

	struct fileList_t {
		QFileInfoList							tiles;

		QFutureWatcher<QImage>					futureWatcher_LoadTiles;
		QFuture<QImage>							future_LoadTiles;
	} fileList;
	//QFileSystemWatcher 		fileWatcher;

	void				FindAllFileTypes( const QString & path, const QStringList & fileTypes, QFileInfoList & files );

	void				PopulateTileList();
	void				RebuildTileThumbnails();

	static QImage		Async_LoadImages( const QFileInfo & file );

private Q_SLOTS:
	void				Async_ImageLoadedAt( int index );
	void				Async_ImageLoadFinished();

	void				Action_ImportTiles();
};

#endif // ROLEPLAYER_H
