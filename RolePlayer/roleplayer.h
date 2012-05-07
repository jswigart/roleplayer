#ifndef ROLEPLAYER_H
#define ROLEPLAYER_H

#include <QPointer>
#include <QMap>
#include <QtGui/QMainWindow>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QTimer>

#include "widget_labelclickable.h"
#include "ui_roleplayer.h"

class QLabelClickable;
class QStandardItem;
class QFileSystemWatcher;
class QStandardItemModel;

class RolePlayer : public QMainWindow {
	Q_OBJECT
public:
	RolePlayer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RolePlayer();

private:
	Ui::RolePlayer			ui;

	struct icons_t {
		QIcon		critical;
		QIcon		folderOpen;
		QIcon		folderClosed;
	} icons;
	
	typedef QMap< QString, QStandardItem* > ItemMap;
	struct tiles_t {
		QStandardItemModel *		model;
		ItemMap						rootItems;
	} layers;

	struct fileList_t {
		QFileInfoList							tiles;

		QFutureWatcher<QImage>					async_LoadTiles;
	} fileList;

	QFileSystemWatcher 		fileWatcher;
	QTimer					fileRefresh;

	QList< QImage >		masterTileList;

	QPointer<QLabelClickable>	selectedTile;

	void				AddMapEditTab( const QString & name );

	void				FindAllFileTypes( const QString & path, const QStringList & fileTypes, QFileInfoList & files );

	void				RebuildTileThumbnails();

	static QImage		Async_LoadImages( const QFileInfo & file );
private Q_SLOTS:
	void				Slot_PopulateTileList();
	void				Slot_TabChanged( int index );
	void				Slot_TabCloseRequested( int index );
	void				Slot_DirectoryChanged( const QString & path );
	void				Slot_FileChanged( const QString & path );
	void				Slot_TreeItemExpanded( const QModelIndex & index );
	void				Slot_TreeItemCollapse( const QModelIndex & index );
	void				Slot_ImageLoadedAt( int index );
	void				Slot_ImageLoadFinished();
	void				Slot_TileLabelClicked( QLabelClickable * label );

	// menu actions
	void				Action_MapProperties();
	void				Action_NewEditTab();
	void				Action_Save();
	void				Action_SaveAll();
	void				Action_SaveMapImage();
	void				Action_ImportTiles();
};

#endif // ROLEPLAYER_H
