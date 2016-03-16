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


static const float			kClickMovement			= 2;
static const float			kBriefDetailsOffset		= 30;
static const QString		kDishDetailsText		= "Quesadilla cu pui \n\n   Ingrediente: tortilla  piept de pui  cascaval  ardei gras  ceapa  patrunjel  ulei  boia  usturoi  oregano  sare";


DishView::DishView( QWidget *parent, QPixmap pixmap )
	: QWidget( parent )
	, dishPixmap( pixmap )
	, mousePressed( false )
	, dishSelected( false )
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
	imageLabel->setMinimumSize( widgetSize );
	imageLabel->setMaximumSize( widgetSize );
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

	/* Effects */
	selectedEffect = new SelectedEffect( imageLabel );
	imageLabel->setGraphicsEffect( selectedEffect );

	/* Move objects */
	detailsLabel->move( 0, widgetSize.height() );

	/* Properties */
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
	if( mousePressed )
	{
		mousePressed = false;

		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );
	}

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

void DishView::mousePressEvent( QMouseEvent* mouseEvent )
{
	mousePressed = true;

	this->move( this->pos() + QPoint( kClickMovement, kClickMovement ) );
}

void DishView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
	if( mousePressed )
	{
		mousePressed = false;

		dishSelected = !dishSelected;

		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );
	}

	if( dishSelected )
	{
		selectedEffect->enable();

		// Drop shadow is computation intensive and reduces performance
		QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect( this );
		dropShadow->setOffset( 0.0f );
		dropShadow->setBlurRadius( kSelectedShadowSize );
		dropShadow->setColor( kSelectedGlowColor );
		this->setGraphicsEffect( dropShadow );
	}
	else
	{
		selectedEffect->disable();

		this->setGraphicsEffect( NULL );
	}
}