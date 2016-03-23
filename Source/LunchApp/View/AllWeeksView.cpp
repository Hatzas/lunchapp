#include "AllWeeksView.h"

#include <QMoveEvent>
#include <QMovie>

#include "Style.h"
#include "MetroView.h"


AllWeeksView::AllWeeksView(QWidget *parent)
	: QWidget(parent)
{
	init();
}

AllWeeksView::~AllWeeksView()
{
}

void AllWeeksView::init()
{
	loadingLabel = new QLabel( this );
	loadingLabel->setMovie( new QMovie( "Resources/loader1.gif" ) );
	loadingLabel->movie()->setScaledSize( kLoadingAnimSize );
	loadingLabel->setMinimumSize( kLoadingAnimSize );
	loadingLabel->adjustSize();
	loadingLabel->hide();
}

void AllWeeksView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}

void AllWeeksView::resizeEvent( QResizeEvent* event )
{
	// Increase or decrease spacing between weeks so they can be centered on the screen
	// TO DO

	for( int i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		weekViewsVect[i]->resizeEvent( event );
	}
}

void AllWeeksView::selectionChangedOn( const Dish& dish )
{
	emit ((MetroView*)this->parent())->selectionChangedOn( dish );
}

void AllWeeksView::addWeek( const Week& week )
{
	showLoadingAnim( false );

	WeekView* weekView = new WeekView( this, week );

	int startDay = week.getStartDate().day();
	int endDay = week.getEndDate().day();

	// Position according to date
	if( weekViewsVect.size() == 0 )
	{
		weekViewsVect.push_back( weekView );
	}
	else if( week < weekViewsVect[0]->getWeek() )
	{
		// Insert before first
		weekView->move( weekViewsVect[0]->pos().x() - weekView->width(), 0 );

		weekViewsVect.insert( weekViewsVect.begin(), weekView );
	}
	else if( week > weekViewsVect[ weekViewsVect.size() - 1 ]->getWeek() )
	{
		// Insert after last
		weekView->move( weekViewsVect[ weekViewsVect.size() - 1 ]->pos().x() + weekViewsVect[ weekViewsVect.size() - 1 ]->width(), 0 );

		weekViewsVect.push_back( weekView );
	}

	weekView->show();

	this->adjustSize();
}

bool AllWeeksView::scrollStarted( EDirection direction )
{
	if( weekViewsVect.size() == 0 )
		return false;

	// If loading, do nothing
	if( loadingLabel->isVisible() )
		return false;

	int x = this->x();
	if( direction == eToLeftDirection && this->x() == 0 )
	{
		// Increase view size and reposition to accommodate new view
		increaseSize( direction );
		showLoadingAnim( true, direction );

		// Request week before the first
		emit ((MetroView*)this->parent())->requestWeekBefore( weekViewsVect[0]->getWeek() );
	}
	else if(  direction == eToRightDirection && ( this->x() + this->width() - weekViewsVect[ weekViewsVect.size()-1 ]->width() == 0 ) )
	{
		// Increase view size and reposition to accommodate new view
		increaseSize( direction );
		showLoadingAnim( true, direction );

		// Request week after the last
		emit ((MetroView*)this->parent())->requestWeekAfter( weekViewsVect[ weekViewsVect.size()-1 ]->getWeek() );
	}

	return true;
}

void AllWeeksView::increaseSize( EDirection direction )
{
	this->setMinimumWidth( this->width() + kWeekWidth );				// "kWeekWidth" should be replaced with a more dynamic approach
																		// as the views could be centered on a larger window
	if( direction == eToLeftDirection )
	{
		this->move( this->x() - kWeekWidth, this->y() );

		// Reposition all weeks so visually they remain the same
		for( int i = 0 ; i < weekViewsVect.size() ; i++ )
		{
			weekViewsVect[i]->move( weekViewsVect[i]->x() + kWeekWidth, weekViewsVect[i]->y() );
		}
	}

	this->adjustSize();
}

void AllWeeksView::showLoadingAnim( bool show, EDirection direction )
{
	if( !show )
	{
		loadingLabel->movie()->stop();
		loadingLabel->hide();

		return;
	}

	if( direction == eToRightDirection )
	{
		loadingLabel->move( this->width() - kWeekWidth / 2 - loadingLabel->width() / 2, this->y() + kLoadingAnimOffset );
		loadingLabel->show();
		loadingLabel->movie()->start();
	}
	else if( direction == eToLeftDirection )
	{
		loadingLabel->move( kWeekWidth / 2 - loadingLabel->width() / 2, this->y() + kLoadingAnimOffset );
		loadingLabel->show();
		loadingLabel->movie()->start();
	}
}
