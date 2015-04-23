#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <stdio.h>

void makeAutostart(QString adress)
{
	QString usrName = qgetenv("USER").constData();

	QString text;
	text += "[Desktop Entry]\nType=Application\nExec=";
	text += adress + "/" + QApplication::applicationName() + "\n";
	text += "Hidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName[ru]=Qt\nName=Qt\nComment[ru]=\nComment=";

	QFile mFile;
	mFile.setFileName("/home/" + usrName + "/.config/autostart/alarm.desktop");

	mFile.open(QIODevice::WriteOnly);
	mFile.write(text.toLatin1());

	mFile.close();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	makeAutostart(QApplication::applicationDirPath());

	Dialog w;
	w.show();

	QApplication::setQuitOnLastWindowClosed(0); // Эта байда не закрывает приложение

	return a.exec();
}
