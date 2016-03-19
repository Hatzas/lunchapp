#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"
#include "Controller/Controller.h"

class QSystemTrayIcon;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );
				~MainWindow();
	
	void		showTrayMessage( const QString& msg );

signals:
	void		SelectionChangedOn( const Dish& dish );

private:
	Ui::MainWindow		ui;

	MetroView*			metroView;
	Controller*			controller;

	QSystemTrayIcon*	trayIcon;

	void				sendWeek();

	void				setupTray();
	void				setupController();
};
