#include "AllWeeksView.h"

#include <QMoveEvent>
#include <QMovie>

#include "Style.h"
#include "MainWindow.h"


static const float		kScrollOwershootRatio           = 1 / 8.f;

#ifdef Q_OS_ANDROID
static const QSize		kLoadingAnimSize				= QSize( 48, 48 );
#else
static const QSize		kLoadingAnimSize				= QSize( 24, 24 );
#endif


AllWeeksView::AllWeeksView( QWidget *parent, bool editMode /*= false*/ )
	: QWidget(parent)
	, editMode( editMode )
	, windowWidth( Style::getWeekWidth() )
{
	init();
}

AllWeeksView::~AllWeeksView()
{
}

void AllWeeksView::init()
{
    /* Properties */
    this->setAttribute( Qt::WA_NoSystemBackground, true );

	// Loading gif
	loadingLabel = new QLabel( this );
	loadingLabel->setMovie( new QMovie( RESOURCES_ROOT"loader1.gif" ) );
	loadingLabel->movie()->setScaledSize( kLoadingAnimSize );
	loadingLabel->setMinimumSize( kLoadingAnimSize );
	loadingLabel->adjustSize();
	loadingLabel->setAttribute( Qt::WA_NoSystemBackground, true );
	loadingLabel->hide();

	// Animations
	forwardAnimation = new QPropertyAnimation( this, "pos" );
	forwardAnimation->setEasingCurve( QEasingCurve::OutCirc );
	forwardAnimation->setDuration( kWeekAnimationTime / 3 );

	backAnimation = new QPropertyAnimation( this, "pos" );
	backAnimation->setEasingCurve( QEasingCurve::OutCirc );
	backAnimation->setDuration( kWeekAnimationTime / 1.2f );

	firstLastAnimations = new QSequentialAnimationGroup();
	firstLastAnimations->addAnimation( forwardAnimation );
	firstLastAnimations->addAnimation( backAnimation );

	if( editMode )
	{
		addEmptyWeek();
	}
	else
	{
		// Request data
		QDate startDate = QDate(2016, 4, 18);
		QDate endDate = QDate(2016, 4, 22);
        emit ((MetroView*)this->parent())->requestWeek( startDate, endDate );		// current week

		showLoadingAnim( true, eHere );
	}
}

bool AllWeeksView::event( QEvent *event )
{
	return QWidget::event( event );
}

void AllWeeksView::wheelEvent( QWheelEvent* wheelEvent )
{
	return QWidget::wheelEvent( wheelEvent );
}

void AllWeeksView::mainWindowResized( QSize size )
{
	windowWidth = qMax<int>( (int)Style::getWeekWidth(), size.width() );
	centerWeekViews();

    for( size_t i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		weekViewsVect[i]->mainWindowResized( size );
	}

	this->adjustSize();
}

void AllWeeksView::selectionChangedOn( const Dish& dish )
{
	emit ((MetroView*)this->parent())->selectionChangedOn( dish );
}

