#include "MainWindow.h"

#include <algorithm>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	metroView = new MetroView( parent );
	ui.verticalLayout->addWidget( metroView );

	trayIcon = new QSystemTrayIcon( this );

	setupTray();
	showTrayMessage( "Baga meniul" );

	SendWeek();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showTrayMessage( const QString& msg )
{
	trayIcon->showMessage( tr("Lunch App"), msg, QSystemTrayIcon::Information, 20000 );
}

void MainWindow::SendWeek()
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
	trayIcon->setIcon( QIcon("Resources/like.png") );
	trayIcon->setToolTip( tr("Lunch App\nApasa-l") );
	trayIcon->show();
}
