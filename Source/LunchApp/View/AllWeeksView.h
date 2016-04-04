#pragma once

#include <QWidget>
#include <QAtomicInt>
#include <QSequentialAnimationGroup>

#include "WeekView.h"


class AllWeeksView : public QWidget
{
	Q_OBJECT

public:
	enum EDirection
	{
		eToRightDirection,
		eToLeftDirection
	};
							AllWeeksView(QWidget *parent);
							~AllWeeksView();

			void			init();

	virtual void			wheelEvent( QWheelEvent* event );
			void			mainWindowResized( QResizeEvent* event );

			void			selectionChangedOn( const Dish& dish );
			bool			scrollStarted( EDirection direction );		// Returns true if there are items to show after scroll

			void			addWeek( const Week& week );
			Week			getVisibleWeek();

private:
	std::vector<WeekView*>		weekViewsVect;
	QLabel*						loadingLabel;

	int							windowWidth;

	QSequentialAnimationGroup*	firstLastAnimations;
	QPropertyAnimation*			forwardAnimation;
	QPropertyAnimation*			backAnimation;

			void			increaseSize( EDirection direction );
			void			centerWeekViews();

			void			showLoadingAnim( bool show, EDirection direction = eToRightDirection );

};
