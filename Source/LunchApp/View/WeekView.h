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
	virtual void	resizeEvent( QResizeEvent* event );

			void	selectionChangedOn( const Dish& dish );		

	const Week&		getWeek()							{ return week; }	

private:
	Week					week;
	std::vector<DayView*>	dayViewsVect;

	void			init();

	void			AddDays();

};
