#include "Controller.h"

#include <windows.h>
#include <time.h>
#include <QTimer>

#include "Network/DataTransfer.h"
#include "Model/InterestCruncher.h"


User* Controller::user = NULL;


Controller::Controller(QObject *parent)
	: QThread(parent)
{
	// The user should be taken from the AD or just username
	// And the user type from the DB
	user = new User( "Andi", User::eAdmin );

	dataTransfer = new DataTransfer( this );

	connect( dataTransfer, SIGNAL( menuFinished(const Week&) ), this, SLOT( dataFinished(const Week&) ) );
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

void Controller::requestWeek( QDate startDate, QDate endDate )
{
	dataTransfer->getMenu(startDate, endDate);		// disabled until better looking images

	// Dummy data
	// sendDummyWeek( startDate, endDate );
}

void Controller::requestAllDishes()
{
	// Get all dishes from database
	// TO DO

	emit allDishesArrived( getAllDishes() );
}

void Controller::selectionChangedOn( const Dish& dish )
{
	// Store selection in database
	// TO DO
}

void Controller::publishWeek( const Week& week )
{
	// Store week in database
	// TO DO
}

void Controller::run()
{
	// Could check for notifications here

	exec();		// This executes the pending signals
}

void Controller::dataFinished( Week& week )
{
	// Compute UserInterest in another class
	// TO DO
	InterestCruncher::getInstance()->crunchUserInterest( week );

	emit weekArrived( week );
}

void Controller::sendDummyWeek(QDate startDate, QDate endDate)
{
//	Sleep( 500 );

	std::vector<Dish> dishesVect;
	dishesVect.push_back( Dish( "Ciorba de varza",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa3.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare5.png"), 2 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare1.png"), 2 ) );
	dishesVect.push_back( Dish( "Salata din gradina ursului",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/salata1.png"), 3 ) );
	dishesVect.push_back( Dish( "Salata din gradina bunicii",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/salata2.png"), 3 ) );
	dishesVect.push_back( Dish( "Supa de ceva fara ceva",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa4.png"), 1 ) );

	dishesVect[0].setNumHappies( rand() % 250 );
	dishesVect[1].setNumMeahs( rand() % 250 );
	dishesVect[2].setNumWows( rand() % 250 );
	dishesVect[3].setNumWows( rand() % 250 );
	dishesVect[4].setNumWows( rand() % 250 );
	dishesVect[5].setNumWows( rand() % 250 );
// 
// 	dishesVect[0].setUserRating( Dish::eHappy );
// 	dishesVect[1].setUserRating( Dish::eMeah );
// 	dishesVect[5].setUserRating( Dish::eHappy );

// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	std::vector<Day> daysVect;

	srand(time(0));
	randomizeRatings(dishesVect);
	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Luni", dishesVect ) );

	randomizeRatings(dishesVect);
	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Marti", dishesVect ) );

	randomizeRatings(dishesVect);
	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Miercuri", dishesVect ) );

	randomizeRatings(dishesVect);
	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Joi", dishesVect ) );

	randomizeRatings(dishesVect);
	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
// 	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
// 	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
// 	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
// 	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Vineri", dishesVect ) );

	Week week( startDate, endDate, daysVect );

	// Show only 2 after and 2 before
	QDate today = QDate::currentDate();
	if( startDate.weekNumber() - today.weekNumber() > 1 )
		week.setLastAvailable( true );
	else if( startDate.weekNumber() - today.weekNumber() < -1 )
		week.setFirstAvailable( true );

	emit dataFinished( week );
}

std::vector<Dish> Controller::getAllDishes()
{
	// Dummy data
	std::vector<Dish> dishesVect;
	dishesVect.push_back( Dish( "Ciorba de varza",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa1.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare2.png"), 2 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare3.png"), 2 ) );
	dishesVect.push_back( Dish( "Salata din gradina bunicii",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/salata3.png"), 3 ) );
	dishesVect.push_back( Dish( "Salata din gradina ursului",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/salata4.png"), 3 ) );
	dishesVect.push_back( Dish( "Supa de ceva fara ceva",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa2.png"), 1 ) );

	return dishesVect;
}

void Controller::randomizeRatings( std::vector<Dish> &dishesVect )
{
	dishesVect[0].setNumHappies( rand() % 250 );
	dishesVect[0].setNumWows( rand() % 50 );
	dishesVect[0].setNumMeahs( rand() % 100 );
	dishesVect[1].setNumHappies( rand() % 250 );
	dishesVect[1].setNumWows( rand() % 50 );
	dishesVect[1].setNumMeahs( rand() % 100 );
	dishesVect[2].setNumHappies( rand() % 250 );
	dishesVect[2].setNumWows( rand() % 50 );
	dishesVect[2].setNumMeahs( rand() % 100 );
	dishesVect[3].setNumHappies( rand() % 250 );
	dishesVect[3].setNumWows( rand() % 50 );
	dishesVect[3].setNumMeahs( rand() % 100 );
	dishesVect[4].setNumHappies( rand() % 250 );
	dishesVect[4].setNumWows( rand() % 50 );
	dishesVect[4].setNumMeahs( rand() % 100 );
	dishesVect[5].setNumHappies( rand() % 250 );
	dishesVect[5].setNumWows( rand() % 50 );
	dishesVect[5].setNumMeahs( rand() % 100 );
}