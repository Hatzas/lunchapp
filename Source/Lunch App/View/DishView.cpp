#include "DishView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPalette>
#include <QPainter>

#include "Style.h"
#include "DayView.h"

static const float			kBriefDetailsOffset		= 30;
static const QString		kDishDetailsText		= "Quesadilla cu pui \n\n   Ingrediente: tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare";

DishView::DishView( QWidget *parent, QPixmap pixmap )
	: QWidget( parent )
	, dishPixmap( pixmap )
{
	init();
}

DishView::~DishView()
{

}

void DishView::init()
{
	// Compute scale based on image size and desired dish width
	float scale = kDishWidth / dishPixmap.width();
	QSize widgetSize = dishPixmap.size() * scale;

	/* Create objects */
	imageLabel = new QLabel( this );
	imageLabel->setPixmap( dishPixmap );
	imageLabel->adjustSize();

	detailsLabel = new QLabel( this );
	detailsLabel->setText( kDishDetailsText );
	detailsLabel->setWordWrap( true );
	detailsLabel->setFont( QFont( kFontName, 10 ) );
	detailsLabel->setAutoFillBackground( true );
	detailsLabel->setMinimumSize( widgetSize );
	detailsLabel->setMaximumSize( widgetSize );
	detailsLabel->setAlignment( Qt::AlignTop );
	detailsLabel->setStyleSheet( "background-color: rgba(255,255,255,80%)" );
	detailsLabel->adjustSize();

	/* Animations */
	detailsAnimation = new QPropertyAnimation( detailsLabel, "pos" );
	detailsAnimation->setEasingCurve( QEasingCurve::OutCirc );

	/* Move objects */
	detailsLabel->move( 0, widgetSize.height() );

	// Properties
	this->setMouseTracking( true );
	imageLabel->setMouseTracking( true );
	detailsLabel->setMouseTracking( true );

	this->setMinimumSize( widgetSize );
	this->setMaximumSize( widgetSize );
	
	this->adjustSize();
}

void DishView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((DayView*)this->parent())->wheelEvent( wheelEvent );
}

void DishView::enterEvent( QEvent* event )
{
	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	// Start text animation
	detailsAnimation->setStartValue( detailsLabel->pos() );
	detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() - kBriefDetailsOffset ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();
}

void DishView::leaveEvent( QEvent* event )
{
	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	detailsAnimation->setStartValue( detailsLabel->pos() );
	detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();
}

void DishView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
	// Show full details if mouse over them
	if( ( this->childAt( mouseEvent->pos() ) == detailsLabel ) && ( detailsAnimation->state() != QAbstractAnimation::Running ) )
	{
		detailsAnimation->setStartValue( detailsLabel->pos() );
		detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), 0.0f ) );
		
		detailsAnimation->setDuration( kFullDishDetailsAnimationTime );
		detailsAnimation->start();
	}
}