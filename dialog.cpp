#include "dialog.h"
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QAction>
#include <QProcess>

Dialog::Dialog(QWidget *parent): QDialog(parent), sa(new QScrollArea), left(new QVBoxLayout), right(new QVBoxLayout),
	main(new QHBoxLayout), plusButton(new QPushButton("+"/*, this*/))
{
	setting = new QSettings("MyPro", "mysetting");

	right->addWidget(plusButton);
	main->addLayout(left); main->addLayout(right);
	this->setLayout(main);

	plusButton->setMinimumWidth(40);
	connect(plusButton, SIGNAL(clicked()), this, SLOT(plusButtonClicked()));

	sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	sa->setWidget(this);
	this->setFixedSize(400, vertSize);
	sa->resize(this->size() += QSize(20, 0));
	sa->show();

	LoadSettings();

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(checkAlarms()));
	timer->start(30000);

	QPushButton* toTray = new QPushButton("toTray");
	right->addWidget(toTray);
	connect(toTray, SIGNAL(clicked()), this, SLOT(addToTray()));
}

Dialog::~Dialog()
{
	delete left; delete right; delete main;
	delete sa;

	for(int i = 0; i < dateTimeVec.size(); i++)
		delete dateTimeVec[i];		dateTimeVec.clear();
	for(int i = 0; i < pushButtonVec.size(); i++)
		delete pushButtonVec[i];	pushButtonVec.clear();

	delete plusButton;
	delete setting;
	delete trayIcon;
	delete trayMenu;
}

void Dialog::plusButtonClicked()
{
	this->setFixedSize(400, vertSize+=30);

	QDateTime dt(QDate().currentDate(), QTime(0, 0, 0));
	QHBoxLayout* hb = new QHBoxLayout;
	QPushButton* btn = new QPushButton(QString().number(pushButtonVec.size()));
	pushButtonVec.push_back(btn);

	btn->setMinimumWidth(30);

	dateTimeVec.push_back(new QDateTimeEdit(dt, this));

	hb->addWidget(btn);
	hb->addWidget(dateTimeVec.back());

	left->addLayout(hb);

	connect((dateTimeVec.back()), SIGNAL(dateTimeChanged(const QDateTime&)), this, SLOT(changedDateTime(QDateTime)));
	connect(btn, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));

	SaveSettings();
}

void Dialog::deleteButtonClicked()
{
	this->setFixedSize(400, vertSize-=30);

	int i = ((QPushButton*)sender())->text().toInt();

	delete dateTimeVec[i];
	dateTimeVec.erase(dateTimeVec.begin() + i);
	delete pushButtonVec[i];
	pushButtonVec.erase(pushButtonVec.begin() + i);

	for(int j = 0; j < pushButtonVec.size(); j++)
		pushButtonVec[j]->setText(QString().number(j));
	SaveSettings();
}

void Dialog::finishedEditing()
{
	qDebug() << "finishedEditing()";
	SaveSettings();
}

void Dialog::changedDateTime(const QDateTime& dt)
{
	qDebug() << "changedDateTime(): " << dt;

	SaveSettings();
}

void Dialog::SaveSettings()
{
	qDebug() << "Saving";

	setting->beginGroup("Dialog");
		for(int i = 0; i < dateTimeVec.size(); i++)
			setting->setValue(QString().number(i), dateTimeVec[i]->dateTime());
		setting->setValue("size", dateTimeVec.size());
	setting->endGroup();
}

void Dialog::LoadSettings()
{
	int i = 0;

	setting->beginGroup("Dialog");
		int size = setting->value("size").toInt();

		while(i < size)
		{
			plusButtonClicked();
			QDateTime dt(setting->value(QString().number(i)).toDateTime());
			dateTimeVec.back()->setDateTime(dt);
			++i;
		}
	setting->endGroup();
}

void Dialog::checkAlarms()
{
	QDateTime dt(QDate().currentDate(), QTime().currentTime().addMSecs(-QTime().currentTime().msec()).addSecs(-QTime().currentTime().second()));
	qDebug() << "checkAlarms() " << dt;
	for(int i = 0; i < dateTimeVec.size(); i++)
	{
		if(dateTimeVec[i]->dateTime() == dt)
		{
			QProcess::startDetached("aplay /home/maxim/lol3.wav");
			QMessageBox::information(this,"Realy quit?", "Are you sure?", tr("Yes"), tr("No"));
		}
	}
}

void Dialog::slotShowHide()
{
	sa->show();
}

void Dialog::keyPressEvent(QKeyEvent* k)
{
	if(k->key() == Qt::Key_Escape)
		k->ignore();
}

void Dialog::addToTray()
{
	static bool trayState = 0;
	if(trayState == 0)
	{
		trayState = 1;

		trayIcon = new QSystemTrayIcon(this);
		QPixmap pixmap(15, 15);
		pixmap.fill(Qt::red);
		QIcon icon(pixmap);
		trayIcon->setIcon(icon);
		trayIcon->show();

		QAction* pactShowHide = new QAction("Show Application Window", this);
		connect(pactShowHide, SIGNAL(triggered()), this, SLOT(slotShowHide()));

		trayMenu = new QMenu(this);
		trayMenu->addAction(pactShowHide);
		trayIcon->setContextMenu(trayMenu);
	}
}
