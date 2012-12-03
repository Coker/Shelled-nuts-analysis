#include "bil_495.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BIL_495 w;

	w.show();
	return a.exec();
}
