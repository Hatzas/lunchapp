#include "DishView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPalette>
#include <QPainter>
#include <QRubberBand>

#include "Style.h"
#include "DayDishesView.h"


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
	QSize baseWidgetSize = dish.getPixmap().size() * scale;

	// Size
	resizeByUserPreference( baseWidgetSize );

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
	dishPixmap = dish.getPixmap();

	/* Create objects */
	imageLabel = new QLabel( this );
	imageLabel->setPixmap( dishPixmap );
	imageLabel->setFixedSize( this->size() );
	imageLabel->setAlignment( Qt::AlignCenter );
	imageLabel->adjustSize();

//	QRubberBand* cropBand = new QRubberBand( QRubberBand::Rectangle, imageLabel );
//	cropBand->setGeometry( QRect( ( baseWidgetSize.width() - this->width() ) / 2, ( baseWidgetSize.height() - this->height() ) / 2, this->width(), this->height() ) );

	ribbonLabel = new QLabel( this );
	ribbonLabel->setPixmap( getRibbonByCourse( dish.getCourseNum() ) );
	ribbonLabel->adjustSize();
	ribbonLabel->setScaledContents( true );

	detailsLabel = new QLabel( this );
	QString text = dish.getName() + " \n\nIngrediente: " + dish.getIngredients();	// Separate ingredients with 2 spaces
	detailsLabel->setText( text );
	detailsLabel->setWordWrap( true );
	detailsLabel->setFont( QFont( kFontName, 10 ) );
	detailsLabel->setAutoFillBackground( true );
	detailsLabel->setFixedSize( this->size() );
	detailsLabel->setAlignment( Qt::AlignTop );
	detailsLabel->setStyleSheet( kDetailsOverlayStyleSheet );
	detailsLabel->adjustSize();

	ratingView = new DishRatingView( this, dish );

	/* Animations */
	detailsAnimation = new QPropertyAnimation( detailsLabel, "pos" );
	detailsAnimation->setEasingCurve( QEasingCurve::OutCirc );

	ratingViewAnimation = new QPropertyAnimation( ratingView, "pos" );
	ratingViewAnimation->setEasingCurve( QEasingCurve::OutCirc );
	ratingViewAnimation->setDuration( kBirefDishDetailsAnimationTime );

	/* Effects */
	selectedEffect = new SelectedEffect( imageLabel );
	imageLabel->setGraphicsEffect( selectedEffect );

	// Size tweaks based on user interest
	if( dish.getUserInterest() == Dish::eLow )
	{
		ribbonLabel->setFixedSize( ribbonLabel->size() / 1.5f );

		monochromePixmap = monochromePixmap.scaled( monochromePixmap.size() / 1.5f, Qt::KeepAspectRatio );
		dishPixmap = dishPixmap.scaled( dishPixmap.size() / 1.5f, Qt::KeepAspectRatio );
		imageLabel->setPixmap( dishPixmap );
	}

	/* Move objects */
	ratingView->move( this->width(), this->height() - kBriefDetailsOffset - ratingView->height() - 2 );
	detailsLabel->move( 0, this->height() );

	/* Properties */
	this->setMouseTracking( true );
	imageLabel->setMouseTracking( true );
	detailsLabel->setMouseTracking( true );
}

QPixmap DishView::getRibbonByCourse( int courseNum )
{
	if( courseNum == 1 )
		return QPixmap( "Resources//ribbon1.png" );
	else if( courseNum == 2 )
		return QPixmap( "Resources//ribbon2.png" );

	return QPixmap( "Resources//ribbon3.png" );
}

void DishView::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((DayDishesView*)this->parent())->wheelEvent( wheelEvent );
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
	if( disabled || mouseEvent->button() != Qt::LeftButton )
		return;

	mousePressed = true;

	this->move( this->pos() + QPoint( kClickMovement, kClickMovement ) );
}

void DishView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
	if( disabled || !mousePressed || mouseEvent->button() != Qt::LeftButton )
		return;

	mousePressed = false;

	dish.setUserSelected( !dish.getUserSelected() );
	((DayDishesView*)parent())->selectionChangedOn( dish );

	this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );

	setSelected( dish.getUserSelected() );
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

		imageLabel->setPixmap( dishPixmap );
	}
}

void DishView::setSelected( bool selected )
{
	if( selected )
	{
		// Show effects
		//selectedEffect->enable();
		imageLabel->setStyleSheet( kSelectedStyleSheet );

		// Drop shadow is computation intensive and reduces performance
// 		QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect( this );
// 		dropShadow->setOffset( 0.0f );
// 		dropShadow->setBlurRadius( kSelectedShadowSize );
// 		dropShadow->setColor( kSelectedGlowColor );
// 		this->setGraphicsEffect( dropShadow );

		// Show ratings view
		ratingViewAnimation->stop();
		ratingViewAnimation->setStartValue( ratingView->pos() );
		ratingViewAnimation->setEndValue( QPointF( this->width() - ratingView->width(), ratingView->pos().y() ) );
		ratingViewAnimation->start();
	}
	else
	{
		// Show effects
		//selectedEffect->disable();
		imageLabel->setStyleSheet("");

//		this->setGraphicsEffect( NULL );

		// Hide ratings view
		ratingViewAnimation->stop();
		ratingViewAnimation->setStartValue( ratingView->pos() );
		ratingViewAnimation->setEndValue( QPointF( this->width(), ratingView->pos().y() ) );
		ratingViewAnimation->start();
	}
}

void DishView::resizeByUserPreference( QSize baseSize )
{
	this->setFixedSize( baseSize );
	this->adjustSize();

	if( dish.getUserInterest() == Dish::eHigh )
	{
		// Leave as is
	}
	else if( dish.getUserInterest() == Dish::eMedium )
	{
		// Crop half height
		int newHeight = this->height() / 2;
		this->setFixedHeight( newHeight );
		this->adjustSize();
	}
	else if( dish.getUserInterest() == Dish::eLow )
	{
		// Crop half height
		QSize newSize = (this->size() - QSize( kDishSpacing, 0 )) / 2;
		this->setFixedSize( newSize );
		this->adjustSize();
	}
}