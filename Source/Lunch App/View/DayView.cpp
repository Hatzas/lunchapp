#include "DayView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QLabel>

#include "Style.h"
#include "WeekView.h"
#include "DishView.h"


DayView::DayView( QWidget *parent, QString dayName )
	: QWidget( parent )
	, dayName( dayName )
{
	init();
}

DayView::~DayView()
{

}

void DayView::init()
{
	/* Create objects */
	QLabel* textLabel = new QLabel( this );
	textLabel->setText( dayName );
	textLabel->setFont( QFont( kFontName, 20 ) );
	textLabel->setStyleSheet( "background-color: rgba(255, 255, 255, 0);" );
	textLabel->adjustSize();

	DishView* image1 = new DishView( this, QPixmap("Resources\\mancare1.png") );
	DishView* image2 = new DishView( this, QPixmap("Resources\\mancare2.png") );

	// Move objects
	textLabel->move( (image1->width() - textLabel->width()) / 2.0f , 0 );
	image1->move( 0, textLabel->height() + 2 * kDishSpacing );
	image2->move( 0, image1->y() + image1->height() + kDishSpacing );

	this->adjustSize();
}

void DayView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((WeekView*)this->parent())->wheelEvent( wheelEvent );
}