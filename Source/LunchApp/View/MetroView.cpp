#include "MetroView.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QParallelAnimationGroup>
#include <QGraphicsSceneWheelEvent>
#include <QWheelEvent>
#include <QAbstractAnimation>
#include <QOpenGLWidget>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QBoxLayout>
#include <QScroller>
#include <QScroller>
#include <QLocale>

#include "Style.h"
#include "InfiniteBackground.h"
#include "MainWindow.h"
#include "../Controller/Controller.h"


static const QString kWeekDatePrefix		= "Saptamana: ";


MetroView::MetroView( QWidget *parent, bool adminMode /*= false*/ )
	: QGraphicsView( parent )
	, adminMode( adminMode )
	, allDishesView( NULL )
{
// 	if( Controller::getUser()->getRole() == User::eAdmin )
// 		adminMode = true;									// This should be replaced by a menu button

	init();
}

MetroView::~MetroView()
{

}

void MetroView::init()
{
#ifndef QT_NO_OPENGL
	QOpenGLWidget* openGLWidget = new QOpenGLWidget();

// 	QSurfaceFormat format;
// 	format.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
// 	format.setOption( QSurfaceFormat::StereoBuffers, true );
// 	openGLWidget->setUpdateBehavior( QOpenGLWidget::PartialUpdate );
// 	openGLWidget->setFormat( format );

	this->setViewport( openGLWidget );
#endif

	this->setRenderHint( QPainter::Antialiasing );
	this->setRenderHint( QPainter::SmoothPixmapTransform );
	this->setCacheMode( QGraphicsView::CacheBackground );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	scene = new QGraphicsScene( this );
	this->setScene( scene );

	addSceneItems();
}

