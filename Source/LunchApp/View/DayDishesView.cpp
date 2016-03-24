#include "DayDishesView.h"

#include <QScroller>
#include <QScrollEvent>
#include <QEvent>

#include "Style.h"
#include "DayView.h"


DayDishesView::DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect )
	: QWidget( parent )
	, dishesVect( dishesVect )
{
	init();
}

DayDishesView::~DayDishesView()
{

}

void DayDishesView::init()
{
	QScroller::grabGesture( this, QScroller::MiddleMouseButtonGesture );

	AddDishes();

	this->setMinimumWidth( kDayWidth );
	this->adjustSize();

	StackDishViews();

	this->setMinimumSize( this->size() + QSize( kDishSpacing, 0 ) );	// add bottom spacing so the shadow is rendered completely
	this->adjustSize();
}

bool DayDishesView::event( QEvent* event )
{
	switch( event->type() )
	{
	case QEvent::ScrollPrepare:
		{
			QRect visibleRect = this->visibleRegion().boundingRect();
			QSize size = this->size();

			QScrollPrepareEvent *se = static_cast<QScrollPrepareEvent *>( event );
			se->setViewportSize( QSize( this->width(), visibleRect.height() ) );
			se->setContentPosRange( QRectF( 0, 0, size.width(), size.height() - visibleRect.height() ) );
			se->setContentPos( -dishViewsVect[0]->pos() );
			se->accept();
			return true;
		}
	case QEvent::Scroll: 
		{
			QScrollEvent *se = static_cast<QScrollEvent *>( event );
			QPointF scrollPos = -se->contentPos() - se->overshootDistance();

			int deltaY = scrollPos.y() - dishViewsVect[0]->y();
			for( int i = 0 ; i < dishViewsVect.size() ; i++ )
			{
				dishViewsVect[i]->move( dishViewsVect[i]->x(), dishViewsVect[i]->y() + deltaY );
			}

			return true;
		}
	default:
		break;
	}

	return QWidget::event( event );
}

void DayDishesView::wheelEvent( QWheelEvent* event )
{
	return ((DayView*)this->parent())->wheelEvent( event );
}

void DayDishesView::mainWindowResized( QResizeEvent * event )
{
	// Reposition dishes if they fit in the view
	QRect visibleRect = this->visibleRegion().boundingRect();
	QSize size = this->size();

	if( visibleRect.height() >= size.height() );
	{
		int deltaY = -dishViewsVect[0]->y();
		for( int i = 0 ; i < dishViewsVect.size() ; i++ )
		{
			dishViewsVect[i]->move( dishViewsVect[i]->x(), dishViewsVect[i]->y() + deltaY );
		}
	}
}

void DayDishesView::selectionChangedOn( const Dish& dish )
{
	// Disable other dishes from same course
	for( size_t i = 0 ; i < dishViewsVect.size() ; i++ )
	{
		if( dish.getName() != dishViewsVect[i]->getDish().getName()
			&& dish.getCourseNum() == dishViewsVect[i]->getDish().getCourseNum() )
		{
			dishViewsVect[i]->setDisabled( dish.getUserSelected() );
		}
	}

	((DayView*)this->parent())->selectionChangedOn( dish );

	// Update View
	update();
}

void DayDishesView::AddDishes()
{
	for( size_t i = 0 ; i < dishesVect.size() ; i++ )
	{
		dishViewsVect.push_back( new DishView( this, dishesVect[i] ) );
	}
}

bool compareDishViews( DishView* first, DishView* second )
{
	if( first->width() * first->height() > second->width() * second->height() )
		return true;
	else
		return false;
}

void DayDishesView::StackDishViews()
{
	if( dishViewsVect.size() == 0 )
		return;

	/* Stack images */
	// Sort by size
	std::sort( dishViewsVect.begin(), dishViewsVect.end(), compareDishViews );

	// Stack
	dishViewsVect[0]->move( kDishSpacing, 0 );

	int width = this->width();
	for( size_t i = 1 ; i < dishViewsVect.size() ; i++ )
	{
		DishView* lastPlaced = dishViewsVect[i - 1];
		DishView* current = dishViewsVect[i];

		int lastXCoord = lastPlaced->x() + lastPlaced->width() + kDishSpacing;
		int lastYCoord = lastPlaced->y() + lastPlaced->height() + kDishSpacing;

		int currentWidth = current->width();
		if( lastXCoord + current->width() <= width )
		{
			// Place besides
			current->move( lastXCoord, lastPlaced->y() );
		}
		else
		{
			// Place below
			current->move( kDishSpacing, lastYCoord );
		}
	}

	// Re-arrange model vect to keep correspondence with view vect
	for( size_t i = 0 ; i < dishViewsVect.size() ; i++ )
	{
		dishesVect[i] = dishViewsVect[i]->getDish();
	}
}