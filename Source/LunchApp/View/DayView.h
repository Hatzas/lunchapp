#pragma once

#include <vector>
#include <QWidget>
#include <QLabel>

#include "Model/Day.h"
#include "DayDishesView.h"


class DayView : public QWidget
{
	Q_OBJECT

public:
						DayView( QWidget *parent, const Day& day );
						~DayView();

	virtual void		wheelEvent( QWheelEvent* event );
			void		mainWindowResized( QResizeEvent* event );

			void		selectionChangedOn( const Dish& dish );		

private:
	Day						day;
	QLabel*					dayNameLabel;
	DayDishesView*			dishesView;

	void				init();

};
