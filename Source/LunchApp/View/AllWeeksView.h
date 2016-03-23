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
	virtual void			resizeEvent( QResizeEvent* event );

			void			selectionChangedOn( const Dish& dish );

			void			addWeek( const Week& week );

			bool			scrollStarted( EDirection direction );		// Returns true if there are items to show after scroll

private:
	std::vector<WeekView*>		weekViewsVect;
	QLabel*						loadingLabel;

			void			increaseSize( EDirection direction );

			void			showLoadingAnim( bool show, EDirection direction = eToRightDirection );

};
