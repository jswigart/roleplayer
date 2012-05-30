#include <QtGui/QApplication>
#include <QLibraryInfo>

#include "roleplayer.h"

#include "gamecharacter.h"
#include "gamescenario.h"

int main( int argc, char *argv[] ) {
	QApplication a( argc, argv );

	//////////////////////////////////////////////////////////////////////////
	//QLibraryInfo::location(QLibraryInfo::PluginsPath)
	/*QStringList libPaths;
	libPaths.append( "./resources/plugins" );
	QCoreApplication::setLibraryPaths( libPaths );*/
	//////////////////////////////////////////////////////////////////////////
	
	RolePlayer w;
	w.show();
	return a.exec();
}
