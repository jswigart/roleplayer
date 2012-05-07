#include <QtGui/QApplication>

#include "roleplayer.h"

#include "gametilemap.h"
#include "gamecharacter.h"

int main( int argc, char *argv[] ) {
	QApplication a( argc, argv );

	qmlRegisterType<QGameTileMap>( "TileTools", 1, 0, "GameTileMap" );
	qmlRegisterType<QGameCharacter>( "TileTools", 1, 0, "GameCharacter" );

	RolePlayer w;
	w.show();
	return a.exec();
}
