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

	QSurfaceFormat format;
	format.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
	format.setOption( QSurfaceFormat::StereoBuffers, true );
	openGLWidget->setFormat( format );

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
	weekPrefixLabel->move( 40, 30 );

	weekDateLabel = new QLabel( this );
	weekDateLabel->setFont( QFont( kFontName, 10 ) );
	weekDateLabel->move( weekPrefixLabel->x() + weekPrefixLabel->width(), 30 );

	weeksView = new AllWeeksView( this );
	weeksView->move( 0, kWeekOffsetY );

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( weekDateLabel );

	/* Effects */
	QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
	opacity->setOpacity( 1.0f );
	weekDateLabel->setGraphicsEffect( opacity );

	/* Animations */
	weekDateOutAnimation = new QPropertyAnimation( opacity, "opacity" );
	weekDateOutAnimation->setDuration( kWeekAnimationTime / 2.f );
	weekDateOutAnimation->setStartValue( 1.0f );
	weekDateOutAnimation->setEndValue( 0.0f );
	weekDateOutAnimation->setEasingCurve( QEasingCurve::OutCubic );

	weekDateInAnimation = new QPropertyAnimation( opacity, "opacity" );
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

	this->setMinimumWidth( kWeekWidth + 150 );			// don't leave this constant here
	this->setMinimumHeight( this->height() );
	this->adjustSize();

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	// Signals / Slots
	connect( weekMoveAnimation, SIGNAL( finished(void) ), this, SLOT( weekAnimationFinished(void) ) );
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

void MetroView::wheelEvent( QWheelEvent* wheelEvent )
{
	int scrollDist = this->width() /*kDayWidth*/;	// Scroll one column or the whole week
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

			int numDays = (int)((weekMoveAnimation->endValue().toPointF().x() - weekMoveAnimation->startValue().toPointF().x()) / kDayWidth);
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

			int numDays = (int)((weekMoveAnimation->startValue().toPointF().x() - weekMoveAnimation->endValue().toPointF().x()) / kDayWidth);
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
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	weeksView->mainWindowResized( event );

	QGraphicsView::resizeEvent( event );
}

void MetroView::setWeekDateText( const Week &currentWeek )
{
	QLocale romanian;

	QDate startDate = currentWeek.getStartDate();
	QDate endDate = currentWeek.getEndDate();
	if( startDate.month() != endDate.month() )
	{
		weekDateLabel->setText( QString::number( startDate.day() ) + " " + romanian.monthName( startDate.month() )
			+ " - " + QString::number( endDate.day() ) + " " + romanian.monthName( endDate.month() ) );
	}
	else
	{
		weekDateLabel->setText( QString::number( startDate.day() ) + " - " + QString::number( endDate.day() ) 
			+ " " + romanian.monthName( endDate.month() ) );
	}

	weekDateLabel->adjustSize();

	weekDateInAnimation->start();
}