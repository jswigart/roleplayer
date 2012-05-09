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

class QTool;
class QLabelClickable;
class QStandardItem;
class QFileSystemWatcher;
class QStandardItemModel;
class QDeclarativeComponent;

class RolePlayer : public QMainWindow {
	Q_OBJECT
public:
	RolePlayer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RolePlayer();

	QToolBar *			GetMainToolBar() { return ui.mainToolBar; }
	QToolBar *			GetToolsToolBar() { return toolBarTools; }	

	void				AppendToLog( const QString & msg );
signals:
	void				TileSelected( QLabelClickable * label );
private Q_SLOTS:
	void				Slot_ToolTriggered( QAction * action );
	void				Slot_PopulateTileList();
	void				Slot_TabChanged( int index );
	void				Slot_TabCloseRequested( int index );
	void				Slot_DirectoryChanged( const QString & path );
	void				Slot_FileChanged( const QString & path );
	void				Slot_TreeItemExpanded( const QModelIndex & index );
	void				Slot_TreeItemCollapse( const QModelIndex & index );
	void				Slot_ImageLoadedAt( int index );
	void				Slot_ImageLoadFinished();
	void				Slot_TileSelected( QLabelClickable * label );
	void				Slot_RefreshPropertyList();

	// menu actions
	void				Action_Preferences();
	void				Action_MapProperties();
	void				Action_NewFile();
	void				Action_SaveFile();
	void				Action_SaveFileAll();
	void				Action_SaveFileMapImage();
	void				Action_ImportTiles();

private:
	Ui::RolePlayer			ui;

	struct icons_t {
		QIcon		critical;
		QIcon		folderNew;
		QIcon		folderOpen;
		QIcon		folderClosed;
		QIcon		saveFile;
	} icons;
	
	struct fileList_t {
		QFileInfoList				tiles;
		QFutureWatcher<QImage>		async_LoadTiles;
	} fileList;

	QFileSystemWatcher 			fileWatcher;
	QTimer						fileRefresh;

	QList< QImage >				masterTileList;
	
	QPointer<QToolBar>			toolBarTools;
	
	void				CacheIcons();
	void				SetupToolBars();
	void				ConnectSlots();
	void				InitObjectPallette();
	QDeclarativeComponent * CacheQMLComponent( QDeclarativeEngine * engine, const QUrl & file );

	void				AddMapEditTab( const QString & name );

	void				FindAllFileTypes( const QString & path, const QStringList & fileTypes, QFileInfoList & files );

	void				RebuildTileThumbnails();

	static QImage		Async_LoadImages( const QFileInfo & file );
};

#endif // ROLEPLAYER_H
