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
	mondayView->move( kDaySpacing, 0 );

	tuesdayView = new DayView( this, "Marti" );
	tuesdayView->move( mondayView->pos().x() + mondayView->width() + kDaySpacing, 0 );

	wednesdayView = new DayView( this, "Miercuri" );
	wednesdayView->move( tuesdayView->pos().x() + tuesdayView->width() + kDaySpacing, 0 );

	thursdayView = new DayView( this, "Joi" );
	thursdayView->move( wednesdayView->pos().x() + wednesdayView->width() + kDaySpacing, 0 );

	fridayView = new DayView( this, "Vineri" );
	fridayView->move( thursdayView->pos().x() + thursdayView->width() + kDaySpacing, 0 );

	this->adjustSize();
}

void WeekView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}