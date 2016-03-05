#ifndef LUNCHAPP_H
#define LUNCHAPP_H

#include <QtWidgets/QMainWindow>
#include "ui_lunchapp.h"

class LunchApp : public QMainWindow
{
	Q_OBJECT

public:
	LunchApp(QWidget *parent = 0);
	~LunchApp();

private:
	Ui::LunchAppClass ui;
};

#endif // LUNCHAPP_H
