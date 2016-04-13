#include "WeekView.h"

#include <QTimer>

#include "Style.h"
#include "AllWeeksView.h"
#include "NotificationWindow.h"

static const QString	kDayNames[] = { "Luni", "Marti", "Miercuri", "Joi", "Vineri" };


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
	AddDays();

	this->adjustSize();

	// Start menu notification timer
	int timeToLunch = QTime::currentTime().msecsTo( kDayMenuNotificationTime );
	if( timeToLunch < 0 )	// if time has passed
	{
		timeToLunch += QTime( 23, 59, 9999 ).msec();
	}

	QTimer::singleShot( timeToLunch, this, SLOT( showDayMenuNotification() ) );

	QTimer::singleShot( 10000, this, SLOT( showDayMenuNotification() ) );			// for development only
}

void WeekView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((AllWeeksView*)this->parent())->wheelEvent( wheelEvent );
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

void WeekView::showDayMenuNotification()
{
	int day = QDate::currentDate().dayOfWeek() - 1;
    if( week.getDays().size() <= (size_t)day )				// not all days have menus
		return;

	Day currentDay;
	for( int i = 0 ; i < 5 ; i++ )
	{
		if( week.getDays()[ i ].getName() == kDayNames[ day ] )
		{
			currentDay = week.getDays()[ i ];
			break;
		}
	}
	if( currentDay.getDishes().size() == 0  )		// current day doesn't have a menu
		return;

	std::vector<QPixmap> dishesPixmaps;
	QString todayMenu;
    for( size_t i = 0 ; i < currentDay.getDishes().size() ; i++ )
	{
		if( currentDay.getDishes()[i].getUserSelected() )
		{
			todayMenu = "Azi ai:\n" + currentDay.getDishes()[i].getName() + " ( " + currentDay.getDishes()[i].getIdentifier() + " )";

			dishesPixmaps.clear();
			dishesPixmaps.push_back( currentDay.getDishes()[i].getPixmap() );

			(new NotificationWindow( todayMenu, dishesPixmaps, kMenuNotificationShowTime ))->show();
		}
	}
}

void WeekView::AddDays()
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
