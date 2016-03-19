#pragma once

#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>

#include "AllWeeksView.h"
#include "InfiniteBackground.h"


class MetroView : public QGraphicsView
{
	Q_OBJECT

public:
					MetroView( QWidget *parent );
					~MetroView();

	virtual void	wheelEvent( QWheelEvent* event );

signals:
		void		requestWeekBefore( const Week& week );
		void		requestWeekAfter( const Week& week );

		void		selectionChangedOn( const Dish& dish );

public slots:
			void	weekArrived( const Week& week );

protected:
	virtual void	resizeEvent( QResizeEvent * event );

private:
	QGraphicsScene*				scene;

	InfiniteBackground*			background;
	QLabel*						weekLabel;
	AllWeeksView*				weeksView;

	QPropertyAnimation*			weekMoveAnimation;
	QPropertyAnimation*			backgroundAnimation;

	QSequentialAnimationGroup*	weekAnimationsQueue;
	QParallelAnimationGroup*	animations;

	void			init();
	void			addSceneItems();
};