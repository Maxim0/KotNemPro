#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Dialog w;
	w.show();

	QApplication::setQuitOnLastWindowClosed(0); // Эта байда не закрывает приложение

	return a.exec();
}