void AllWeeksView::addWeek( const Week& week )
{
	showLoadingAnim( false );

	if( week.isEmpty() )
	{
		if( weekViewsVect.size() == 0 )
		{
			return;
		}

		if( week < weekViewsVect.front()->getWeek() )
		{
			weekViewsVect.front()->getWeek().setFirstAvailable( true );
		}
		else if( week > weekViewsVect.front()->getWeek() )
		{
			weekViewsVect.back()->getWeek().setLastAvailable( true );
		}

		return;
	}

	WeekView* weekView = new WeekView( this, week );

	// Position according to date
	int centerX = ( windowWidth - weekView->width() ) / 2;
	if( weekViewsVect.size() == 0 )
	{
		weekViewsVect.push_back( weekView );
	}
	else if( week < weekViewsVect[0]->getWeek() )
	{
		// Insert before first
		weekView->move( centerX, 0 );

		weekViewsVect.insert( weekViewsVect.begin(), weekView );
	}
	else if( week > weekViewsVect[ weekViewsVect.size() - 1 ]->getWeek() )
	{
		// Insert after last
		weekView->move( weekViewsVect.size() * windowWidth + centerX, 0 );

		weekViewsVect.push_back( weekView );
	}

	weekView->show();

	if( weekViewsVect.size() == 1 )
	{
		increaseSize( eRightDirection );
		showLoadingAnim( true, eRightDirection );

		QDate nextWeekStartDate = week.getStartDate().addDays( 7 );
		QDate nextWeekEndDate = week.getEndDate().addDays( 7 );
		emit ((MetroView*)this->parent())->requestWeek( nextWeekStartDate, nextWeekEndDate );			// next week
	}
	else if( weekViewsVect.size() == 2 )
	{
		increaseSize( eLeftDirection );
		showLoadingAnim( true, eLeftDirection );

		QDate prevWeekStartDate = weekViewsVect.front()->getWeek().getStartDate().addDays( -7 );
		QDate prevWeekEndDate = weekViewsVect.front()->getWeek().getEndDate().addDays( -7 );
		emit ((MetroView*)this->parent())->requestWeek( prevWeekStartDate, prevWeekEndDate );			// previous week
	}

	this->adjustSize();

	centerWeekViews();
}

void AllWeeksView::addEmptyWeek()
{
	// Create Week
	weeksVect.push_back( Week() );
	Week& week = weeksVect.back();

	// Next week date
	QDate mondayAfter = QDate::currentDate().addDays( 8 - QDate::currentDate().dayOfWeek() );
	QDate fridayAfter = mondayAfter.addDays( 4 );
	week.setStartDate( mondayAfter );
	week.setEndDate( fridayAfter );

	std::vector<Day> emptyDaysVect;
	emptyDaysVect.push_back( Day( "luni" ) );
	emptyDaysVect.push_back( Day( "marti" ) );
	emptyDaysVect.push_back( Day( "miercuri" ) );
	emptyDaysVect.push_back( Day( "joi" ) );
	emptyDaysVect.push_back( Day( "vineri" ) );
	week.setDays( emptyDaysVect );

	// Create view
	WeekView* weekView = new WeekView( this, week, editMode );

	// Position according to date
	int centerX = ( windowWidth - weekView->width() ) / 2;
	if( weekViewsVect.size() == 0 )
	{
		weekViewsVect.push_back( weekView );
	}
	else if( week < weekViewsVect[0]->getWeek() )
	{
		// Insert before first
		weekView->move( centerX, 0 );

		weekViewsVect.insert( weekViewsVect.begin(), weekView );
	}
	else if( week > weekViewsVect[ weekViewsVect.size() - 1 ]->getWeek() )
	{
		// Insert after last
		weekView->move( weekViewsVect.size() * windowWidth + centerX, 0 );

		weekViewsVect.push_back( weekView );
	}

	weekView->show();

	this->adjustSize();
}

Week AllWeeksView::getVisibleWeek()
{	
    for( size_t i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		if( weekViewsVect[i]->visibleRegion().boundingRect().width() == weekViewsVect[i]->width() )
		{
			return weekViewsVect[i]->getWeek();
		}
	}

	return Week();
}

Week AllWeeksView::getWeek( int index )
{
    if( index < (int)weekViewsVect.size() )
		return weekViewsVect[ index ]->getWeek();

	return Week();
}

