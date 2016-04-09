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
							MetroView( QWidget *parent, bool adminMode = false );
							~MetroView();

	virtual void			wheelEvent( QWheelEvent* event );

signals:
			void				requestWeekBefore( const Week& week );
			void				requestWeekAfter( const Week& week );
			void				requestAllDishes();

			void				selectionChangedOn( const Dish& dish );

			void				publishWeek( const Week& week );

public slots:
			void			weekArrived( const Week& week );
			void			allDishesArrived( std::vector<Dish>& allDishesVect );

			void			weekAnimationFinished();

			void			weekDatePressed( bool checked );			
			void			dateSelected();

			void			publishPressed( bool );

protected:
	virtual void			resizeEvent( QResizeEvent * event );

private:
	QGraphicsScene*				scene;

	bool						adminMode;

	InfiniteBackground*			background;
	QPushButton*				weekDateButton;
	AllWeeksView*				weeksView;
	DayView*					allDishesView;
	QLabel*						userLabel;
	QPushButton*				administrateButton;
	QPushButton*				publishButton;

	QCalendarWidget*			calendar;

	QPropertyAnimation*			weekDateOutAnimation;
	QPropertyAnimation*			weekDateInAnimation;
	QPropertyAnimation*			calendarFadeAnimation;
	QPropertyAnimation*			weekMoveAnimation;
	QPropertyAnimation*			backgroundAnimation;

	QParallelAnimationGroup*	animations;

	void					init();
	void					addSceneItems();

	void					setWeekDateText( const Week &currentWeek );

	void					alignButtons();
};