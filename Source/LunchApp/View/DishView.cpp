#include "DishView.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPalette>
#include <QPainter>
#include <QRubberBand>
#include <QComboBox>
#include <QBitmap>
#include <QPainterPath>
#include <QDebug>

#include "Style.h"
#include "MainWindow.h"

static const int			kAtEnd					= 0xffffff;
static const float			kClickMovement			= 2;
static const float			kBriefDetailsOffset		= 30;

#ifdef Q_OS_ANDROID
static const int			kIdentifierFontSize		= 5;
static const int			kDetailsFontSize		= 10;
static const float			kPointToPixel			= 7.f;
#else
static const int			kIdentifierFontSize		= 10;
static const int			kDetailsFontSize		= 10;
static const float			kPointToPixel			= 2.5f;
#endif

DishView::DishView( QWidget *parent )
	: QWidget( parent )
	, isPlaceholder( true )
{
	initPlaceholder();
}

DishView::DishView( QWidget *parent, const Dish& dish, bool editMode /*= false*/ )
	: QWidget( parent )
	, dish( dish )
	, disabled( false )
	, isPlaceholder( false )
	, editMode( editMode )
	, mousePressed( false )
{
	init();
}

DishView::~DishView()
{

}

void DishView::init()
{
    // Properties
    this->setAttribute( Qt::WA_NoSystemBackground, true );

	// If dish has no image
    if( dish.getPixmap().isNull() || dish.getPixmap().width() == 0 )
	{
		dishPixmap = QPixmap( Style::getDishWidth(), Style::getDishHeight() );
		dishPixmap.fill( Qt::darkRed );
	}
	else
	{
		dishPixmap = dish.getPixmap();
	}

	// Compute scale based on image size, desired dish width and screen resolution
	float scale = Style::getDishWidth() / dishPixmap.width();
	QSize baseWidgetSize = dishPixmap.size() * scale;

	// Size
	resizeByUserPreference( baseWidgetSize );

	// Create B&W image
	QImage blackAndWhiteImg = dishPixmap.toImage();//new QImage( widgetSize, QImage::Format_Grayscale8 );
	for( int x = 0 ; x < blackAndWhiteImg.width() ; x++ )
	{
		for( int y = 0 ; y < blackAndWhiteImg.height() ; y++ )
		{
			int grayColor = qGray( blackAndWhiteImg.pixel( x, y ) );
			blackAndWhiteImg.setPixel( x, y, qRgb( grayColor, grayColor, grayColor ) );
		}
	}

	monochromePixmap = QPixmap::fromImage( blackAndWhiteImg ).scaled( QSize( dishPixmap.size() * scale ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
	dishPixmap = dishPixmap.scaled( QSize( dishPixmap.size() * scale ), Qt::KeepAspectRatio, Qt::SmoothTransformation );

	/* Create objects */
	imageLabel = new QLabel( this );
	imageLabel->setPixmap( dishPixmap );
	imageLabel->setFixedSize( this->size() );
	imageLabel->setAlignment( Qt::AlignCenter );
	imageLabel->adjustSize();

	QPixmap ribbonPixmap = getRibbonByCourse( dish.getCourseNum() );
	ribbonLabel = new QLabel( this );
	ribbonLabel->setPixmap( ribbonPixmap );
	ribbonLabel->adjustSize();
	ribbonLabel->setScaledContents( true );
    ribbonLabel->setAttribute( Qt::WA_NoSystemBackground, true );

// 	QBitmap textMask = ribbonPixmap.createMaskFromColor( Qt::black, Qt::MaskInColor );
// 	ribbonLabel->setMask( textMask );

	identifierLabel = new QLabel( this );
	identifierLabel->setText( dish.getIdentifier() );					// disabled, as might only be relevant when going to lunch
	identifierLabel->setFont( QFont( kFontName, kIdentifierFontSize ) );
	identifierLabel->setAlignment( Qt::AlignCenter );
	identifierLabel->adjustSize();
    identifierLabel->setAttribute( Qt::WA_NoSystemBackground, true );

	QPalette palette = identifierLabel->palette();
	palette.setColor( identifierLabel->foregroundRole(), Qt::white );
	identifierLabel->setPalette( palette );

	detailsLabel = new QLabel( this );
	QString text = dish.getName() + " \n\nIngrediente: " + dish.getIngredients();	// Separate ingredients with 2 spaces
	detailsLabel->setText( text );
	detailsLabel->setWordWrap( true );
    detailsLabel->setFont( QFont( kFontName, kDetailsFontSize ) );
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

		identifierLabel->setFont( QFont( kFontName, kIdentifierFontSize / 1.5f ) );
		identifierLabel->adjustSize();
	}

	/* Move objects */
	identifierLabel->move( identifierLabel->width() / 2, 0 );
	ratingView->move( this->width(), this->height() - kBriefDetailsOffset - ratingView->height() - 2 );

	if( editMode )
		detailsLabel->move( 0, this->height() - detailsLabel->font().pointSize() * kPointToPixel );
	else
		detailsLabel->move( 0, this->height() );

	/* Properties */
	this->setMouseTracking( true );
	imageLabel->setMouseTracking( true );
	detailsLabel->setMouseTracking( true );
}

void DishView::initPlaceholder()
{
	// Size
	this->setFixedSize( kDishPlaceholderSize * Style::getWindowScale() );
	this->adjustSize();

	/* Create objects */
	detailsLabel = new QLabel( this );
	detailsLabel->setAutoFillBackground( true );
	detailsLabel->setFixedSize( this->size() );
	detailsLabel->setStyleSheet( "background-color: rgb(200,200,200)" );
	detailsLabel->adjustSize();

	QComboBox* allDishesCombo = new QComboBox( this );
	allDishesCombo->setEditable( true );
	allDishesCombo->setFont( QFont( kFontName, 10 ) );
	allDishesCombo->setFixedWidth( this->width() );
	allDishesCombo->adjustSize();

	allDishesCombo->insertItem( kAtEnd, QPixmap( RESOURCES_ROOT"Supa1.png" ), "Supa de ceva fara ceva" );
	allDishesCombo->insertItem( kAtEnd, QPixmap( RESOURCES_ROOT"Mancare1.png" ), "Fajitas cu pui" );
	allDishesCombo->insertItem( kAtEnd, QPixmap( RESOURCES_ROOT"Salata1.png" ), "Salata din gradina ursului" );

	QPixmap plusPixmap( RESOURCES_ROOT"plus.png" );
	QPushButton* plusButton = new QPushButton( this );
	plusButton->setIcon( plusPixmap );
	plusButton->setIconSize( plusPixmap.size() );
	plusButton->setStyleSheet( kButtonsStyleSheet );
	plusButton->adjustSize();

	/* Move */
	plusButton->move( ( this->width() - plusButton->width() ) / 2, ( this->height() - plusButton->height() ) / 2 );

	/* Signals and slots */
	connect( allDishesCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( comboSelectionChanged( int ) ) );
}

QPixmap DishView::getRibbonByCourse( int courseNum )
{
	QPixmap pixmap;
	if( courseNum == 1 )
		pixmap = QPixmap( RESOURCES_ROOT"/ribbon1.png" );
	else if( courseNum == 2 )
		pixmap = QPixmap( RESOURCES_ROOT"/ribbon2.png" );
	else
		pixmap = QPixmap( RESOURCES_ROOT"/ribbon3.png" );

	// Text clip path (not working good)
//	QPixmap bkpPixmap = pixmap;

//	QPainter painter( &pixmap );
//	painter.eraseRect( pixmap.rect() );

// 	QFont font = QFont( kFontName, kIdentifierFontSize, QFont::Bold );
// 	font.setKerning( false );
// 	font.setStrikeOut( false );
// 	font.setStyleStrategy( QFont::PreferAntialias );

// 	QPainterPath clipTextPath;
// 	clipTextPath.addText( pixmap.width() / 2 - kIdentifierFontSize, pixmap.height() / 2, font, dish.getIdentifier() );

// 	painter.setFont( font );
// 	painter.drawText( pixmap.width() / 2 - kIdentifierFontSize, pixmap.height() / 2, dish.getIdentifier() );

// 	painter.setClipPath( clipTextPath );
// 	painter.drawPixmap( 0, 0, bkpPixmap );
// 	painter.end();

	return pixmap.scaled( QSize( pixmap.size() * Style::getWindowScale() ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
}

void DishView::comboSelectionChanged( int /*selection*/ )
{

}

bool DishView::event( QEvent* event )
{
	return QWidget::event( event );
}

void DishView::wheelEvent( QWheelEvent* wheelEvent )
{
	return QWidget::wheelEvent( wheelEvent );
}

void DishView::enterEvent( QEvent* /*event*/ )
{
	if( disabled || isPlaceholder || editMode )
		return;

	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	// Start text animation
	detailsAnimation->setStartValue( detailsLabel->pos() );
	detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() - detailsLabel->font().pointSize() * kPointToPixel ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();
}

void DishView::leaveEvent( QEvent* /*event*/ )
{
	if( disabled || isPlaceholder )
		return;

	if( mousePressed && !editMode )
	{
		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );
	}

	if( detailsAnimation->state() == QAbstractAnimation::Running )
		detailsAnimation->stop();

	detailsAnimation->setStartValue( detailsLabel->pos() );

	if( editMode )
		detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() - detailsLabel->font().pointSize() * kPointToPixel ) );
	else
		detailsAnimation->setEndValue( QPointF( detailsLabel->pos().x(), this->height() ) );

	detailsAnimation->setDuration( kBirefDishDetailsAnimationTime );
	detailsAnimation->start();

	mousePressed = false;
}

