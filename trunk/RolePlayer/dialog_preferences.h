#ifndef DIALOG_PREFERENCES
#define DIALOG_PREFERENCES

#include <QDialog>

#include "ui_dialog_preferences.h"

class QToolBar;
class RolePlayer;
class QDialogPreferences : public QDialog
{
	Q_OBJECT
public:
	QDialogPreferences( RolePlayer * app, Qt::WindowFlags f = 0 );
	~QDialogPreferences();	
private:
	Ui::DialogPreferences				ui;

	void AddKeyGroup( const QString & name, const QString & tooltip, QToolBar * bar );
private slots:
	void Key_Changed( QtProperty * prop, const QKeySequence & key );
};

#endif // DIALOG_PREFERENCES
