#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>

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
	virtual void	mouseMoveEvent( QMouseEvent* mouseEvent );

private:
	QPixmap					dishPixmap;

	QLabel*					imageLabel;
	QLabel*					detailsLabel;

	QPropertyAnimation*		detailsAnimation;

	void			init();
};