void DishView::mouseMoveEvent( QMouseEvent* mouseEvent )
{
	if( disabled || isPlaceholder )
		return;

	if( editMode && mousePressed )
	{
		return QWidget::mousePressEvent( mouseEvent );
	}

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
#ifdef Q_OS_ANDROID
    return QWidget::mousePressEvent( mouseEvent );
#endif

	if( disabled || isPlaceholder || mouseEvent->button() != Qt::LeftButton )
		return;

	mousePressed = true;
	
	if( editMode )
	{
		return QWidget::mousePressEvent( mouseEvent );
	}
	else
	{
		this->move( this->pos() + QPoint( kClickMovement, kClickMovement ) );
	}
}

void DishView::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
#ifdef Q_OS_ANDROID
    return  QWidget::mouseReleaseEvent( mouseEvent );
#endif

	if( disabled || isPlaceholder || !mousePressed || mouseEvent->button() != Qt::LeftButton )
		return;

	mousePressed = false;

	if( editMode )
	{
		return QWidget::mousePressEvent( mouseEvent );
	}
	else
	{
		dish.setUserSelected( !dish.getUserSelected() );
		((DayDishesView*)parent())->selectionChangedOn( dish );

		this->move( this->pos() + QPoint( -kClickMovement, -kClickMovement ) );

		setSelected( dish.getUserSelected() );
	}
}

void DishView::mouseDoubleClickEvent( QMouseEvent * event )
{
#ifdef Q_OS_ANDROID
	qDebug() << "double click";

	if( editMode )
	{
		return QWidget::mouseDoubleClickEvent( event );
	}

	dish.setUserSelected( !dish.getUserSelected() );
	((DayDishesView*)parent())->selectionChangedOn( dish );

	setSelected( dish.getUserSelected() );
#else
	return QWidget::mouseDoubleClickEvent( event );
#endif
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
		QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect( this );
		dropShadow->setOffset( 0.0f );
		dropShadow->setBlurRadius( kSelectedShadowSize );
		dropShadow->setColor( kSelectedGlowColor );
		this->setGraphicsEffect( dropShadow );

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

		this->setGraphicsEffect( NULL );

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
		QSize newSize = (this->size() - QSize( Style::getDishSpacing(), 0 )) / 2;
		this->setFixedSize( newSize );
		this->adjustSize();
	}
}
