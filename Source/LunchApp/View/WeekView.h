#pragma once

#include <QWidget>

#include "Model/Week.h"
#include "DayView.h"


class WeekView : public QWidget
{
	Q_OBJECT

public:
					WeekView( QWidget *parent, const Week& week );
					~WeekView();

	virtual void	wheelEvent( QWheelEvent* event );

private:
	Week					week;
	std::vector<DayView*>	dayViewsVect;

	void			init();

	void			AddDays();

};
