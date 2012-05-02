#include "roleplayer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RolePlayer w;
	w.show();
	return a.exec();
}
