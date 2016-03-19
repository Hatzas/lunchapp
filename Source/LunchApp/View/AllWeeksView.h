#pragma once

#include <QWidget>

#include "WeekView.h"


class AllWeeksView : public QWidget
{
	Q_OBJECT

public:
							AllWeeksView(QWidget *parent);
							~AllWeeksView();

	virtual void			wheelEvent( QWheelEvent* event );

			void			addWeek( const Week& week );
protected:		
	virtual void			moveEvent( QMoveEvent* event );

private:
	std::vector<WeekView*>		weekViewsVect;

	bool						requestSent;
};
