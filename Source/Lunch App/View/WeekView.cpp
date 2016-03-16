#include "WeekView.h"

#include "Style.h"
#include "MetroView.h"


WeekView::WeekView( QWidget *parent )
	: QWidget(parent)
{
	init();
}

WeekView::~WeekView()
{

}

void WeekView::init()
{
	/* Create objects */
	mondayView = new DayView( this, "Luni" );
	mondayView->move( 0, 0 );

	tuesdayView = new DayView( this, "Marti" );
	tuesdayView->move( mondayView->pos().x() + mondayView->width() - kDishSpacing, 0 );	// "- kDishSpacing" is a temporary patch to fix double spacing between days

	wednesdayView = new DayView( this, "Miercuri" );
	wednesdayView->move( tuesdayView->pos().x() + tuesdayView->width() - kDishSpacing, 0 );

	thursdayView = new DayView( this, "Joi" );
	thursdayView->move( wednesdayView->pos().x() + wednesdayView->width() - kDishSpacing, 0 );

	fridayView = new DayView( this, "Vineri" );
	fridayView->move( thursdayView->pos().x() + thursdayView->width() - kDishSpacing, 0 );

	this->adjustSize();
}

void WeekView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}