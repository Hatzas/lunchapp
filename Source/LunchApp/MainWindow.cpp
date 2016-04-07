#include "MainWindow.h"

#include <algorithm>
#include <QMenu>
#include <QCloseEvent>

#include "Style.h"
#include "View/NotificationWindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	// Style
	Style::init();

	// Size
	this->setMinimumSize( Style::getWeekWidth(), Style::getWindowHeight() );
	this->adjustSize();

	// Controller and connections
	setupController();

	// UI
	ui.setupUi(this);
	metroView = new MetroView( parent );
	ui.verticalLayout->addWidget( metroView );

	this->resize( this->minimumSize() );

	// Tray
	setupTray();
	showTrayMessage( "Sunt si eu aici" );
	
	// Connections
	makeConnections();

	// Dummy data
	sendDummyWeek();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showTrayMessage( const QString& msg )
{
	NotificationWindow* customWindow = new NotificationWindow( msg );
	customWindow->show();
}

void MainWindow::resizeEvent( QResizeEvent * event )
{
}

void MainWindow::onTrayActivation(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Context)
		return;

	show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	hide();
	event->ignore();

	showTrayMessage(tr("Aplicatia nu e moarta, se transforma (in tray)"));
}

void MainWindow::sendDummyWeek()
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

	// Identifiers
	dishesVect[0].setIdentifier( "C1" );
	dishesVect[1].setIdentifier( "M1" );
	dishesVect[2].setIdentifier( "M2" );
	dishesVect[3].setIdentifier( "S1" );
	dishesVect[4].setIdentifier( "S2" );
	dishesVect[5].setIdentifier( "C2" );

	dishesVect[0].setNumWows( 120 );
	dishesVect[0].setUserRating( Dish::eWow );
	dishesVect[0].setUserSelected( true );
	dishesVect[1].setNumWows( 120 );
	dishesVect[2].setNumWows( 120 );
	dishesVect[2].setUserSelected( true );
	dishesVect[3].setNumWows( 120 );
	dishesVect[4].setNumWows( 120 );
	dishesVect[5].setNumWows( 120 );

	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	std::vector<Day> daysVect;

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Luni", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Marti", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Miercuri", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Joi", dishesVect ) );

	std::random_shuffle( dishesVect.begin(), dishesVect.end() );
	dishesVect[0].setUserInterest( Dish::EUserInterest( rand() % 4 - 1 ) );
	dishesVect[1].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[2].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[3].setUserInterest( Dish::EUserInterest( rand() % 4 ) );
	dishesVect[4].setUserInterest( Dish::EUserInterest( rand() % 4 - 1) );
	dishesVect[5].setUserInterest( Dish::EUserInterest( rand() % 4 ) );

	daysVect.push_back( Day( "Vineri", dishesVect ) );

	QDate monday = QDate::currentDate();
	QDate friday = monday.addDays( 4 );

	int startDay = monday.day();
	int endDay = friday.day();

	Week week( monday, friday, daysVect );

	metroView->weekArrived( week );
}

void MainWindow::setupTray()
{
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon( QPixmap( kAppIconPath ) );
	trayIcon->setToolTip( tr( "Lunch App\nApasa-l" ) );

	NotificationWindow::setup( trayIcon );

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivation(QSystemTrayIcon::ActivationReason)));

	trayIconMenu = new QMenu(this);
	
	QAction* quitAction = new QAction(tr("Inchide de tot"), trayIconMenu);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	trayIconMenu->addAction(quitAction);
	
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->show();
}

void MainWindow::setupController()
{
	controller = new Controller();
	controller->moveToThread( controller );
	controller->start();
}

void MainWindow::makeConnections()
{
	connect( controller, SIGNAL( weekArrived( const Week& ) ), metroView, SLOT( weekArrived( const Week& ) ), Qt::QueuedConnection );

	connect( metroView, SIGNAL( requestWeekBefore( const Week& ) ), controller, SLOT( requestWeekBefore( const Week& ) ), Qt::QueuedConnection );
	connect( metroView, SIGNAL( requestWeekAfter( const Week& ) ), controller, SLOT( requestWeekAfter( const Week& ) ), Qt::QueuedConnection );
	connect( metroView, SIGNAL( selectionChangedOn( const Dish& ) ), controller, SLOT( selectionChangedOn( const Dish& ) ), Qt::QueuedConnection );
}
