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

			void			mainWindowResized( QSize size );

			void			selectionChangedOn( const Dish& dish );		

			Week&			getWeek()										{ return week; }	

protected:
	virtual	bool			event( QEvent *event );
	virtual void			wheelEvent( QWheelEvent* event );

private:
	Week					week;
	std::vector<DayView*>	dayViewsVect;
	bool					editMode;

	void					init();

	void					addDays();

};
