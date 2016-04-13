#include "DayView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QScroller>

#include "Style.h"
#include "WeekView.h"
#include "DishView.h"


DayView::DayView( QWidget *parent, Day& day, EMode mode /*= eNormalMode*/ )
	: QWidget( parent )
	, day( day )
	, name( day.getName() )
	, dishesVect( day.getDishes() )
	, mode( mode )
{
	init();
}

DayView::DayView( QWidget *parent, QString name, std::vector<Dish>& dishes, EMode mode /*= eNormalMode*/ )
	: QWidget( parent )
	, name( name )
	, dishesVect( dishes )
	, mode( mode )
{
	init();
}

DayView::~DayView()
{

}

void DayView::init()
{
    // Properties
    this->setAttribute( Qt::WA_NoSystemBackground, true );

	/* Create objects */
	if( mode == eBrowseMode )
	{
		// Add search field
	}

	dayNameLabel = new QLabel( this );
	dayNameLabel->setText( name );
	dayNameLabel->setFont( QFont( kFontName, 20 ) );
	dayNameLabel->setStyleSheet( "background-color: rgba(255, 255, 255, 0);" );
	dayNameLabel->adjustSize();

	dishesView = new DayDishesView( this, dishesVect, mode );

	/* Move and resize objects */
	dishesView->move( 0, dayNameLabel->height() + 2 * Style::getDishSpacing() );

	this->adjustSize();

	dayNameLabel->move( Style::getDishSpacing() + ( this->width() - dayNameLabel->width() ) / 2.0f , 0 );

// 	this->setMinimumSize( this->size() + QSize( 0, Style::getDishSpacing() ) );	// add bottom spacing so the shadow is rendered completely
// 	this->adjustSize();
}

void DayView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((WeekView*)this->parent())->wheelEvent( wheelEvent );
}

void DayView::mainWindowResized( QSize size )
{
	dishesView->mainWindowResized( size );
	this->adjustSize();
}

void DayView::selectionChangedOn( const Dish& dish )
{
	((WeekView*)this->parent())->selectionChangedOn( dish );
}
