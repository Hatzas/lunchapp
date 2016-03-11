#pragma once

#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>

#include "WeekView.h"


class MetroView : public QGraphicsView
{
	Q_OBJECT

public:
					MetroView( QWidget *parent );
					~MetroView();

	virtual void	wheelEvent( QWheelEvent* event );

private:
	QGraphicsScene*				scene;

	QLabel*						weekLabel;
	WeekView*					currentWeekView;

	QPropertyAnimation*			weekAnimation;

	QSequentialAnimationGroup*	weekAnimationsQueue;
	QParallelAnimationGroup*	animations;

	void			init();
	void			addSceneItems();
};