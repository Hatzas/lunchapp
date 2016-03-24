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

#include "Style.h"
#include "InfiniteBackground.h"


static const QString kWeekTextPrefix		= "Saptamana: ";


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
	weekLabel = new QLabel( this );
	weekLabel->setText( kWeekTextPrefix );
	weekLabel->setFont( QFont( kFontName, 10 ) );
	weekLabel->adjustSize();
	weekLabel->move( 40, 30 );

	weeksView = new AllWeeksView( this );
	weeksView->move( 0, kWeekOffsetY );

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( weekLabel );

	/* Effects */
	QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
	opacity->setOpacity( 1.0f );
	weekLabel->setGraphicsEffect( opacity );

	/* Animations */
	QPropertyAnimation* weekTextAnimationOut = new QPropertyAnimation( opacity, "opacity" );
	weekTextAnimationOut->setDuration( kWeekAnimationTime / 2.f );
	weekTextAnimationOut->setStartValue( 1.0f );
	weekTextAnimationOut->setEndValue( 0.0f );
	weekTextAnimationOut->setEasingCurve( QEasingCurve::OutCubic );

	QPropertyAnimation* weekTextAnimationIn = new QPropertyAnimation( opacity, "opacity" );
	weekTextAnimationIn->setDuration( kWeekAnimationTime / 2.f );
	weekTextAnimationIn->setStartValue( 0.0f );
	weekTextAnimationIn->setEndValue( 1.0f );
	weekTextAnimationIn->setEasingCurve( QEasingCurve::InCubic );

	QSequentialAnimationGroup* textAnimations = new QSequentialAnimationGroup();
	textAnimations->addAnimation( weekTextAnimationOut );
	textAnimations->addAnimation( weekTextAnimationIn );

	weekMoveAnimation = new QPropertyAnimation( weeksView, "pos" );
	weekMoveAnimation->setEasingCurve( QEasingCurve::OutCirc );

	backgroundAnimation = new QPropertyAnimation( background, "offset" );
	backgroundAnimation->setEasingCurve( QEasingCurve::OutCirc );

	animations = new QParallelAnimationGroup();
	animations->addAnimation( backgroundAnimation );
	//animations->addAnimation( textAnimations );
	animations->addAnimation( weekMoveAnimation );

	this->setMinimumWidth( kWeekWidth + 150 );			// don't leave this constant here
	this->setMinimumHeight( this->height() );
	this->adjustSize();

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();
}

void MetroView::weekArrived( const Week& week )
{
	weeksView->addWeek( week );
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

	// Center week on screen
	// TO DO

	weeksView->mainWindowResized( event );

	QGraphicsView::resizeEvent( event );
}