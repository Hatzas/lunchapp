#pragma once

#include <QWidget>

#include "Model/Week.h"
#include "DayView.h"


class WeekView : public QWidget
{
	Q_OBJECT

public:
							WeekView( QWidget *parent, const Week& week, bool editMode = false );
							~WeekView();

	virtual void			wheelEvent( QWheelEvent* event );
			void			mainWindowResized( QSize size );

			void			selectionChangedOn( const Dish& dish );		

	const Week&				getWeek()										{ return week; }	

public slots:
			void			showDayMenuNotification();

private:
	Week					week;
	std::vector<DayView*>	dayViewsVect;
	bool					editMode;

	void					init();

	void					AddDays();

};
