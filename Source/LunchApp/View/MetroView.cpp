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


MetroView::MetroView(QWidget *parent)
	: QGraphicsView(parent)
{
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
	// Gradient background
	// 	QLinearGradient linearGrad(	QPointF( this->width() / 4.f, 0 ), QPointF( this->width() / 2.f, this->height() ) );
	// 	linearGrad.setColorAt( 1, QColor( 135, 206, 255 ) );
	// 	linearGrad.setColorAt( 0, QColor( 30, 144, 255 ) );
	// 	this->setBackgroundBrush( QBrush( linearGrad ) );

	// Image background
	//	this->setBackgroundBrush( QBrush( QPixmap( "Resources\\background1.jpg" ) ) );

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

	weeksView = new AllWeeksView( this );
	weeksView->move( 0, kWeekTopOffset );

	userLabel = new QLabel( this );
	userLabel->setText( Controller::getUser()->getUsername() );
	userLabel->setFont( QFont( kFontName, 10 ) );
	userLabel->adjustSize();

	calendar = new QCalendarWidget( this );
	calendar->hide();

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( weeksView );
	scene->addWidget( weekDateButton );
	scene->addWidget( userLabel );

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
	this->setMinimumSize( Style::getWeekWidth(), Style::getWindowHeight() );
	this->adjustSize();

	// Move
	userLabel->move( this->width() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	// Signals / Slots
	connect( weekMoveAnimation, SIGNAL( finished(void) ), this, SLOT( weekAnimationFinished(void) ) );
	connect( weekDateButton, SIGNAL( clicked( bool ) ), this, SLOT( weekDatePressed( bool ) ) );
	connect( calendar, SIGNAL( selectionChanged() ), this, SLOT( dateSelected() ) );
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

	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	weeksView->mainWindowResized( event );

	userLabel->move( event->size().width() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
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