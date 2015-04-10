#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QTimer>

Dialog::Dialog(QWidget *parent):QDialog(parent), ui(new Ui::Dialog)
{
	ui->setupUi(this);
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkTime()));
	timer->start(1000);
}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::buttonClicked()
{
	qDebug() << ui->timeEdit->time().toString().remove(5, 3);
	hmTime = ui->timeEdit->time().toString().remove(5, 3);
	this->hide();
}

void Dialog::checkTime()
{
	qDebug() << QTime().currentTime().toString().remove(5, 3);
	if(QTime().currentTime().toString().remove(5, 3) == hmTime)
		this->show();
}
