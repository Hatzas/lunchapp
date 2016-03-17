#include "DayView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>

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
	/* Create objects */
	dayNameLabel = new QLabel( this );
	dayNameLabel->setText( day.getName() );
	dayNameLabel->setFont( QFont( kFontName, 20 ) );
	dayNameLabel->setStyleSheet( "background-color: rgba(255, 255, 255, 0);" );
	dayNameLabel->adjustSize();

	AddDishes();

	// Move objects
	this->adjustSize();
	dayNameLabel->move( kDishSpacing + ( this->width() - dayNameLabel->width() ) / 2.0f , 0 );
	StackDishViews();

	this->setMinimumSize( this->size() + QSize( 0, kDishSpacing ) );	// add bottom spacing so the shadow is rendered completely
	this->adjustSize();
}

void DayView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((WeekView*)this->parent())->wheelEvent( wheelEvent );
}

void DayView::AddDishes()
{
	std::vector<Dish>& dishesVect = day.getDishes();
	for( size_t i = 0 ; i < dishesVect.size() ; i++ )
	{
		disheViewsVect.push_back( new DishView( this, dishesVect[i] ) );
	}
}

void DayView::StackDishViews()
{
	if( disheViewsVect.size() == 0 )
		return;

	// First image placed below the day name
	disheViewsVect[0]->move( kDishSpacing, dayNameLabel->height() + 2 * kDishSpacing );

	// The rest, place below one another (for now)
	for( size_t i = 1 ; i < disheViewsVect.size() ; i++ )
	{
		disheViewsVect[i]->move( kDishSpacing, disheViewsVect[i-1]->y() + disheViewsVect[i-1]->height() + kDishSpacing );
	}

	// Stack images

	// Sort by size
	// TO DO

	// Stack
	// TO DO
}
