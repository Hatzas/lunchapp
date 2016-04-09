#pragma once

#include <QThread>
#include <QDate>

#include "../Model/Dish.h"
#include "../Model/Week.h"
#include "../Model/User.h"

class DataTransfer;

class Controller : public QThread
{
	Q_OBJECT

public:
						Controller( QObject *parent = 0 );
						~Controller();

	static	User*		getUser()							{ return user; }

			void		sendDummyWeek( QDate startDate, QDate endDate );

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
	static	User*			user;

			DataTransfer*	dataTransfer;
};
