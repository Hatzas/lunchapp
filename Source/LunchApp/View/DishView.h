#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>

#include "SelectedEffect.h"
#include "../Model/Dish.h"


class DishView : public QWidget
{
	Q_OBJECT

public:
						DishView( QWidget *parent, const Dish& dish );
						~DishView();

			void		setDisabled( bool disabled );
			bool		getDisabled()						{ return disabled; }
protected:
	virtual void		wheelEvent( QWheelEvent* event );
	
	virtual void		enterEvent( QEvent* event );
	virtual void		leaveEvent( QEvent* event );

	virtual void		mouseMoveEvent( QMouseEvent* event );
	virtual void		mousePressEvent( QMouseEvent* event );
	virtual void		mouseReleaseEvent( QMouseEvent* event );

private:
			Dish						dish;
			bool						disabled;			// true when a user selection excludes other dishes (like from the same course)

			QLabel*						imageLabel;
			QLabel*						ribbonLabel;
			QLabel*						detailsLabel;

			QPropertyAnimation*			detailsAnimation;

			SelectedEffect*				selectedEffect;

			bool						mousePressed;

			void		init();
			QPixmap		GetRibbonByCourse( int courseNum );
};
