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

			void			init();

	virtual void			wheelEvent( QWheelEvent* event );

signals:
			void			requestWeekBefore( const Week& week );
			void			requestWeekAfter( const Week& week );
			void			requestWeek( QDate startDate, QDate endDate );

			void			requestAllDishes();

			void			selectionChangedOn( const Dish& dish );

			void			publishWeek( const Week& week );

			void			uploadPicture( QPixmap pixmap );

public slots:
			void			weekArrived( const Week& week );
			void			allDishesArrived( Day allDishesVect );

			void			weekAnimationFinished();

			void			weekDatePressed( bool checked );			
			void			dateSelected();

			void			publishPressed( bool );
			void			uploadImagePressed( bool );

protected:
	virtual void			resizeEvent( QResizeEvent * event );

private:
	QGraphicsScene*				scene;

	bool						adminMode;

	InfiniteBackground*			background;
	QLabel*						weekPrefixLabel;
	QPushButton*				weekDateButton;
	AllWeeksView*				weeksView;
	DayView*					allDishesView;
	QLabel*						userLabel;
	QPushButton*				administrateButton;
	QPushButton*				publishButton;
	QPushButton*				changeBackgroundButton;
	QPushButton*				photoButton;

	QCalendarWidget*			calendar;

	QPropertyAnimation*			weekDateOutAnimation;
	QPropertyAnimation*			weekDateInAnimation;
	QPropertyAnimation*			calendarFadeAnimation;
	QPropertyAnimation*			weekMoveAnimation;
	QPropertyAnimation*			backgroundAnimation;

	QParallelAnimationGroup*	parallelAnimations;

	void					addSceneItems();

	void					setWeekDateText( const Week &currentWeek );

	void					alignControls();

	QPixmap					takePicture( bool fromCamera );

	void					addCameraWidget();
};