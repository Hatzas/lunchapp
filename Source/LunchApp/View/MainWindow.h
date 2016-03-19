#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"
#include <QSystemTrayIcon>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );

				~MainWindow();
	
	void		showTrayMessage(const QString& msg);

private slots:
	void		onTrayActivation(QSystemTrayIcon::ActivationReason reason);

private:
	Ui::MainWindow		ui;
	MetroView*			metroView;
	QSystemTrayIcon*	trayIcon;
	QMenu*				trayIconMenu;

	void				SendWeek();
	void				setupTray();
};
