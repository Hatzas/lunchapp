#include "AllWeeksView.h"

#include <QMoveEvent>
#include <QMovie>

#include "Style.h"
#include "MainWindow.h"
#include "MetroView.h"


static const float		kWeekElasticWidth		= 200;


AllWeeksView::AllWeeksView(QWidget *parent)
	: QWidget(parent)
	, windowWidth( Style::getWeekWidth() )
{
	init();
}

AllWeeksView::~AllWeeksView()
{
}

void AllWeeksView::init()
{
	// Loading gif
	loadingLabel = new QLabel( this );
	loadingLabel->setMovie( new QMovie( "Resources/loader1.gif" ) );
	loadingLabel->movie()->setScaledSize( kLoadingAnimSize );
	loadingLabel->setMinimumSize( kLoadingAnimSize );
	loadingLabel->adjustSize();
	loadingLabel->hide();

	// Animations
	forwardAnimation = new QPropertyAnimation( this, "pos" );
	forwardAnimation->setEasingCurve( QEasingCurve::OutCirc );
	forwardAnimation->setDuration( kWeekAnimationTime / 2 );

	backAnimation = new QPropertyAnimation( this, "pos" );
	backAnimation->setEasingCurve( QEasingCurve::OutCirc );
	backAnimation->setDuration( kWeekAnimationTime / 1.2f );

	firstLastAnimations = new QSequentialAnimationGroup();
	firstLastAnimations->addAnimation( forwardAnimation );
	firstLastAnimations->addAnimation( backAnimation );
}

void AllWeeksView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}

void AllWeeksView::mainWindowResized( QResizeEvent* event )
{
	auto eventWidth = event->size().width();
	windowWidth = qMax<int>( (int)(Style::getWeekWidth() * Style::getWindowScale()), event->size().width() );

	centerWeekViews();

	for( int i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		weekViewsVect[i]->mainWindowResized( event );
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
	for( int i = 0 ; i < weekViewsVect.size() ; i++ )
	{
		if( weekViewsVect[i]->visibleRegion().boundingRect().width() == weekViewsVect[i]->width() )
		{
			return weekViewsVect[i]->getWeek();
		}
	}

	return Week();
}

bool AllWeeksView::scrollStarted( EDirection direction )
{
	// If loading or animating or no weeks, do nothing
	if( weekViewsVect.size() == 0 || loadingLabel->isVisible() || firstLastAnimations->state() == QAbstractAnimation::Running )
		return false;

	WeekView* firstWeek = weekViewsVect[ 0 ];
	WeekView* lastWeek = weekViewsVect[ weekViewsVect.size()-1 ];

	if( direction == eToLeftDirection && firstWeek->visibleRegion().boundingRect().width() == firstWeek->width() )
	{
		// If no more weeks before
		if( firstWeek->getWeek().isFirstAvailable() )
		{
			// Show animation
			forwardAnimation->setStartValue( this->pos() );
			forwardAnimation->setEndValue( this->pos() + QPoint( Style::getWeekWidth() / 5, 0 ) );
			backAnimation->setStartValue( forwardAnimation->endValue() );
			backAnimation->setEndValue( this->pos() );

			firstLastAnimations->start();

			return false;
		}

		// Increase view size and reposition to accommodate new view
		increaseSize( direction );
		showLoadingAnim( true, direction );

		// Request week before the first
		emit ((MetroView*)this->parent())->requestWeekBefore( weekViewsVect[0]->getWeek() );
	}
	else if( direction == eToRightDirection && lastWeek->visibleRegion().boundingRect().width() == lastWeek->width() )
	{
		// If no more weeks after
		if( lastWeek->getWeek().isLastAvailable() )
		{
			// Show animation
			forwardAnimation->setStartValue( this->pos() );
			forwardAnimation->setEndValue( this->pos() - QPoint( Style::getWeekWidth() / 5, 0 ) );
			backAnimation->setStartValue( forwardAnimation->endValue() );
			backAnimation->setEndValue( this->pos() );

			firstLastAnimations->start();

			return false;
		}

		// Increase view size and reposition to accommodate new view
		increaseSize( direction );
		showLoadingAnim( true, direction );

		// Request week after the last
		emit ((MetroView*)this->parent())->requestWeekAfter( weekViewsVect[ weekViewsVect.size()-1 ]->getWeek() );
	}

	QRect visibleRect = weekViewsVect[ weekViewsVect.size()-1 ]->visibleRegion().boundingRect();

	return true;
}

void AllWeeksView::increaseSize( EDirection direction )
{
	this->setMinimumWidth( this->width() + windowWidth );

	if( direction == eToLeftDirection )
	{
		this->move( this->x() - windowWidth, this->y() );

		// Reposition all weeks so visually they remain the same
		for( int i = 0 ; i < weekViewsVect.size() ; i++ )
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
	for( int i = 1 ; i < weekViewsVect.size() ; i++ )
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
	for( int i = 0 ; i < weekViewsVect.size() ; i++ )
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

	if( direction == eToRightDirection )
	{
		loadingLabel->move( this->width() - windowWidth / 2 - loadingLabel->width() / 2, this->y() + kLoadingAnimOffset );
		loadingLabel->show();
		loadingLabel->movie()->start();
	}
	else if( direction == eToLeftDirection )
	{
		loadingLabel->move( windowWidth / 2 - loadingLabel->width() / 2, this->y() + kLoadingAnimOffset );
		loadingLabel->show();
		loadingLabel->movie()->start();
	}
}
