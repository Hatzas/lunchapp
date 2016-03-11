#pragma once

#include <QWidget>

class DayView : public QWidget
{
	Q_OBJECT

public:
					DayView( QWidget *parent, QString dayName );
					~DayView();

	virtual void	wheelEvent( QWheelEvent* event );

private:
	QString		dayName;

	void			init();
};
