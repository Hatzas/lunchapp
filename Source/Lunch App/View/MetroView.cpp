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

#include "Style.h"
#include "InfiniteBackground.h"


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
	/* Create objects */
	background = new InfiniteBackground( QPixmap( "Resources\\background5.bmp" ), this );

	QLabel* textLabel = new QLabel( this );
	textLabel->setText( "Saptamana: 1 - 6 Martie" );
	textLabel->setFont( QFont( kFontName, 10 ) );
	textLabel->adjustSize();
	textLabel->move( 40, 30 );

	QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect();
	opacity->setOpacity( 1.0f );
	textLabel->setGraphicsEffect( opacity );

	currentWeekView = new WeekView( this );
	currentWeekView->move( 0, kWeekOffsetY );

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( textLabel );
	scene->addWidget( currentWeekView );

	/* Create animations */
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

	weekAnimation = new QPropertyAnimation( currentWeekView, "pos" );
	weekAnimation->setEasingCurve( QEasingCurve::OutCirc );

	backgroundAnimation = new QPropertyAnimation( background, "offset" );
	backgroundAnimation->setEasingCurve( QEasingCurve::OutCirc );

	animations = new QParallelAnimationGroup();
	animations->addAnimation( backgroundAnimation );
	//animations->addAnimation( textAnimations );
	animations->addAnimation( weekAnimation );

	this->setMinimumWidth( currentWeekView->width() );
	this->setMinimumHeight( this->height() );

	this->adjustSize();

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	// Gradient background
// 	QLinearGradient linearGrad(	QPointF( this->width() / 4.f, 0 ), QPointF( this->width() / 2.f, this->height() ) );
// 	linearGrad.setColorAt( 1, QColor( 135, 206, 255 ) );
// 	linearGrad.setColorAt( 0, QColor( 30, 144, 255 ) );
// 	this->setBackgroundBrush( QBrush( linearGrad ) );

	// Image background
//	this->setBackgroundBrush( QBrush( QPixmap( "Resources\\background1.jpg" ) ) );
}

void MetroView::wheelEvent( QWheelEvent* wheelEvent )
{
	if( wheelEvent->delta() > 0 )
	{
		if( weekAnimation->state() != QAbstractAnimation::Running )
		{
			weekAnimation->setStartValue( currentWeekView->pos() );
			weekAnimation->setEndValue( QPointF( currentWeekView->pos().x() + kDayWidth, currentWeekView->pos().y() ) );
			weekAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() + kDayWidth * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			animations->stop();

			weekAnimation->setStartValue( weekAnimation->currentValue().toPointF() );
			weekAnimation->setEndValue( QPointF( weekAnimation->endValue().toPointF().x() + kDayWidth, currentWeekView->pos().y() ) );

			int numDays = (int)((weekAnimation->endValue().toPointF().x() - weekAnimation->startValue().toPointF().x()) / kDayWidth);
			weekAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() + kDayWidth * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			animations->start();
		}
	} 
	else if( wheelEvent->delta() < 0 )
	{
		if( weekAnimation->state() != QAbstractAnimation::Running )
		{
			weekAnimation->setStartValue( currentWeekView->pos() );
			weekAnimation->setEndValue( QPointF( currentWeekView->pos().x() - kDayWidth, currentWeekView->pos().y() ) );
			weekAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() - kDayWidth * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			animations->stop();

			weekAnimation->setStartValue( weekAnimation->currentValue().toPointF() );
			weekAnimation->setEndValue( QPointF( weekAnimation->endValue().toPointF().x() - kDayWidth, currentWeekView->pos().y() ) );

			int numDays = (int)((weekAnimation->startValue().toPointF().x() - weekAnimation->endValue().toPointF().x()) / kDayWidth);
			weekAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() - kDayWidth * kBackgroundScrollRatio, background->getOffset().y() ) );
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
}