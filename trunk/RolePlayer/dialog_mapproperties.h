#ifndef DIALOG_MAP_PROPERTIES
#define DIALOG_MAP_PROPERTIES

#include <QDialog>
#include <QString>
#include <QFuture>
#include <QFutureWatcher>

class QGameTileMap;
class QDialogMapPropertiesImpl;
class QDialogMapProperties : public QDialog
{
	Q_OBJECT
public:
	QDialogMapProperties( QGameTileMap * view, QWidget *parent = 0, Qt::WindowFlags f = 0 );
	~QDialogMapProperties();	
private:
	QDialogMapPropertiesImpl *		impl;
	
private Q_SLOTS:
	void Slot_Finished( int result );
};

#endif // DIALOG_IMPORT_TILES
