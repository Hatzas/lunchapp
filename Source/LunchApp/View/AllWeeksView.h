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
		eHere,
		eRightDirection,
		eLeftDirection
	};
							AllWeeksView( QWidget *parent, bool editMode = false );
							~AllWeeksView();

			void			init();

			void			mainWindowResized( QSize size );

			void			selectionChangedOn( const Dish& dish );
			bool			scrollStarted( EDirection direction );		// Returns true if there are items to show after scroll

			void			addWeek( const Week& week );
			Week			getVisibleWeek();

			int				getWeeksNum()								{ return weekViewsVect.size(); }
			Week			getWeek( int index );

protected:
		virtual	bool		event( QEvent *event );
		virtual void		wheelEvent( QWheelEvent* event );

private:
	bool						editMode;
	std::vector< Week >			weeksVect;

	std::vector<WeekView*>		weekViewsVect;
	QLabel*						loadingLabel;

	int							windowWidth;

	QSequentialAnimationGroup*	firstLastAnimations;
	QPropertyAnimation*			forwardAnimation;
	QPropertyAnimation*			backAnimation;

			void			addEmptyWeek();

			void			increaseSize( EDirection direction );
			void			centerWeekViews();

			void			showLoadingAnim( bool show, EDirection direction = eRightDirection );
};
