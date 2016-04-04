#pragma once

#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QCalendarWidget>
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
			void	weekAnimationFinished();
			void	weekDatePressed( bool checked );
			void	dateSelected();

protected:
	virtual void	resizeEvent( QResizeEvent * event );

private:
	QGraphicsScene*				scene;

	InfiniteBackground*			background;
	QPushButton*				weekDateButton;
	AllWeeksView*				weeksView;
	QLabel*						userLabel;

	QCalendarWidget*			calendar;

	QPropertyAnimation*			weekDateOutAnimation;
	QPropertyAnimation*			weekDateInAnimation;
	QPropertyAnimation*			calendarFadeAnimation;
	QPropertyAnimation*			weekMoveAnimation;
	QPropertyAnimation*			backgroundAnimation;

	QParallelAnimationGroup*	animations;

	void			init();
	void			addSceneItems();
	void			setWeekDateText( const Week &currentWeek );
};