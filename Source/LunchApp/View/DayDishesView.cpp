#include "DayDishesView.h"

#include <QScroller>
#include <QScrollEvent>
#include <QEvent>
#include <QDrag>
#include <QMimeData>

#include "Style.h"
#include "DayView.h"
#include "../Controller/Controller.h"


static const QString	kDishMimeType = "dish";
static const float		kBackgroundMarginRatio = 0.7f;


DayDishesView::DayDishesView( QWidget *parent, std::vector<Dish>& dishesVect, EMode mode /*= eNormalMode */)
	: QWidget( parent )
	, dishesVect( dishesVect )
	, mode( mode )
	, editBackgroundLabel( NULL )
{
// 	if( Controller::getUser()->getRole() == User::eRegular )
// 	{
		init();
// 	}
// 	else if( Controller::getUser()->getRole() == User::eAdmin )
// 	{
// 		initEditable();
// 	}
}

DayDishesView::~DayDishesView()
{

}

void DayDishesView::init()
{
	// Properties
	//QScroller::grabGesture( this, QScroller::MiddleMouseButtonGesture );

	if( mode == eEditMode )
	{
		this->setAcceptDrops( true );
	}

	// Size
	this->setMinimumSize( QSize( Style::getDishWidth() + 2 * Style::getDishSpacing(), 2 * (Style::getDishWidth() + Style::getDishSpacing()) ) );
	this->adjustSize();

	// Animation
	scrollAnimation = new QPropertyAnimation( this, "contentOffset" );
	scrollAnimation->setEasingCurve( QEasingCurve::OutCirc );
	scrollAnimation->setDuration( kWeekAnimationTime / 1.5f );

	// Objects
	addDishes();

	stackDishViews();

	this->adjustSize();

	if( mode == eEditMode )
	{
		editBackgroundLabel = new QLabel( this );
		editBackgroundLabel->setStyleSheet( kEditableDayStyleSheet );
		editBackgroundLabel->setFixedSize( this->size() - QSize( 2 * kBackgroundMarginRatio * Style::getDishSpacing(), 0 ) );
		editBackgroundLabel->move( Style::getDishSpacing() * kBackgroundMarginRatio, 0 );
		editBackgroundLabel->adjustSize();
	}
}

void DayDishesView::initEditable()
{
	// Placeholder
	dishViewsVect.push_back( new DishView( this ) );

	this->setMinimumWidth( this->width() + Style::getDishSpacing() );
	this->adjustSize();

	this->setMinimumSize( this->size() + QSize( 0, Style::getDishSpacing() ) );	// add bottom spacing so the shadow is rendered completely
	this->adjustSize();
}

