#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>


#include "SelectedEffect.h"


class DishView : public QWidget
{
	Q_OBJECT

public:
					DishView( QWidget *parent, QPixmap pixmap );
					~DishView();

protected:
	virtual void	wheelEvent( QWheelEvent* event );
	
	virtual void	enterEvent( QEvent* event );
	virtual void	leaveEvent( QEvent* event );

	virtual void	mouseMoveEvent( QMouseEvent* event );
	virtual void	mousePressEvent( QMouseEvent* event );
	virtual void	mouseReleaseEvent( QMouseEvent* event );

private:
	bool						dishSelected;

	QPixmap						dishPixmap;
	QLabel*						imageLabel;
	QLabel*						detailsLabel;

	QPropertyAnimation*			detailsAnimation;
	SelectedEffect*				selectedEffect;

	bool						mousePressed;

	void			init();
};
