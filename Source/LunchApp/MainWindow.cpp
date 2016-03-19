#include "MainWindow.h"

#include <algorithm>
#include <QSystemTrayIcon>


MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	// UI
	ui.setupUi(this);
	metroView = new MetroView( parent );
	ui.verticalLayout->addWidget( metroView );

	// Tray
	setupTray();
	showTrayMessage( "Baga meniul" );

	// Controller and connections
	setupController();

	// Dummy data
	sendWeek();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showTrayMessage( const QString& msg )
{
	trayIcon->showMessage( tr("Lunch App"), msg, QSystemTrayIcon::Information, 20000 );
}

void MainWindow::sendWeek()
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
	trayIcon = new QSystemTrayIcon( this );
	trayIcon->setIcon( QIcon("Resources/like.png") );
	trayIcon->setToolTip( tr("Lunch App\nApasa-l") );
	trayIcon->show();
}

void MainWindow::setupController()
{
	controller = new Controller( this );

	//connect( metroView, SIGNAL( SelectionChangedOn( const Dish& ) ), controller, SLOT( SelectionChangedOn( const Dish& ) ) );
}
