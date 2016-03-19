#include "AllWeeksView.h"

#include <QMoveEvent>

#include "Style.h"
#include "MetroView.h"


AllWeeksView::AllWeeksView(QWidget *parent)
	: QWidget(parent)
	, requestSent( false )
{

}

AllWeeksView::~AllWeeksView()
{

}

void AllWeeksView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}

void AllWeeksView::addWeek( const Week& week )
{
	WeekView* weekView = new WeekView( this, week );

	// Position according to date
	if( weekViewsVect.size() > 0 )
	{
		// TO DO
		int x1 = weekViewsVect[0]->pos().x();
		int x2 = weekView->width();
		weekView->move( weekViewsVect[0]->pos().x() - weekView->width(), 0 );
	}

	// Insert sorted based on date
	// TO DO
	weekViewsVect.push_back( weekView );

	this->adjustSize();
}

void AllWeeksView::moveEvent( QMoveEvent* event )
{
	if( requestSent )
		return;

	int x = event->pos().x();
	if( x > kDishSpacing )
	{
		// Request week before the first
		emit ((MetroView*)this->parent())->requestWeekBefore( weekViewsVect[0]->getWeek() );

		requestSent = true;
	}
	else if( x < -kDishSpacing )
	{
		// Request week after the last
		emit ((MetroView*)this->parent())->requestWeekBefore( weekViewsVect[ weekViewsVect.size()-1 ]->getWeek() );

		requestSent = true;
	}
}
