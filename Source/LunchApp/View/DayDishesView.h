#pragma once

#include <QWidget>
#include <QDropEvent>

#include "CommonStructs.h"
#include "DishView.h"


class DayDishesView : public QWidget
{
	Q_OBJECT
	Q_PROPERTY( QPoint contentOffset READ getContentOffset WRITE setContentOffset NOTIFY contentOffsetChanged )

public:
						DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect, EMode mode = eNormalMode );
						~DayDishesView();

			void			mainWindowResized( QSize size );

			void			selectionChangedOn( const Dish& dish );

			void			setContentOffset( QPoint offset );
			QPoint			getContentOffset() const				{ return internalContentOffset; }

signals:
			void			contentOffsetChanged();

protected:
	virtual bool			event( QEvent* event );
	virtual void			wheelEvent( QWheelEvent* wheelEvent );

	virtual void			mousePressEvent( QMouseEvent* event );

	virtual void			dragEnterEvent( QDragEnterEvent* event );
	virtual void			dragLeaveEvent( QDragLeaveEvent* event );
	virtual void			dropEvent( QDropEvent* event );

	virtual void			resizeEvent( QResizeEvent* event );

private:
	std::vector<DishView*>			dishViewsVect;
	std::vector<Dish>&				dishesVect;

			EMode					mode;
			QLabel*					editBackgroundLabel;

			QPoint					internalContentOffset;

			QPropertyAnimation*		scrollAnimation;

			void			init();
			void			initEditable();

			void			addDish( const Dish& dish );
			void			addDishes();

			void			stackDishViews();
};
