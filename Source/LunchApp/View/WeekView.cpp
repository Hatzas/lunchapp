#include "WeekView.h"

#include "Style.h"
#include "AllWeeksView.h"


WeekView::WeekView( QWidget *parent, const Week& week  )
	: QWidget(parent)
	, week( week )
{
	init();
}

WeekView::~WeekView()
{

}

void WeekView::init()
{
	/* Create objects */
	AddDays();

	this->adjustSize();
}

void WeekView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((AllWeeksView*)this->parent())->wheelEvent( wheelEvent );
}

void WeekView::resizeEvent( QResizeEvent* event )
{
	for( int i = 0 ; i < dayViewsVect.size() ; i++ )
	{
		dayViewsVect[i]->resizeEvent( event );
	}
}

void WeekView::selectionChangedOn( const Dish& dish )
{
	((AllWeeksView*)this->parent())->selectionChangedOn( dish );
}

void WeekView::AddDays()
{
	std::vector<Day>& daysVect = week.getDays();
	for( size_t i = 0 ; i < daysVect.size() ; i++ )
	{
		DayView* dayView = new DayView( this, daysVect[i] );

		if( i > 0 )
			dayView->move( dayViewsVect[i-1]->pos().x() + dayViewsVect[i-1]->width() - kDishSpacing + kDaySpacing, 0 );		// "- kDishSpacing" is a temporary patch to fix double spacing between days

		dayViewsVect.push_back( dayView );
	}
}
