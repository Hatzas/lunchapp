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

#include "Style.h"


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

	animations = new QParallelAnimationGroup();
	//animations->addAnimation( textAnimations );
	animations->addAnimation( weekAnimation );

	this->setMinimumWidth( currentWeekView->width() );
	this->setMinimumHeight( this->height() );

	this->adjustSize();
}

void MetroView::wheelEvent( QWheelEvent* wheelEvent )
{
	if( wheelEvent->delta() > 0 )
	{
		if( weekAnimation->state() != QAbstractAnimation::Running )
		{
			weekAnimation->setStartValue( currentWeekView->pos() );
			weekAnimation->setEndValue( QPointF( currentWeekView->pos().x() + /*this->width()*/ kDayWidth, currentWeekView->pos().y() ) );
			weekAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			animations->stop();

			weekAnimation->setStartValue( weekAnimation->currentValue().toPointF() );
			weekAnimation->setEndValue( QPointF( weekAnimation->endValue().toPointF().x() + /*this->width()*/ kDayWidth, currentWeekView->pos().y() ) );

			int numDays = (int)((weekAnimation->endValue().toPointF().x() - weekAnimation->startValue().toPointF().x()) / kDayWidth);
			weekAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			animations->start();
		}
	} 
	else if( wheelEvent->delta() < 0 )
	{
		if( weekAnimation->state() != QAbstractAnimation::Running )
		{
			weekAnimation->setStartValue( currentWeekView->pos() );
			weekAnimation->setEndValue( QPointF( currentWeekView->pos().x() - /*this->width()*/ kDayWidth, currentWeekView->pos().y() ) );
			weekAnimation->setDuration( kWeekAnimationTime );

			animations->start();
		}
		else
		{
			animations->stop();

			weekAnimation->setStartValue( weekAnimation->currentValue().toPointF() );
			weekAnimation->setEndValue( QPointF( weekAnimation->endValue().toPointF().x() - /*this->width()*/ kDayWidth, currentWeekView->pos().y() ) );

			int numDays = (int)((weekAnimation->startValue().toPointF().x() - weekAnimation->endValue().toPointF().x()) / kDayWidth);
			weekAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			animations->start();
		}
	}
}