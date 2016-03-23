#include "Controller.h"

#include <windows.h>
#include "Network/DataTransfer.h"

Controller::Controller(QObject *parent)
	: QObject(parent)
{
	dataTransfer = new DataTransfer(this);
	dataTransfer->getMenu(QDateTime(), QDateTime());
	connect(dataTransfer, SIGNAL(menuFinished(const Week&)), this, SLOT(dataFinished(const Week&)));
}

Controller::~Controller()
{

}

void Controller::requestWeekAfter( const Week& week )
{
	// Compute DateTime after week endDate
	QDateTime startDate;
	QDateTime endDate;
	// TO DO

	requestWeek( startDate, endDate );
}

void Controller::requestWeekBefore( const Week& week )
{
	// Compute DateTime before week startDate
	QDateTime startDate;
	QDateTime endDate;
	// TO DO

	requestWeek( startDate, endDate );
}

void Controller::selectionChangedOn( const Dish& dish )
{
	// Store selection in database
	// TO DO
}

void Controller::requestWeek( QDateTime startDate, QDateTime endDate )
{
	dataTransfer->getMenu(startDate, endDate);
}

void Controller::dataFinished(const Week& week)
{
	emit weekArrived(week);
}
