#include "Controller.h"

#include <windows.h>
#include "Network/DataTransfer.h"

Controller::Controller(QObject *parent)
	: QThread(parent)
{
	dataTransfer = new DataTransfer( this );
	dataTransfer->getMenu( QDate(2016, 3, 21), QDate(2016, 3, 25) );

	connect(dataTransfer, SIGNAL(menuFinished(const Week&)), this, SLOT(dataFinished(const Week&)));
}

Controller::~Controller()
{
}

void Controller::requestWeekAfter( const Week& week )
{
	// Compute DateTime after week endDate
	QDate startDate = week.getStartDate().addDays( 7 );					// ! This will not work is the week does not start with Monday !
	QDate endDate = startDate.addDays( 4 );								// ! Use dayOfWeek() to rectify !/

	int startDay = startDate.day();
	int endDay = endDate.day();

	requestWeek( startDate, endDate );
}

void Controller::requestWeekBefore( const Week& week )
{
	// Compute DateTime before week startDate
	QDate startDate = week.getStartDate().addDays( -7 );				// ! This will not work is the week does not start with Monday !
	QDate endDate = startDate.addDays( 4 );								// ! Use dayOfWeek() to rectify !/

	int startDay = startDate.day();
	int endDay = endDate.day();

	requestWeek( startDate, endDate );
}

void Controller::selectionChangedOn( const Dish& dish )
{
	// Store selection in database
	// TO DO
}

void Controller::run()
{
	// Could check for notifications here

	exec();		// This executes the pending signals
}

void Controller::requestWeek( QDate startDate, QDate endDate )
{
	dataTransfer->getMenu(startDate, endDate);
}

void Controller::dataFinished(const Week& week)
{
	emit weekArrived( week );
}