bool AllWeeksView::scrollStarted( EDirection direction )
{
	// If loading or animating or no weeks, do nothing
	if( weekViewsVect.size() == 0 || loadingLabel->isVisible() || firstLastAnimations->state() == QAbstractAnimation::Running )
		return false;

	WeekView* firstWeek = weekViewsVect[ 0 ];
	WeekView* secondWeek = weekViewsVect[ 1 ];
	WeekView* preLastWeek = weekViewsVect[ weekViewsVect.size() - 2 ];
	WeekView* lastWeek = weekViewsVect[ weekViewsVect.size() - 1 ];

	if( direction == eLeftDirection )
	{
		// If no more weeks before
		if( firstWeek->visibleRegion().boundingRect().width() == firstWeek->width() 
			&& firstWeek->getWeek().isFirstAvailable() )
		{
			// Show animation
			forwardAnimation->setStartValue( this->pos() );
			forwardAnimation->setEndValue( this->pos() + QPoint( Style::getWeekWidth() * kScrollOwershootRatio, 0 ) );
			backAnimation->setStartValue( forwardAnimation->endValue() );
			backAnimation->setEndValue( this->pos() );

			firstLastAnimations->start();

			return false;
		}

		if( secondWeek->visibleRegion().boundingRect().width() == secondWeek->width() )
		{
			// Increase view size and reposition to accommodate new view
			increaseSize( direction );
			showLoadingAnim( true, direction );

			// Request week before the first
			emit ((MetroView*)this->parent())->requestWeekBefore( weekViewsVect[0]->getWeek() );
		}
	}
	else if( direction == eRightDirection )
	{
		// If no more weeks after
		if( lastWeek->visibleRegion().boundingRect().width() == lastWeek->width()
			&& lastWeek->getWeek().isLastAvailable() )
		{
			// Show animation
			forwardAnimation->setStartValue( this->pos() );
			forwardAnimation->setEndValue( this->pos() - QPoint( Style::getWeekWidth() * kScrollOwershootRatio, 0 ) );
			backAnimation->setStartValue( forwardAnimation->endValue() );
			backAnimation->setEndValue( this->pos() );

			firstLastAnimations->start();

			return false;
		}

		if( preLastWeek->visibleRegion().boundingRect().width() == preLastWeek->width() )
		{
			// Increase view size and reposition to accommodate new view
			increaseSize( direction );
			showLoadingAnim( true, direction );

			// Request week after the last
			emit ((MetroView*)this->parent())->requestWeekAfter( weekViewsVect[ weekViewsVect.size()-1 ]->getWeek() );
		}
	}

    weekViewsVect[ weekViewsVect.size()-1 ]->visibleRegion().boundingRect();

	return true;
}

void AllWeeksView::increaseSize( EDirection direction )
{
	this->setMinimumWidth( this->width() + windowWidth );

	if( direction == eLeftDirection )
	{
		this->move( this->x() - windowWidth, this->y() );

		// Reposition all weeks so visually they remain the same
        for( size_t i = 0 ; i < weekViewsVect.size() ; i++ )
		{
			weekViewsVect[i]->move( weekViewsVect[i]->x() + windowWidth, weekViewsVect[i]->y() );
		}
	}

	this->adjustSize();
}

void AllWeeksView::centerWeekViews()
{
	// Get visible week
	int visibleWeekIdx = 0;
    for( size_t i = 1 ; i < weekViewsVect.size() ; i++ )
	{
		if( weekViewsVect[i]->visibleRegion().boundingRect().width() == weekViewsVect[i]->width() )
		{
			visibleWeekIdx = i;
		}
	}

	// Increase size
	this->setMinimumWidth( weekViewsVect.size() * windowWidth );
	this->adjustSize();

	// Re-position weeks taking into account new spacing
    for( size_t i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		int centerX = ( windowWidth - weekViewsVect[i]->width() ) / 2;
		weekViewsVect[i]->move( windowWidth * i + centerX, 0 );
	}

	// Move so same week remains visible
	this->move( visibleWeekIdx * -windowWidth, this->y() );
}

void AllWeeksView::showLoadingAnim( bool show, EDirection direction )
{
	if( !show )
	{
		loadingLabel->movie()->stop();
		loadingLabel->hide();

		return;
	}

	QSize parentSize = ((QWidget*)((QWidget*)parent())->parent())->size();
	if( direction == eRightDirection )
	{
		loadingLabel->move( this->width() - windowWidth / 2 - loadingLabel->width() / 2, parentSize.height() / 3 );
	}
	else if( direction == eLeftDirection )
	{
		loadingLabel->move( windowWidth / 2 - loadingLabel->width() / 2, parentSize.height() / 3 );
	}
	else
	{
		loadingLabel->move( windowWidth / 2 - loadingLabel->width() / 2, parentSize.height() / 2 );
	}

	loadingLabel->show();
	loadingLabel->movie()->start();
}
