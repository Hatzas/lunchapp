#include "WeekView.h"

#include <QTimer>

#include "Style.h"
#include "AllWeeksView.h"
#include "NotificationWindow.h"

WeekView::WeekView( QWidget *parent, const Week& week, bool editMode /*= false*/ )
	: QWidget(parent)
	, week( week )
	, editMode( editMode )
{
	init();
}

WeekView::~WeekView()
{

}

void WeekView::init()
{
    /* Properties */
    this->setAttribute( Qt::WA_NoSystemBackground, true );

	/* Create objects */
	addDays();

	this->adjustSize();
}

bool WeekView::event( QEvent *event )
{
	return QWidget::event( event );
}

void WeekView::wheelEvent( QWheelEvent* wheelEvent )
{
	return QWidget::wheelEvent( wheelEvent );
}

void WeekView::mainWindowResized( QSize size )
{
    for( size_t i = 0 ; i < dayViewsVect.size() ; i++ )
	{
		dayViewsVect[i]->mainWindowResized( size );
	}

	this->adjustSize();
}

void WeekView::selectionChangedOn( const Dish& dish )
{
	((AllWeeksView*)this->parent())->selectionChangedOn( dish );
}

void WeekView::addDays()
{
    std::vector<Day>& daysVect = week.getDays();

	for( size_t i = 0 ; i < daysVect.size() ; i++ )
	{
		DayView* dayView = new DayView( this, daysVect[i], editMode ? eEditMode : eNormalMode );

		if( i > 0 )
			dayView->move( dayViewsVect[i-1]->pos().x() + dayViewsVect[i-1]->width() - Style::getDishSpacing() + Style::getDaySpacing(), 0 );		// "- Style::getDishSpacing()" is a temporary patch to fix double spacing between days

		dayViewsVect.push_back( dayView );
	}
}
