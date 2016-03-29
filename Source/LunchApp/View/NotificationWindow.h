#pragma once

#include <QLabel>
#include <QVector>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QPaintEvent>


class NotificationWindow : public QWidget
{
	Q_OBJECT

public:
	explicit					NotificationWindow( QSystemTrayIcon* trayIcon, const QString &text, const int duration);

protected:
	virtual	void				paintEvent( QPaintEvent * event );

public slots:
			bool				close();
			void				moveOut();

private:
	static	QVector<NotificationWindow *>	windows;
	static	int								lastIdx;

			QSystemTrayIcon*				trayIcon;
			QString							text;
	
			QLabel*							textLabel;

			QPropertyAnimation*				moveAnimation;

			void				init();

			void				moveIn();
};