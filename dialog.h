#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

private:
	Ui::Dialog *ui;
	QString hmTime;

public slots:
	void buttonClicked();
	void checkTime();
};

#endif // DIALOG_H
