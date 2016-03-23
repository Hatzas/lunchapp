#pragma once

#include <QWidget>
#include <QAtomicInt>

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

			void			addWeek( const Week& week );

			bool			scrollStarted( EDirection direction );		// Returns true if there are items to show after scroll

private:
	std::vector<WeekView*>		weekViewsVect;
	QLabel*						loadingLabel;

	int							screenWidth;

			void			increaseSize( EDirection direction );
			void			centerWeekViews();

			void			showLoadingAnim( bool show, EDirection direction = eToRightDirection );

};
