#pragma once

#include <QWidget>

#include "DishView.h"


class DayDishesView : public QWidget
{
	Q_OBJECT

public:
						DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect );
						~DayDishesView();

			void		init();

	virtual bool		event( QEvent* event );
	virtual void		wheelEvent( QWheelEvent* event );
			void		mainWindowResized( QResizeEvent* event );

	void				selectionChangedOn( const Dish& dish );

private:
	std::vector<DishView*>	disheViewsVect;
	std::vector<Dish>&		dishesVect;


	void				AddDishes();
	void				StackDishViews();
};
