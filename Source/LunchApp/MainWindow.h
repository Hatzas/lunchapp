#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "MetroView.h"
#include "Controller/Controller.h"
#include <QSystemTrayIcon>


class MainWindow : public QWidget
{
	Q_OBJECT

public:
						MainWindow( QWidget *parent = 0 );
						~MainWindow();
	
			void		showTrayMessage( const QString& msg );

signals:
			void		selectionChangedOn( const Dish& dish );

protected:
	virtual void		resizeEvent( QResizeEvent * event );

private slots:
			void		onTrayActivation( QSystemTrayIcon::ActivationReason reason );
	virtual void		closeEvent( QCloseEvent* event );

private:
			Ui::MainWindow		ui;

			MetroView*			metroView;
			Controller*			controller;
	
			QSystemTrayIcon*	trayIcon;
			QMenu*				trayIconMenu;

			void				sendWeek();

			void				setupTray();
			void				setupController();

			void				makeConnections();

};
