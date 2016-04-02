#pragma once

#include <QWidget>

#include "DishView.h"


class DayDishesView : public QWidget
{
	Q_OBJECT

public:
						DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect );
						~DayDishesView();

	virtual bool		event( QEvent* event );
	virtual void		wheelEvent( QWheelEvent* event );
			void		mainWindowResized( QResizeEvent* event );

	void				selectionChangedOn( const Dish& dish );

private:
	std::vector<DishView*>	dishViewsVect;
	std::vector<Dish>&		dishesVect;

	void				init();
	void				initEditable();

	void				AddDishes();
	void				StackDishViews();
};
