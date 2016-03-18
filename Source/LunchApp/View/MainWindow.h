#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"

class QSystemTrayIcon;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
				MainWindow( QWidget *parent = 0 );

				~MainWindow();
	
	void		showTrayMessage( const QString& msg );

private:
	Ui::MainWindow		ui;
	MetroView*			metroView;
	QSystemTrayIcon*	trayIcon;

	void				SendWeek();
	void				setupTray();
};
