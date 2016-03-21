#include "DishView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPalette>
#include <QPainter>

#include "Style.h"
#include "DayView.h"


static const float			kClickMovement			= 2;
static const float			kBriefDetailsOffset		= 30;


DishView::DishView( QWidget *parent, const Dish& dish  )
	: QWidget( parent )
	, dish( dish )
	, disabled( false )
	, mousePressed( false )
{
	init();
}

DishView::~DishView()
{

}

void DishView::init()
{
	// Compute scale based on image size and desired dish width
	float scale = kDishWidth / dish.getPixmap().width();
	QSize widgetSize = dish.getPixmap().size() * scale;

	// Create B&W image
	QImage blackAndWhiteImg = dish.getPixmap().toImage();//new QImage( widgetSize, QImage::Format_Grayscale8 );
	for( int x = 0 ; x < blackAndWhiteImg.width() ; x++ )
	{
		for( int y = 0 ; y < blackAndWhiteImg.height() ; y++ )
		{
			int grayColor = qGray( blackAndWhiteImg.pixel( x, y ) );
			blackAndWhiteImg.setPixel( x, y, qRgb( grayColor, grayColor, grayColor ) );
		}
	}

	monochromePixmap = QPixmap::fromImage( blackAndWhiteImg );

	/* Create objects */
	imageLabel = new QLabel( this );
	imageLabel->setPixmap( dish.getPixmap() );
	imageLabel->setMinimumSize( widgetSize );
	imageLabel->setMaximumSize( widgetSize );
	imageLabel->adjustSize();

	ribbonLabel = new QLabel( this );
	ribbonLabel->setPixmap( GetRibbonByCourse( dish.getCourseNum() ) );
	ribbonLabel->adjustSize();

	detailsLabel = new QLabel( this );
	QString text = dish.getName() + " \n\n   Ingrediente: " + dish.getIngredients();	// Separate ingredients with 2 spaces
	detailsLabel->setText( text );
	detailsLabel->setWordWrap( true );
	detailsLabel->setFont( QFont( kFontName, 10 ) );
	detailsLabel->setAutoFillBackground( true );
	detailsLabel->setMinimumSize( widgetSize );
	detailsLabel->setMaximumSize( widgetSize );
	detailsLabel->setAlignment( Qt::AlignTop );
	detailsLabel->setStyleSheet( kDetailsOverlayStyleSheet );
	detailsLabel->adjustSize();

	/* Animations */
	detailsAnimation = new QPropertyAnimation( detailsLabel, "pos" );
	detailsAnimation->setEasingCurve( QEasingCurve::OutCirc );

	/* Effects */
	selectedEffect = new SelectedEffect( imageLabel );
	imageLabel->setGraphicsEffect( selectedEffect );

	/* Move objects */
	detailsLabel->move( 0, widgetSize.height() );

	/* Properties */
	this->setMouseTracking( true );
	imageLabel->setMouseTracking( true );
	detailsLabel->setMouseTracking( true );

	this->setMinimumSize( widgetSize );
	this->setMaximumSize( widgetSize );
	
	this->adjustSize();
}

QPixmap DishView::GetRibbonByCourse( int courseNum )
{
	if( courseNum == 1 )
		return QPixmap( "Resources//ribbon1.png" );
	else if( courseNum == 2 )
		return QPixmap( "Resources//ribbon2.png" );

	return QPixmap( "Resources//ribbon3.png" );
}

void DishView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((DayView*)this->parent())->wheelEvent( wheelEvent );
}

void DishView::enterEvent( QEvent* event )
{
	if( disabled )
		return;

	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	// Start text animation
	detailsAnimation->setStartValue( detailsLabel->pos() );
	detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() - kBriefDetailsOffset ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();
}

void DishView::leaveEvent( QEvent* event )
{
	if( disabled )
		return;

	if( mousePressed )
	{
		mousePressed = false;

		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );
	}

	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	detailsAnimation->setStartValue( detailsLabel->pos() );
	detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();
}

void DishView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
	if( disabled )
		return;

	// Show full details if mouse over them
	if( ( this->childAt( mouseEvent->pos() ) == detailsLabel ) && ( detailsAnimation->state() != QAbstractAnimation::Running ) )
	{
		detailsAnimation->setStartValue( detailsLabel->pos() );
		detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), 0.0f ) );
		
		detailsAnimation->setDuration( kFullDishDetailsAnimationTime );
		detailsAnimation->start();
	}
}

void DishView::mousePressEvent( QMouseEvent* mouseEvent )
{
	if( disabled )
		return;

	mousePressed = true;

	this->move( this->pos() + QPoint( kClickMovement, kClickMovement ) );
}

void DishView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
	if( disabled )
		return;

	if( mousePressed )
	{
		mousePressed = false;

		dish.setSelected( !dish.isSelected() );
		((DayView*)parent())->SelectionChangedOn( dish );

		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );
	}

	if( dish.isSelected() )
	{
		//selectedEffect->enable();
		imageLabel->setStyleSheet( kSelectedStyleSheet );

		// Drop shadow is computation intensive and reduces performance
		QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect( this );
		dropShadow->setOffset( 0.0f );
		dropShadow->setBlurRadius( kSelectedShadowSize );
		dropShadow->setColor( kSelectedGlowColor );
		this->setGraphicsEffect( dropShadow );
	}
	else
	{
		//selectedEffect->disable();
		imageLabel->setStyleSheet("");

		this->setGraphicsEffect( NULL );
	}
}

void DishView::setDisabled( bool disabled )
{
	this->disabled = disabled;

	if( disabled )
	{
		// Show disabled effect
// 		QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect( this );
// 		this->setGraphicsEffect( blurEffect );

// 		QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect( this );
// 		colorizeEffect->setColor( kDisabledColor );
// 		imageLabel->setGraphicsEffect( colorizeEffect );

		imageLabel->setPixmap( monochromePixmap );
	}
	else
	{
//		this->setGraphicsEffect( NULL );

		imageLabel->setPixmap( dish.getPixmap() );
	}
}