#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "ui_MainWindow.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );
				~MainWindow();
private:
	Ui::MainWindow	ui;
};

#endif // MAINWINDOW_H
