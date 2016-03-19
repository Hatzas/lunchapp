#pragma once

#include <vector>
#include <QWidget>
#include <QLabel>

#include "Model/Day.h"
#include "DishView.h"


class DayView : public QWidget
{
	Q_OBJECT

public:
					DayView( QWidget *parent, const Day& day );
					~DayView();

	virtual void	wheelEvent( QWheelEvent* event );

			void	SelectionChangedOn( const Dish& dish );		

private:
	Day						day;

	QLabel*					dayNameLabel;
	std::vector<DishView*>	disheViewsVect;

	void			init();

	void			AddDishes();
	void			StackDishViews();

};
