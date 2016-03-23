#include "DayView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QScroller>

#include "Style.h"
#include "WeekView.h"
#include "DishView.h"


DayView::DayView( QWidget *parent, const Day& day )
	: QWidget( parent )
	, day( day )
{
	init();
}

DayView::~DayView()
{

}

void DayView::init()
{
	QScroller::grabGesture( this, QScroller::LeftMouseButtonGesture );

	/* Create objects */
	dayNameLabel = new QLabel( this );
	dayNameLabel->setText( day.getName() );
	dayNameLabel->setFont( QFont( kFontName, 20 ) );
	dayNameLabel->setStyleSheet( "background-color: rgba(255, 255, 255, 0);" );
	dayNameLabel->adjustSize();

	dishesView = new DayDishesView( this, day.getDishes() );

	// Move objects
	dishesView->move( 0, dayNameLabel->height() +  kDishSpacing );

	this->adjustSize();
	dayNameLabel->move( kDishSpacing + ( this->width() - dayNameLabel->width() ) / 2.0f , 0 );

	this->setMinimumSize( this->size() + QSize( 0, kDishSpacing ) );	// add bottom spacing so the shadow is rendered completely
	this->adjustSize();
}

void DayView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((WeekView*)this->parent())->wheelEvent( wheelEvent );
}

void DayView::mainWindowResized( QResizeEvent* event )
{
	dishesView->mainWindowResized( event );
}

void DayView::selectionChangedOn( const Dish& dish )
{
	((WeekView*)this->parent())->selectionChangedOn( dish );
}