#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );

				~MainWindow();
private:
	Ui::MainWindow	ui;
	MetroView*		metroView;

	void		SendWeek();
};
