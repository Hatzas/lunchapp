#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>


#include "SelectedEffect.h"
#include "../Model/Dish.h"


class DishView : public QWidget
{
	Q_OBJECT

public:
						DishView( QWidget *parent, const Dish& dish );
						~DishView();

protected:
	virtual void		wheelEvent( QWheelEvent* event );
	
	virtual void		enterEvent( QEvent* event );
	virtual void		leaveEvent( QEvent* event );

	virtual void		mouseMoveEvent( QMouseEvent* event );
	virtual void		mousePressEvent( QMouseEvent* event );
	virtual void		mouseReleaseEvent( QMouseEvent* event );

private:
			Dish					dish;

			QLabel*					imageLabel;
			QLabel*					ribbonLabel;
			QLabel*					detailsLabel;

			QPropertyAnimation*		detailsAnimation;
			SelectedEffect*			selectedEffect;

			bool					mousePressed;

			void		init();
			QPixmap		GetRibbonByCourse( int courseNum );
};
