#pragma once

#include <QThread>
#include <QDate>

#include "../Model/Dish.h"
#include "../Model/Week.h"


class DataTransfer;

class Controller : public QThread
{
	Q_OBJECT

public:
				Controller( QObject *parent = 0 );
				~Controller();

signals:
	void		weekArrived( const Week& week );

public slots:
	void		requestWeekBefore( const Week& week );
	void		requestWeekAfter( const Week& week );

	void		selectionChangedOn( const Dish& dish );

protected:
	void		run();

private:
	void		requestWeek( QDate startDate, QDate endDate );

private slots:
	void		dataFinished(const Week& week);

private:
	DataTransfer*	dataTransfer;
};
