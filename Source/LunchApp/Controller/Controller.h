#pragma once

#include <QObject>
#include <QDateTime>

#include "../Model/Dish.h"
#include "../Model/Week.h"


class Controller : public QObject
{
	Q_OBJECT

public:
				Controller(QObject *parent);
				~Controller();

signals:
	void		weekArrived( const Week& week );

public slots:
	void		requestWeekBefore( const Week& week );
	void		requestWeekAfter( const Week& week );

	void		selectionChangedOn( const Dish& dish );

private:
	void		requestWeek( QDateTime startDate, QDateTime endDate );
};
