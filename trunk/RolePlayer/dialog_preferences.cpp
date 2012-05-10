#include <QToolBar>
#include <QAction>

#include "roleplayer.h"
#include "dialog_preferences.h"

Q_DECLARE_METATYPE(QAction*);

QDialogPreferences::QDialogPreferences( RolePlayer * app, Qt::WindowFlags f ) :
	QDialog( app, f ) {	
	
	ui.setupUi( this );
	
	AddKeyGroup( "Menu", "Main Menus", app->menuBar() );
	AddKeyGroup( "Main", "Main Toolbar", app->GetMainToolBar() );
	AddKeyGroup( "Tools", "Main Toolbar", app->GetToolsToolBar() );
	
	// get notifications when they change
	connect(ui.propTree->propManagers.editKeySequence, SIGNAL(valueChanged(QtProperty*, const QKeySequence&)),
		this, SLOT(Key_Changed(QtProperty*, const QKeySequence&)));
}

QDialogPreferences::~QDialogPreferences() {
}

void QDialogPreferences::AddKeyGroup( const QString & name, const QString & tooltip, QMenuBar * bar ) {
	QtProperty * group = ui.propTree->AddGroup( NULL, name, tooltip );
	for ( int a = 0; a < bar->actions().count(); ++a ) {
		QAction * action = bar->actions()[ a ];
		AddKeyGroup_r( group, name, tooltip, action );
	}		
}

void QDialogPreferences::AddKeyGroup_r( QtProperty * group, const QString & name, const QString & tooltip, QAction * action ) {
	if ( action->menu() == NULL ) {
		QtProperty * prop = ui.propTree->AddKeySequence( group, action->toolTip(), action->text(), action->shortcut() );
		prop->setData( QVariant::fromValue<QAction*>( action ) );
	} else {
		QtProperty * subGroup = ui.propTree->AddGroup( group, action->toolTip(), action->text() );
		for ( int a = 0; a < action->menu()->actions().count(); ++a ) {
			QAction * subAction = action->menu()->actions()[ a ];
			AddKeyGroup_r( subGroup, subAction->text(), subAction->toolTip(), subAction );
		}
	}
}

void QDialogPreferences::AddKeyGroup( const QString & name, const QString & tooltip, QToolBar * bar ) {
	QtProperty * group = ui.propTree->AddGroup( NULL, name, tooltip );
	for ( int a = 0; a < bar->actions().count(); ++a ) {
		QAction * action = bar->actions()[ a ];
		QtProperty * prop = ui.propTree->AddKeySequence( group, action->toolTip(), action->text(), action->shortcut() );
		prop->setData( QVariant::fromValue<QAction*>( action ) );
	}		
}

void QDialogPreferences::Key_Changed( QtProperty * prop, const QKeySequence & key ) {
	QAction * action = prop->data().value<QAction*>();
	if ( action != NULL ) {
		action->setShortcut( key );
	}
}