void MetroView::addSceneItems()
{
	/* Background */
	background = new InfiniteBackground( QPixmap( "Resources\\background3.bmp" ), this );

	/* Create objects */
	QLabel* weekPrefixLabel = new QLabel( this );
	weekPrefixLabel->setText( kWeekDatePrefix );
	weekPrefixLabel->setFont( QFont( kFontName, 10 ) );
	weekPrefixLabel->adjustSize();
	weekPrefixLabel->move( kDateUsernameSideOffset, kDateUsernameTopOffset );

	weekDateButton = new QPushButton( this );
	weekDateButton->setFont( QFont( kFontName, 10 ) );
	weekDateButton->setStyleSheet( kButtonsStyleSheet );
	weekDateButton->move( weekPrefixLabel->x() + weekPrefixLabel->width(), 30 );

	weeksView = new AllWeeksView( this, adminMode );
	weeksView->move( 0, kWeekTopOffset );

	if( adminMode )
	{
		// Add view with all dishes
		allDishesView = new DayView( this, "Toate", getAllDishes(), eBrowseMode );

		// Move to right of screen
		allDishesView->move( this->width() - allDishesView->width() - 2 * Style::getDishSpacing(), weeksView->y() );
	}

	userLabel = new QLabel( this );
	userLabel->setText( Controller::getUser()->getUsername() );
	userLabel->setFont( QFont( kFontName, 10 ) );
	userLabel->adjustSize();

	administrateButton = new QPushButton( this );
	administrateButton->setText( adminMode ? " Meniu " : " Administrare " );
	administrateButton->setFont( QFont( kFontName, 10 ) );
	administrateButton->setStyleSheet( kButtonsStyleSheet );

	if( Controller::getUser()->getRole() != User::eAdmin )
		administrateButton->hide();

	calendar = new QCalendarWidget( this );
	calendar->hide();

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( weeksView );
	scene->addWidget( weekDateButton );
	scene->addWidget( userLabel );
	scene->addWidget( administrateButton );

	/* Effects */
	QGraphicsOpacityEffect *weekDateOpacity = new QGraphicsOpacityEffect();
	weekDateOpacity->setOpacity( 1.0f );
	weekDateButton->setGraphicsEffect( weekDateOpacity );

	QGraphicsOpacityEffect *calendarOpacity = new QGraphicsOpacityEffect();
	calendarOpacity->setOpacity( 0.5f );
	//calendar->setGraphicsEffect( calendarOpacity );

	/* Animations */
	calendarFadeAnimation = new QPropertyAnimation( calendarOpacity, "opacity" );
	calendarFadeAnimation->setDuration( kWeekAnimationTime / 2.f );
	calendarFadeAnimation->setEasingCurve( QEasingCurve::InCubic );

	weekDateOutAnimation = new QPropertyAnimation( weekDateOpacity, "opacity" );
	weekDateOutAnimation->setDuration( kWeekAnimationTime / 2.f );
	weekDateOutAnimation->setStartValue( 1.0f );
	weekDateOutAnimation->setEndValue( 0.0f );
	weekDateOutAnimation->setEasingCurve( QEasingCurve::OutCubic );

	weekDateInAnimation = new QPropertyAnimation( weekDateOpacity, "opacity" );
	weekDateInAnimation->setDuration( kWeekAnimationTime / 2.f );
	weekDateInAnimation->setStartValue( 0.0f );
	weekDateInAnimation->setEndValue( 1.0f );
	weekDateInAnimation->setEasingCurve( QEasingCurve::InCubic );

	weekMoveAnimation = new QPropertyAnimation( weeksView, "pos" );
	weekMoveAnimation->setEasingCurve( QEasingCurve::OutCirc );

	backgroundAnimation = new QPropertyAnimation( background, "offset" );
	backgroundAnimation->setEasingCurve( QEasingCurve::OutCirc );

	animations = new QParallelAnimationGroup();
	animations->addAnimation( backgroundAnimation );
	animations->addAnimation( weekMoveAnimation );
	animations->addAnimation( weekDateOutAnimation );

	// Size	
	if( allDishesView )
		this->setMinimumSize( Style::getWeekWidth() + allDishesView->width() + Style::getDishSpacing(), Style::getWindowHeight() );
	else
		this->setMinimumSize( Style::getWeekWidth(), Style::getWindowHeight() );
	this->adjustSize();

	// Move
	if( Controller::getUser()->getRole() == User::eAdmin )
	{
		administrateButton->move( this->width() - administrateButton->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
		userLabel->move( administrateButton->x() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
	}
	else
	{
		userLabel->move( this->width() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
	}

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	// Signals / Slots
	connect( weekMoveAnimation, SIGNAL( finished(void) ), this, SLOT( weekAnimationFinished(void) ) );
	connect( weekDateButton, SIGNAL( clicked( bool ) ), this, SLOT( weekDatePressed( bool ) ) );
	connect( calendar, SIGNAL( selectionChanged() ), this, SLOT( dateSelected() ) );
	connect( administrateButton, SIGNAL( clicked( bool ) ), parent(), SLOT( switchAdministrate( bool ) ) );
}

void MetroView::weekArrived( const Week& week )
{
	weeksView->addWeek( week );

	setWeekDateText( week );
}

void MetroView::weekAnimationFinished()
{
	// Get currently visible week
	Week currentWeek = weeksView->getVisibleWeek();
	if( currentWeek.getDays().size() == 0 )
		return;

	setWeekDateText( currentWeek );
}

void MetroView::weekDatePressed( bool checked )
{
	calendar->move( weekDateButton->x() + weekDateButton->width(), weekDateButton->y() + weekDateButton->height() );

// 	if( !calendar->isVisible() )
// 	{
// 		calendarFadeAnimation->setStartValue( 0.0f );
// 		calendarFadeAnimation->setEndValue( 1.0f );
// 
// 		calendarFadeAnimation->start();
// 	}

	calendar->setVisible( !calendar->isVisible() );	
}

void MetroView::dateSelected()
{
	calendar->setVisible( false );

	QDate date = calendar->selectedDate();

	// Determine week based selected date
	QDate startDate = date.addDays( -date.dayOfWeek() + 1 );
	QDate endDate = date.addDays( 5 - date.dayOfWeek() );

	// Request week based on selected date
	// TO DO
}

void MetroView::wheelEvent( QWheelEvent* wheelEvent )
{
	calendar->setVisible( false );

	if( adminMode )		// Scrolling through weeks disabled for now
		return;

	int scrollDist = this->width() /*Style::getDayWidth()*/;	// Scroll one column or the whole week
	if( wheelEvent->delta() > 0 )
	{
		if( weekMoveAnimation->state() != QAbstractAnimation::Running )
		{
			// Notify week of scroll and check if not already loading
			bool canScroll = weeksView->scrollStarted( AllWeeksView::eToLeftDirection );
			if( !canScroll )
				return;

			// Update animations
			weekMoveAnimation->setStartValue( weeksView->pos() );
			weekMoveAnimation->setEndValue( QPointF( weeksView->pos().x() + scrollDist, weeksView->pos().y() ) );
			weekMoveAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() + scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			return;

			animations->stop();

			weekMoveAnimation->setStartValue( weekMoveAnimation->currentValue().toPointF() );
			weekMoveAnimation->setEndValue( QPointF( weekMoveAnimation->endValue().toPointF().x() + scrollDist, weeksView->pos().y() ) );

			int numDays = (int)((weekMoveAnimation->endValue().toPointF().x() - weekMoveAnimation->startValue().toPointF().x()) / Style::getDayWidth());
			weekMoveAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() + scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			animations->start();
		}
	} 
	else if( wheelEvent->delta() < 0 )
	{
		if( weekMoveAnimation->state() != QAbstractAnimation::Running )
		{
			// Notify week of scroll and check if not already loading
			bool canScroll = weeksView->scrollStarted( AllWeeksView::eToRightDirection );
			if( !canScroll )
				return;

			// Update animations
			weekMoveAnimation->setStartValue( weeksView->pos() );
			weekMoveAnimation->setEndValue( QPointF( weeksView->pos().x() - scrollDist, weeksView->pos().y() ) );
			weekMoveAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() - scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			return;

			animations->stop();

			weekMoveAnimation->setStartValue( weekMoveAnimation->currentValue().toPointF() );
			weekMoveAnimation->setEndValue( QPointF( weekMoveAnimation->endValue().toPointF().x() - scrollDist, weeksView->pos().y() ) );

			int numDays = (int)((weekMoveAnimation->startValue().toPointF().x() - weekMoveAnimation->endValue().toPointF().x()) / Style::getDayWidth());
			weekMoveAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() - scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			animations->start();
		}
	}
}

void MetroView::resizeEvent( QResizeEvent * event )
{
	QGraphicsView::resizeEvent( event );

	background->setFixedSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	QSize size = event->size();
	if( allDishesView )
		size.setWidth( size.width() - allDishesView->width() - 2 * Style::getDishSpacing() );

	weeksView->mainWindowResized( size );

	// Move to right of screen
	if( adminMode && allDishesView )
		allDishesView->move( this->width() - allDishesView->width() - 2 * Style::getDishSpacing(), weeksView->y() );

	if( Controller::getUser()->getRole() == User::eAdmin )
	{
		administrateButton->move( this->width() - administrateButton->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
		userLabel->move( administrateButton->x() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
	}
	else
	{
		userLabel->move( this->width() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
	}
}

void MetroView::setWeekDateText( const Week &currentWeek )
{
	QLocale romanian;

	QDate startDate = currentWeek.getStartDate();
	QDate endDate = currentWeek.getEndDate();
	if( startDate.month() != endDate.month() )
	{
		weekDateButton->setText( " " + QString::number( startDate.day() ) + " " + romanian.monthName( startDate.month() )
			+ " - " + QString::number( endDate.day() ) + " " + romanian.monthName( endDate.month() ) + " " );
	}
	else
	{
		weekDateButton->setText( " " + QString::number( startDate.day() ) + " - " + QString::number( endDate.day() ) 
			+ " " + romanian.monthName( endDate.month() ) + " " );
	}

	weekDateButton->adjustSize();

	weekDateInAnimation->start();
}

std::vector<Dish> MetroView::getAllDishes()
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
