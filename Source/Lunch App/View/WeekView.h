#pragma once

#include <QWidget>

#include "DayView.h"


class WeekView : public QWidget
{
	Q_OBJECT

public:
					WeekView( QWidget *parent );
					~WeekView();

	virtual void	wheelEvent( QWheelEvent* event );

private:
	DayView*	mondayView;
	DayView*	tuesdayView;
	DayView*	wednesdayView;
	DayView*	thursdayView;
	DayView*	fridayView;

	void			init();
};
