#include "Controller.h"

#include <windows.h>


Controller::Controller(QObject *parent)
	: QObject(parent)
{

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
	// Make request to database
	// TO DO

	// Dummy data
	Sleep( 1000 );

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

	Week week( "1 - 6 Martie", daysVect );

	emit weekArrived( week );
}