#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDateTimeEdit>
#include <QVector>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QScrollArea>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QSessionManager>

class Dialog : public QDialog//, public QApplication
{
	Q_OBJECT

private:
	QScrollArea*					sa;
	QVector<QDateTimeEdit*>				dateTimeVec;
	QVector<QPushButton*>				pushButtonVec;
	QPushButton*					plusButton;
	QVBoxLayout*					left;
	QVBoxLayout*					right;
	QHBoxLayout*					main;
	QSettings*					setting;
	QString						text;
	QVector<QString>				textVec;
	QSystemTrayIcon*				trayIcon;
	QMenu*						trayMenu;
	QPushButton*					toTray;
	int						vertSize = 300;

	void					SaveSettings();
	void					LoadSettings();
	void					keyPressEvent(QKeyEvent* k);

public:
	Dialog(QWidget *parent = 0);
	~Dialog();

public slots:
	void				addToTray();
	void				plusButtonClicked();
	void				deleteButtonClicked();
	void				finishedEditing();
	void				changedDateTime(const QDateTime&);
	void				checkAlarms();
	void				slotShowHide();
};

#endif // DIALOG_H
