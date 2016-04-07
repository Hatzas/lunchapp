#pragma once

#include <QWidget>

#include "DishView.h"


class DayDishesView : public QWidget
{
	Q_OBJECT
	Q_PROPERTY( QPoint contentOffset READ getContentOffset WRITE setContentOffset NOTIFY contentOffsetChanged )

public:
						DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect );
						~DayDishesView();

	virtual bool		event( QEvent* event );
	virtual void		wheelEvent( QWheelEvent* wheelEvent );
			void		mainWindowResized( QResizeEvent* event );

	void				selectionChangedOn( const Dish& dish );

	void				setContentOffset( QPoint offset );
	QPoint				getContentOffset() const				{ return internalContentOffset; }

signals:
	void				contentOffsetChanged();

private:
	std::vector<DishView*>	dishViewsVect;
	std::vector<Dish>&		dishesVect;

	QPoint					internalContentOffset;

	QPropertyAnimation*		scrollAnimation;

	void				init();
	void				initEditable();

	void				AddDishes();
	void				StackDishViews();
};
