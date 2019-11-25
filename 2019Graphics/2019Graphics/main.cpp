#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	if (argc != 3) {
		w.show();
	}
	else {
		w.Console(argv[1], argv[2]);
		a.exit();
	}
	return a.exec();
}