void DayDishesView::mainWindowResized( QSize size )
{
	// Reposition dishes if they fit in the view
	if( mode == eEditMode || mode == eNormalMode )
	{
		this->setMinimumHeight( size.height() );
		this->adjustSize();
	}
	else if( dishViewsVect.size() > 0 )
	{
		QRect visibleRect = this->visibleRegion().boundingRect();

		if( visibleRect.height() >= this->size().height() );
		{
			int deltaY = -dishViewsVect[0]->y();
			for( int i = 0 ; i < dishViewsVect.size() ; i++ )
			{
				dishViewsVect[i]->move( dishViewsVect[i]->x(), dishViewsVect[i]->y() + deltaY );
			}
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

void DayDishesView::setContentOffset( QPoint offset )
{
	if( dishViewsVect.size() == 0 )
		return;

	internalContentOffset = offset;

	int deltaY = internalContentOffset.y() - dishViewsVect[0]->y();
	dishViewsVect[0]->move( dishViewsVect[0]->pos().x(), internalContentOffset.y() );

	// Offset all dish views
	for( size_t i = 1 ; i < dishViewsVect.size() ; i++ )
	{
		dishViewsVect[i]->move( dishViewsVect[i]->pos().x(), dishViewsVect[i]->pos().y() + deltaY );
	}
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

void DayDishesView::wheelEvent( QWheelEvent* wheelEvent )
{
	//return ((DayView*)this->parent())->wheelEvent( event );

	if( dishViewsVect.size() == 0 || scrollAnimation->state() == QAbstractAnimation::Running )
		return;

	if( wheelEvent->delta() > 0 )
	{
		if( dishViewsVect.front()->visibleRegion().boundingRect().height() == dishViewsVect.front()->height() )
			return;

		scrollAnimation->setStartValue( internalContentOffset);
		scrollAnimation->setEndValue( internalContentOffset + QPoint( 0, dishViewsVect.front()->height() + Style::getDishSpacing() ) );
		scrollAnimation->start();
	}
	else if( wheelEvent->delta() < 0 )
	{
		if( dishViewsVect.back()->visibleRegion().boundingRect().height() == dishViewsVect.back()->height() )
			return;

		scrollAnimation->setStartValue( internalContentOffset );
		scrollAnimation->setEndValue( internalContentOffset - QPoint( 0, dishViewsVect.front()->height() + Style::getDishSpacing() ) );
		scrollAnimation->start();
	}
}

void DayDishesView::mousePressEvent( QMouseEvent* mouseEvent )
{
	if( mode == eEditMode || mode == eBrowseMode )
	{
		// Get selected dish
		QWidget* child = this->childAt( mouseEvent->pos() );
		if( child == NULL )
			return;

		child = (QWidget*)(child->parent());

		DishView* selectedDishView = dynamic_cast<DishView*>( child );
		if( selectedDishView == NULL )
			return;

		const Dish& selectedDish = selectedDishView->getDish();

		// Start drag
		QDrag *drag = new QDrag( this );

		QMimeData *mimeData = new QMimeData();
		mimeData->setText( selectedDish.getName() );
		mimeData->setData( kDishMimeType, selectedDish );
		drag->setMimeData( mimeData );

		drag->setPixmap( selectedDishView->getScaledPixmap() );
		drag->setHotSpot( mouseEvent->pos() - child->pos() );

		Qt::DropAction dropAction = drag->exec();
	}
}

void DayDishesView::dragEnterEvent( QDragEnterEvent* event )
{
	if( event->mimeData()->hasFormat( kDishMimeType ) )
	{
		editBackgroundLabel->setStyleSheet( kEditableDayDragEnterStyleSheet );
		event->acceptProposedAction();
	}
}

void DayDishesView::dragLeaveEvent( QDragLeaveEvent* event )
{
	editBackgroundLabel->setStyleSheet( kEditableDayStyleSheet );
}

void DayDishesView::dropEvent( QDropEvent* event )
{
	const QMimeData* mimeData = event->mimeData();

	QString text = mimeData->text();
	Dish dish = mimeData->data( kDishMimeType );

	event->acceptProposedAction();

	addDish(dish);

	editBackgroundLabel->setStyleSheet( kEditableDayStyleSheet );
}

void DayDishesView::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent( event );

	if( editBackgroundLabel )
	{
		editBackgroundLabel->setFixedSize( this->size() - QSize( 2 * kBackgroundMarginRatio * Style::getDishSpacing(), 0 ) );
		editBackgroundLabel->adjustSize();
	}
}

void DayDishesView::addDish( const Dish& dish )
{
	DishView* dishView = new DishView( this, dish, mode != eNormalMode );
	dishViewsVect.push_back( dishView );

	// Place last
	if( dishViewsVect.size() > 1 )
	{
		DishView* dishBefore = dishViewsVect[ dishViewsVect.size() - 2 ];
		dishView->move( dishBefore->x(), dishBefore->y() + dishBefore->height() + Style::getDishSpacing() );
	}
	else
	{
		dishView->move( Style::getDishSpacing(), Style::getDishSpacing() * ( 1.f - kBackgroundMarginRatio ) );
	}

	dishView->show();

	this->adjustSize();
}

void DayDishesView::addDishes()
{
	for( size_t i = 0 ; i < dishesVect.size() ; i++ )
	{
		dishViewsVect.push_back( new DishView( this, dishesVect[i], mode != eNormalMode ) );
	}
}

bool compareDishViews( DishView* first, DishView* second )
{
	if( first->width() * first->height() > second->width() * second->height() )
		return true;
	else
		return false;
}

void DayDishesView::stackDishViews()
{
	if( dishViewsVect.size() == 0 )
		return;

	/* Stack images */
	// Sort by size
	std::sort( dishViewsVect.begin(), dishViewsVect.end(), compareDishViews );

	// Stack
	dishViewsVect[0]->move( Style::getDishSpacing(), 0 );

	int width = this->width();
	for( size_t i = 1 ; i < dishViewsVect.size() ; i++ )
	{
		DishView* lastPlaced = dishViewsVect[i - 1];
		DishView* current = dishViewsVect[i];

		int lastXCoord = lastPlaced->x() + lastPlaced->width() + Style::getDishSpacing();
		int lastYCoord = lastPlaced->y() + lastPlaced->height() + Style::getDishSpacing();

		int currentWidth = current->width();
		if( lastXCoord + current->width() <= width )
		{
			// Place besides
			current->move( lastXCoord, lastPlaced->y() );
		}
		else
		{
			// Place below
			current->move( Style::getDishSpacing(), lastYCoord );
		}
	}

	// Re-arrange model vect to keep correspondence with view vect
	for( size_t i = 0 ; i < dishesVect.size() ; i++ )
	{
		dishesVect[i] = dishViewsVect[i]->getDish();
	}
}
