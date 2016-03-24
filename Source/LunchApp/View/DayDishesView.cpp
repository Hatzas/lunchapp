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
			se->setContentPos( -disheViewsVect[0]->pos() );
			se->accept();
			return true;
		}
	case QEvent::Scroll: 
		{
			QScrollEvent *se = static_cast<QScrollEvent *>( event );
			QPointF scrollPos = -se->contentPos() - se->overshootDistance();

			disheViewsVect[0]->move( disheViewsVect[0]->x(), scrollPos.y() );
			for( int i = 1 ; i < disheViewsVect.size() ; i++ )
			{
				disheViewsVect[i]->move( disheViewsVect[i]->x(), disheViewsVect[i-1]->y() + disheViewsVect[i-1]->height() + kDishSpacing );
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
		disheViewsVect[0]->move( kDishSpacing, 0 );
		for( int i = 1 ; i < disheViewsVect.size() ; i++ )
		{
			disheViewsVect[i]->move( kDishSpacing, disheViewsVect[i-1]->y() + disheViewsVect[i-1]->height() + kDishSpacing );
		}
	}
}

void DayDishesView::selectionChangedOn( const Dish& dish )
{
	// Disable other dishes from same course
	for( size_t i = 0 ; i < dishesVect.size() ; i++ )
	{
		if( dish.getName() != dishesVect[i].getName()
			&& dishesVect[i].getCourseNum() == dish.getCourseNum() )
		{
			disheViewsVect[i]->setDisabled( dish.getUserSelected() );
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
		disheViewsVect.push_back( new DishView( this, dishesVect[i] ) );
	}
}

void DayDishesView::StackDishViews()
{
	if( disheViewsVect.size() == 0 )
		return;

	disheViewsVect[0]->move( kDishSpacing, kDishSpacing );
	for( size_t i = 1 ; i < disheViewsVect.size() ; i++ )
	{
		disheViewsVect[i]->move( kDishSpacing, disheViewsVect[i-1]->y() + disheViewsVect[i-1]->height() + kDishSpacing );
	}

	// Stack images

	// Sort by size
	// TO DO

	// Stack
	// TO DO
}