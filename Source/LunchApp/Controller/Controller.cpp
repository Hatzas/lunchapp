#include "Controller.h"

#include <time.h>
#include <QTimer>

#if WIN32
#   include <windows.h>
#	define SECURITY_WIN32
#	include <Security.h>
#	pragma comment( lib, "Secur32.lib" )
#endif

#include "View/Style.h"
#include "Network/DataTransfer.h"
#include "Model/InterestCruncher.h"


User* Controller::user = NULL;
std::map<QString, int> Controller::weekDays;


Controller::Controller(QObject *parent)
	: QThread(parent)
{
	// If these names change in the database, all hell will brake loose, because of a design flaw
	weekDays["lu"] = 1;
	weekDays["ma"] = 2;
	weekDays["mi"] = 3;
	weekDays["jo"] = 4;
	weekDays["vi"] = 5;
	weekDays["sa"] = 6;
	weekDays["du"] = 7;

	// The user should be taken from the AD or just username
	// And the user type from the DB
	QString userName = "Andi";

#if WIN32
	wchar_t username[256];
	DWORD username_len = 256;
	::GetUserNameEx( ::NameDisplay, username, &username_len);

	userName = QString::fromWCharArray( username );
	userName = userName.left( userName.indexOf( " " ) );
	userName = userName.left( userName.indexOf( "-" ) );
#endif

	user = new User( userName, User::eAdmin );

	dataTransfer = new DataTransfer( this );

	connect( dataTransfer, SIGNAL( menuFinished( Week& ) ), this, SLOT( dataFinished( Week& ) ) );
}

Controller::~Controller()
{
}

void Controller::requestWeekAfter( const Week& week )
{
	// Compute DateTime after week endDate
	QDate startDate = week.getStartDate().addDays( 7 );					// ! This will not work is the week does not start with Monday !
	QDate endDate = startDate.addDays( 4 );								// ! Use dayOfWeek() to rectify !/

	requestWeek( startDate, endDate );
}

void Controller::requestWeekBefore( const Week& week )
{
	// Compute DateTime before week startDate
	QDate startDate = week.getStartDate().addDays( -7 );				// ! This will not work is the week does not start with Monday !
	QDate endDate = startDate.addDays( 4 );								// ! Use dayOfWeek() to rectify !/

	requestWeek( startDate, endDate );
}

void Controller::requestWeek( QDate startDate, QDate endDate )
{
	dataTransfer->getUserMenu(startDate, endDate);

	// Dummy data
	// sendDummyWeek( startDate, endDate );
}

void Controller::requestAllDishes()
{
	// Get all dishes from database
	// TO DO

	emit allDishesArrived( Day( "Toate", getAllDishes() ) );
}

void Controller::selectionChangedOn( const Dish& /* dish */ )
{
	// Store selection in database
	// TO DO
}

void Controller::publishWeek( const Week& /* week */ )
{
	// Store week in database
	// TO DO
}

void Controller::uploadPicture( QPixmap /* pixmap */ )
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
	// Sort week days
	std::sort( week.getDays().begin(), week.getDays().end(), compareDays );

	// Compute UserInterest
	InterestCruncher::getInstance()->crunchUserInterest( week );

	emit weekArrived( week );
}

bool Controller::compareDays( Day first, Day second )
{
	if( weekDays[first.getName().toLower().left(2)] < weekDays[second.getName().toLower().left(2)] )
		return true;
	else
		return false;
}

void Controller::sendDummyWeek(QDate startDate, QDate endDate)
{
//	Sleep( 500 );

	std::vector<Dish> dishesVect;
	dishesVect.push_back( Dish( "Ciorba de varza",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"supa3.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"mancare5.png"), 2 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"mancare1.png"), 2 ) );
	dishesVect.push_back( Dish( "Salata din gradina ursului",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"salata1.png"), 3 ) );
	dishesVect.push_back( Dish( "Salata din gradina bunicii",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"salata2.png"), 3 ) );
	dishesVect.push_back( Dish( "Supa de ceva fara ceva",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"supa4.png"), 1 ) );

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
		QPixmap(RESOURCES_ROOT"supa1.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"mancare2.png"), 2 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"mancare3.png"), 2 ) );
	dishesVect.push_back( Dish( "Salata din gradina bunicii",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"salata3.png"), 3 ) );
	dishesVect.push_back( Dish( "Salata din gradina ursului",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"salata4.png"), 3 ) );
	dishesVect.push_back( Dish( "Supa de ceva fara ceva",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap(RESOURCES_ROOT"supa2.png"), 1 ) );

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
