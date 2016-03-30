#pragma once

#include <QLabel>
#include <QVector>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimer>

#include "Style.h"


class NotificationWindow : public QWidget
{
	Q_OBJECT

public:
	explicit					NotificationWindow( const QString &text, 
									const std::vector<QPixmap>& iconsVect = std::vector<QPixmap>( 1, QPixmap( kAppIconPath ) ),
									const int duration = kNotificationShowTime );

	static	void				setup( QSystemTrayIcon* trayIcon );

protected:
	virtual	void				paintEvent( QPaintEvent * event );
	virtual	void				mousePressEvent( QMouseEvent * event );
	virtual	void				mouseReleaseEvent( QMouseEvent * event );

public slots:
			bool				close();
			void				moveOut();

private:
	static	QVector<NotificationWindow *>	windows;
	static	int								lastIdx;

	static	QSystemTrayIcon*				trayIcon;

			QString							text;
	const	std::vector<QPixmap>&			iconsVect;
	
			QLabel*							textLabel;

			QPropertyAnimation*				moveAnimation;

			bool							mousePressed;

			QTimer*							timer;

			void				init();

			void				moveIn();
};