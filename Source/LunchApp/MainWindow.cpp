#include "MainWindow.h"

#include <algorithm>
#include <QMenu>
#include <QCloseEvent>
#ifdef Q_OS_ANDROID
#   include<QtAndroidExtras>
#endif

#include "View/Style.h"
#include "View/NotificationWindow.h"


MainWindow::MainWindow( QWidget *parent )
	: QWidget( parent )
	, adminMetroView( NULL )
	, adminMode( false )
{
	// Style
	Style::init();

	// Controller and connections
	setupController();

	// UI
	ui.setupUi(this);
	regularMetroView = new MetroView( this );
	ui.verticalLayout->addWidget( regularMetroView );

	if( Controller::getUser()->getRole() == User::eAdmin )
	{
		adminMetroView = new MetroView( this, true );
		adminMetroView->hide();
	}

	// Connections
	makeConnections();

	// Tray
	setupTray();
//	showNotification( "Sunt si eu aici" );

	// Init members
	regularMetroView->init();
	adminMetroView->init();

	// Size
	this->setMinimumSize( regularMetroView->minimumSize() );
	this->adjustSize();

	//controller->sendDummyWeek( QDate(2016, 3, 21), QDate(2016, 3, 25) );

	// Start hidden for presentation
	this->showMinimized();
}

MainWindow::~MainWindow()
{
}

void MainWindow::switchAdministrate( bool )
{
	adminMode = !adminMode;

	if( adminMode )
	{
		regularMetroView->hide();
		ui.verticalLayout->removeWidget( regularMetroView );

		adminMetroView->show();
		ui.verticalLayout->addWidget( adminMetroView );

 		if( this->width() < adminMetroView->minimumWidth() )
		{
			this->setMinimumWidth( adminMetroView->minimumWidth() );
			this->adjustSize();
		}	
	}
	else
	{
		adminMetroView->hide();
		ui.verticalLayout->removeWidget( adminMetroView );

		regularMetroView->show();
		ui.verticalLayout->addWidget( regularMetroView );

 		if( this->width() < regularMetroView->minimumWidth() )
		{
 			this->setMinimumWidth( regularMetroView->minimumWidth() );
			this->adjustSize();
		}
	}

	
}

void MainWindow::showNotification( const QString& msg )
{
#ifdef Q_OS_ANDROID

    QAndroidJniObject javaNotification = QAndroidJniObject::fromString( msg );
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/LunchApp/LunchApp",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());

#else

	NotificationWindow* customWindow = new NotificationWindow( this, msg );
	customWindow->show();

#endif
}

void MainWindow::showNotification( const QString& msg, const QPixmap& pixmap )
{
#ifdef Q_OS_ANDROID

	// Call method and pass icon
	// TO DO

	QAndroidJniObject javaNotification = QAndroidJniObject::fromString( msg );
	QAndroidJniObject::callStaticMethod<void>("org/qtproject/LunchApp/LunchApp",
		"notify",
		"(Ljava/lang/String;)V",
		javaNotification.object<jstring>());

#else

	std::vector<QPixmap> dishesPixmaps;
	dishesPixmaps.clear();
	dishesPixmaps.push_back( pixmap );

	NotificationWindow* customWindow = new NotificationWindow( this, msg, dishesPixmaps, kMenuNotificationShowTime );
	customWindow->show();

#endif
}

void MainWindow::resizeEvent( QResizeEvent* /*event*/ )
{
}

void MainWindow::onTrayActivation( QSystemTrayIcon::ActivationReason reason )
{
	if( reason == QSystemTrayIcon::Context )
	{
		return;
	}

	this->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
#ifdef Q_OS_ANDROID
    event->accept();
#else

	event->ignore();

	showNotification( tr("Aplicatia va ramane activa in tray") );

	this->hide();
#endif
}

void MainWindow::setupTray()
{
#ifdef Q_OS_ANDROID
	return;
#endif

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon( QPixmap( kAppIconPath ) );
	trayIcon->setToolTip( tr( "Lunch App\nApasa-l" ) );

	NotificationWindow::setup( trayIcon );

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivation(QSystemTrayIcon::ActivationReason)));

	trayIconMenu = new QMenu(this);
	
	QAction* quitAction = new QAction(tr("Inchide de tot"), trayIconMenu);
	quitAction->setIcon(QIcon(RESOURCES_ROOT"quit.png"));
	quitAction->setShortcut(QKeySequence("Ctrl+Q"));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	trayIconMenu->addAction(quitAction);
	
	this->addAction(quitAction);
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
	connect( controller, SIGNAL( notify( const QString&, const QPixmap& ) ), this, SLOT( showNotification( const QString&, const QPixmap& ) ) );

	connect( controller, SIGNAL( weekArrived( const Week& ) ), regularMetroView, SLOT( weekArrived( const Week& ) ), Qt::QueuedConnection );

	connect( regularMetroView, SIGNAL( requestWeekBefore( const Week& ) ), controller, SLOT( requestWeekBefore( const Week& ) ), Qt::QueuedConnection );
	connect( regularMetroView, SIGNAL( requestWeekAfter( const Week& ) ), controller, SLOT( requestWeekAfter( const Week& ) ), Qt::QueuedConnection );
	connect( regularMetroView, SIGNAL( requestWeek( QDate, QDate ) ), controller, SLOT( requestWeek( QDate, QDate ) ), Qt::QueuedConnection );

	connect( regularMetroView, SIGNAL( selectionChangedOn( const Dish& ) ), controller, SLOT( selectionChangedOn( const Dish& ) ), Qt::QueuedConnection );

	if( adminMetroView )
	{
		connect( adminMetroView, SIGNAL( requestAllDishes() ), controller, SLOT( requestAllDishes() ), Qt::QueuedConnection );
		connect( adminMetroView, SIGNAL( publishWeek( const Week& ) ), controller, SLOT( publishWeek( const Week& ) ), Qt::QueuedConnection );
		connect( adminMetroView, SIGNAL( uploadPicture( QPixmap ) ), controller, SLOT( uploadPicture( QPixmap ) ), Qt::QueuedConnection );
	
		connect( controller, SIGNAL( allDishesArrived( Day ) ), adminMetroView, SLOT( allDishesArrived( Day ) ), Qt::QueuedConnection );
	}
}

void MainWindow::sendDummyWeek()
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

	Week week( monday, friday, daysVect );

	regularMetroView->weekArrived( week );
}
