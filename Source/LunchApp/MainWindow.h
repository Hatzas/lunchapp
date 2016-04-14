#pragma once

#include <QWidget>

#include "ui_MainWindow.h"
#include "View/MetroView.h"
#include "Controller/Controller.h"
#include <QSystemTrayIcon>


class MainWindow : public QWidget
{
	Q_OBJECT

public:
						MainWindow( QWidget *parent = 0 );
						~MainWindow();

public slots:
			void		switchAdministrate( bool );

			void		showNotification( const QString& msg );
			void		showNotification( const QString& msg, const QPixmap& pixmap );

signals:
			void		selectionChangedOn( const Dish& dish );

protected:
	virtual void		resizeEvent( QResizeEvent * event );

private slots:
			void		onTrayActivation( QSystemTrayIcon::ActivationReason reason );
	virtual void		closeEvent( QCloseEvent* event );

private:
			Ui::MainWindow		ui;

			MetroView*			regularMetroView;
			MetroView*			adminMetroView;
			Controller*			controller;
	
			QSystemTrayIcon*	trayIcon;
			QMenu*				trayIconMenu;

			bool				adminMode;

			void				sendDummyWeek();

			void				setupTray();
			void				setupController();

			void				makeConnections();

};
