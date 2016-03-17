#pragma once

#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>

#include "WeekView.h"
#include "InfiniteBackground.h"


class MetroView : public QGraphicsView
{
	Q_OBJECT

public:
					MetroView( QWidget *parent );
					~MetroView();

	virtual void	wheelEvent( QWheelEvent* event );

protected:
	virtual void	resizeEvent( QResizeEvent * event );

private:
	QGraphicsScene*				scene;

	InfiniteBackground*			background;
	QLabel*						weekLabel;
	WeekView*					currentWeekView;

	QPropertyAnimation*			weekAnimation;
	QPropertyAnimation*			backgroundAnimation;

	QSequentialAnimationGroup*	weekAnimationsQueue;
	QParallelAnimationGroup*	animations;

	void			init();
	void			addSceneItems();
};