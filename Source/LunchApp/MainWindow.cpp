#include "MainWindow.h"

#include <algorithm>
#include "Network/DataTransfer.h"
#include <QMenu>


MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	// UI
	ui.setupUi(this);
	metroView = new MetroView( parent );
	ui.verticalLayout->addWidget( metroView );

<<<<<<< HEAD:Source/LunchApp/View/MainWindow.cpp
	

	//setupTray();
	//showTrayMessage( "Baga meniul" );

	SendWeek();

	DataTransfer* dt = new DataTransfer(this);
	dt->getMenu();
=======
	// Tray
	setupTray();
	showTrayMessage( "Baga meniul" );

	// Controller and connections
	setupController();

	// Dummy data
	sendWeek();
>>>>>>> GUI:Source/LunchApp/MainWindow.cpp
}

MainWindow::~MainWindow()
{
}

void MainWindow::showTrayMessage( const QString& msg )
{
	trayIcon->showMessage( tr("Lunch App"), msg, QSystemTrayIcon::Information, 20000 );
}

<<<<<<< HEAD:Source/LunchApp/View/MainWindow.cpp
void MainWindow::onTrayActivation(QSystemTrayIcon::ActivationReason reason)
{
	
}

void MainWindow::SendWeek()
=======
void MainWindow::sendWeek()
>>>>>>> GUI:Source/LunchApp/MainWindow.cpp
{
	// Dummy data
	std::vector<Dish> dishesVect;
	dishesVect.push_back( Dish( "Quesadilla cu pui",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/supa1.png"), 1 ) );
	dishesVect.push_back( Dish( "Aripioare de pui cu crusta de porumb",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare2.png"), 2 ) );
	dishesVect.push_back( Dish( "Pastrav pane cu spanac",
		"tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare",
		QPixmap("Resources/mancare3.png"), 2 ) );

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

	metroView->weekArrived( week );
}

void MainWindow::setupTray()
{
<<<<<<< HEAD:Source/LunchApp/View/MainWindow.cpp
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon("Resources/like.png"));
	trayIcon->setToolTip(tr("Lunch App\nApasa-l"));

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivation(QSystemTrayIcon::ActivationReason)));

	trayIconMenu = new QMenu(this);
	trayIcon->setContextMenu(trayIconMenu);
=======
	trayIcon = new QSystemTrayIcon( this );
	trayIcon->setIcon( QIcon("Resources/like.png") );
	trayIcon->setToolTip( tr("Lunch App\nApasa-l") );
>>>>>>> GUI:Source/LunchApp/MainWindow.cpp
	trayIcon->show();
}

void MainWindow::setupController()
{
	controller = new Controller( this );

	connect( controller, SIGNAL( weekArrived( const Week& ) ), metroView, SLOT( weekArrived( const Week& ) )/*, Qt::QueuedConnection*/ );
	
	connect( metroView, SIGNAL( requestWeekBefore( const Week& ) ), controller, SLOT( requestWeekBefore( const Week& ) )/*, Qt::QueuedConnection*/ );
	connect( metroView, SIGNAL( requestWeekAfter( const Week& ) ), controller, SLOT( requestWeekAfter( const Week& ) )/*, Qt::QueuedConnection*/ );
	connect( metroView, SIGNAL( selectionChangedOn( const Dish& ) ), controller, SLOT( selectionChangedOn( const Dish& ) )/*, Qt::QueuedConnection*/ );
}
