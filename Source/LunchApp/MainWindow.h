#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"
<<<<<<< HEAD:Source/LunchApp/View/MainWindow.h
#include <QSystemTrayIcon>
=======
#include "Controller/Controller.h"

class QSystemTrayIcon;
>>>>>>> GUI:Source/LunchApp/MainWindow.h

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );
				~MainWindow();
	
	void		showTrayMessage(const QString& msg);

private slots:
	void		onTrayActivation(QSystemTrayIcon::ActivationReason reason);

signals:
	void		SelectionChangedOn( const Dish& dish );

private:
	Ui::MainWindow		ui;

	MetroView*			metroView;
	Controller*			controller;

	QSystemTrayIcon*	trayIcon;
	QMenu*				trayIconMenu;

	void				sendWeek();

	void				setupTray();
	void				setupController();
};
