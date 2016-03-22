#include "Controller.h"

#include <windows.h>
#include "Network/DataTransfer.h"

Controller::Controller(QObject *parent)
	: QThread(parent)
{
	dataTransfer = new DataTransfer( this );
	dataTransfer->getMenu( QDate(), QDate() );
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
	// Make request to database
	// TO DO

	// Dummy data
	Sleep( 2000 );

	std::vector<Dish> dishesVect;
	dishesVect.push_back( Dish( "Quesadilla cu pui",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa2.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa3.png"), 1 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/salata2.png"), 2 ) );

	std::vector<Day> daysVect;

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	daysVect.push_back( Day( "Luni", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	daysVect.push_back( Day( "Marti", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	daysVect.push_back( Day( "Miercuri", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	daysVect.push_back( Day( "Joi", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	daysVect.push_back( Day( "Vineri", dishesVect ) );

	Week week( startDate, endDate, daysVect );

	emit weekArrived( week );
}