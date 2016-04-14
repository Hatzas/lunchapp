#include "MetroView.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QParallelAnimationGroup>
#include <QGraphicsSceneWheelEvent>
#include <QWheelEvent>
#include <QAbstractAnimation>
#include <QOpenGLWidget>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QBoxLayout>
#include <QScroller>
#include <QScroller>
#include <QLocale>
#include <QDesktopWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QQuickView>
#ifdef Q_OS_ANDROID
#   include <QtAndroidExtras>
#   include <android/log.h>
#endif

#include "Style.h"
#include "InfiniteBackground.h"
#include "MainWindow.h"
#include "../Controller/Controller.h"


static const QString	kWeekDatePrefix				= " Saptamana: ";
static const float		kButtonsYSpacingRatio		= 1.1f;
static const float		kLargeDishScale				= 3.f;

#ifdef Q_OS_ANDROID
static const int		kFontSize					= 13;
#else
static const int		kFontSize					= 10;
#endif

//////////////////////////
// JNI code for Android //
//////////////////////////
#ifdef Q_OS_ANDROID
QString selectedFileName;

#	ifdef __cplusplus
extern "C" {
#	endif

	JNIEXPORT void JNICALL
        Java_org_qtproject_LunchApp_LunchApp_fileSelected(JNIEnv */*env*/,
		jobject /*obj*/,
		jstring results)
	{
        selectedFileName = QAndroidJniObject(results).toString();

        __android_log_print( ANDROID_LOG_VERBOSE, "LunchApp", "fileSelected: %s", selectedFileName.toStdString().c_str() );
	}

#	ifdef __cplusplus
}
#	endif
#endif


MetroView::MetroView( QWidget *parent, bool adminMode /*= false*/ )
	: QGraphicsView( parent )
	, adminMode( adminMode )
	, allDishesView( NULL )
{
//	addCameraWidget();
}

MetroView::~MetroView()
{

}

void MetroView::init()
{
#ifndef QT_NO_OPENGL
	QOpenGLWidget* openGLWidget = new QOpenGLWidget();

// 	QSurfaceFormat format;
// 	format.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
// 	format.setOption( QSurfaceFormat::StereoBuffers, true );
// 	openGLWidget->setUpdateBehavior( QOpenGLWidget::PartialUpdate );
// 	openGLWidget->setFormat( format );

	this->setViewport( openGLWidget );
#endif

    // Properties
	this->setRenderHint( QPainter::Antialiasing );
	this->setRenderHint( QPainter::SmoothPixmapTransform );
	this->setCacheMode( QGraphicsView::CacheBackground );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setAttribute( Qt::WA_NoSystemBackground, true );

	scene = new QGraphicsScene( this );
	this->setScene( scene );

	addSceneItems();
}

void MetroView::addSceneItems()
{
	/* Data requests */
	if( adminMode )
	{
		emit requestAllDishes();
	}

	/* Background */
	background = new InfiniteBackground( QPixmap( RESOURCES_ROOT"background3.bmp" ), this );

	/* Create objects */
	weekPrefixLabel = new QLabel( this );
	weekPrefixLabel->setText( kWeekDatePrefix );
	weekPrefixLabel->setFont( QFont( kFontName, kFontSize ) );
	weekPrefixLabel->adjustSize();
	weekPrefixLabel->move( kDateUsernameSideOffset, kDateUsernameTopOffset );

	weekDateButton = new QPushButton( this );
	weekDateButton->setFont( QFont( kFontName, kFontSize ) );
	weekDateButton->setStyleSheet( kButtonsStyleSheet );
	weekDateButton->move( weekPrefixLabel->x() + weekPrefixLabel->width(), 30 );

	weeksView = new AllWeeksView( this, adminMode );
	weeksView->move( 0, kWeekTopOffset );

    if( adminMode )
    {
        publishButton = new QPushButton( this );
        publishButton->setText( " Publicare " );
        publishButton->setFont( QFont( kFontName, kFontSize ) );
        publishButton->setStyleSheet( kButtonsStyleSheet );

        changeBackgroundButton = new QPushButton( this );
        changeBackgroundButton->setText( " Schimba fundal " );
        changeBackgroundButton->setFont( QFont( kFontName, kFontSize ) );
        changeBackgroundButton->setStyleSheet( kButtonsStyleSheet );

        photoButton = new QPushButton( this );
        photoButton->setStyleSheet( kButtonsStyleSheet );
        photoButton->setMinimumSize( Style::getDayWidth() * kLargeDishScale , Style::getDishHeight() * kLargeDishScale );
        photoButton->setIcon( QIcon( QPixmap( RESOURCES_ROOT"gallery.jpg" ) ) );
        photoButton->setIconSize( photoButton->size() / 2 );
        photoButton->adjustSize();

#ifdef Q_OS_ANDROID
        weeksView->hide();
        publishButton->hide();
		changeBackgroundButton->hide();
#else
		photoButton->hide();
#endif
    }

	userLabel = new QLabel( this );
	userLabel->setText( Controller::getUser()->getUsername() );
	userLabel->setFont( QFont( kFontName, kFontSize ) );
	userLabel->adjustSize();

	administrateButton = new QPushButton( this );
	administrateButton->setText( adminMode ? " Meniu " : " Administrare " );
	administrateButton->setFont( QFont( kFontName, kFontSize ) );
	administrateButton->setStyleSheet( kButtonsStyleSheet );
	administrateButton->adjustSize();

	if( Controller::getUser()->getRole() != User::eAdmin )
    {
		administrateButton->hide();
    }

	calendar = new QCalendarWidget( this );
	calendar->hide();

	/* Add to scene */
	scene->addWidget( background );
	scene->addWidget( weeksView );
	scene->addWidget( weekDateButton );
	scene->addWidget( userLabel );
	scene->addWidget( administrateButton );

	/* Effects */
	QGraphicsOpacityEffect *weekDateOpacity = new QGraphicsOpacityEffect();
	weekDateOpacity->setOpacity( 1.0f );
	weekDateButton->setGraphicsEffect( weekDateOpacity );

	QGraphicsOpacityEffect *calendarOpacity = new QGraphicsOpacityEffect();
	calendarOpacity->setOpacity( 0.5f );
	//calendar->setGraphicsEffect( calendarOpacity );

	/* Animations */
	calendarFadeAnimation = new QPropertyAnimation( calendarOpacity, "opacity" );
	calendarFadeAnimation->setDuration( kWeekAnimationTime / 2.f );
	calendarFadeAnimation->setEasingCurve( QEasingCurve::InCubic );

	weekDateOutAnimation = new QPropertyAnimation( weekDateOpacity, "opacity" );
	weekDateOutAnimation->setDuration( kWeekAnimationTime / 2.f );
	weekDateOutAnimation->setStartValue( 1.0f );
	weekDateOutAnimation->setEndValue( 0.0f );
	weekDateOutAnimation->setEasingCurve( QEasingCurve::OutCubic );

	weekDateInAnimation = new QPropertyAnimation( weekDateOpacity, "opacity" );
	weekDateInAnimation->setDuration( kWeekAnimationTime / 2.f );
	weekDateInAnimation->setStartValue( 0.0f );
	weekDateInAnimation->setEndValue( 1.0f );
	weekDateInAnimation->setEasingCurve( QEasingCurve::InCubic );

	weekMoveAnimation = new QPropertyAnimation( weeksView, "pos" );
	weekMoveAnimation->setEasingCurve( QEasingCurve::OutCirc );

	backgroundAnimation = new QPropertyAnimation( background, "offset" );
	backgroundAnimation->setEasingCurve( QEasingCurve::OutCirc );

	parallelAnimations = new QParallelAnimationGroup();
	parallelAnimations->addAnimation( backgroundAnimation );
	parallelAnimations->addAnimation( weekMoveAnimation );
	parallelAnimations->addAnimation( weekDateOutAnimation );

	// Move
	alignControls();

	if( adminMode )
	{
		setWeekDateText( weeksView->getWeek( 0 ) );
	}

    // Size
#ifdef Q_OS_ANDROID
    QRect windowRect = QApplication::desktop()->screenGeometry();
    this->setMinimumSize( QSize( windowRect.width(), windowRect.height() ) );
#else
	if( adminMode )
    {
		this->setMinimumSize( Style::getWeekWidth() + Style::getDishWidth() + 3 * Style::getDishSpacing(), Style::getWindowHeight() );
    }
    else
    {
		this->setMinimumSize( Style::getWeekWidth(), Style::getWindowHeight() );
    }
#endif
	this->adjustSize();

	// Scrolling image background
	background->setMinimumSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	// Signals / Slots
	connect( weekMoveAnimation, SIGNAL( finished(void) ), this, SLOT( weekAnimationFinished(void) ) );
	connect( weekDateButton, SIGNAL( clicked( bool ) ), this, SLOT( weekDatePressed( bool ) ) );
	connect( calendar, SIGNAL( selectionChanged() ), this, SLOT( dateSelected() ) );
	connect( administrateButton, SIGNAL( clicked( bool ) ), parent(), SLOT( switchAdministrate( bool ) ) );

	if( adminMode )
	{
		connect( publishButton, SIGNAL( clicked( bool ) ), this, SLOT( publishPressed( bool ) ) );
        connect( photoButton, SIGNAL( clicked( bool ) ), this, SLOT( uploadImagePressed( bool ) ) );
	}
}

void MetroView::weekArrived( const Week& week )
{
	weeksView->addWeek( week );

	if( weeksView->getWeeksNum() == 1 )
	{
		setWeekDateText( week );
	}
}

void MetroView::allDishesArrived( Day containerDay )
{
    if( allDishesView != NULL || !adminMode )
		return;

	// Add view with all dishes
	allDishesView = new DayView( this, "Toate", containerDay.getDishes(), eBrowseMode );

	// Move to right of screen
	allDishesView->move( this->width() - allDishesView->width() - 2 * Style::getDishSpacing(), weeksView->y() );

	this->adjustSize();
}

void MetroView::weekAnimationFinished()
{
	// Get currently visible week
	Week currentWeek = weeksView->getVisibleWeek();
	if( currentWeek.getDays().size() == 0 )
		return;

	setWeekDateText( currentWeek );
}

void MetroView::weekDatePressed( bool /*checked*/ )
{
	calendar->move( weekDateButton->x() + weekDateButton->width(), weekDateButton->y() + weekDateButton->height() );

// 	if( !calendar->isVisible() )
// 	{
// 		calendarFadeAnimation->setStartValue( 0.0f );
// 		calendarFadeAnimation->setEndValue( 1.0f );
// 
// 		calendarFadeAnimation->start();
// 	}

	calendar->setVisible( !calendar->isVisible() );	
}

void MetroView::dateSelected()
{
	calendar->setVisible( false );

//	QDate date = calendar->selectedDate();

	// Determine week based selected date
//	QDate startDate = date.addDays( -date.dayOfWeek() + 1 );
//	QDate endDate = date.addDays( 5 - date.dayOfWeek() );

	// Request week based on selected date
	// TO DO
}

void MetroView::publishPressed( bool )
{
	emit publishWeek( weeksView->getVisibleWeek( ) );
}

void MetroView::uploadImagePressed( bool )
{
	bool fromCamera = true;
	QPixmap pixmap = takePicture( fromCamera );
	if( pixmap.width() == 0 )
		return;

	// Set thumbnail
	QPixmap croppedPixmap;
	if( pixmap.width() > pixmap.height() )
	{
        int newWidth = photoButton->rect().width() * (float)pixmap.height() / (float)photoButton->height();
		int margin = ( pixmap.width() - newWidth ) / 2;

		QRect cropRect = QRect( margin, 0, pixmap.width() - 2 * margin, pixmap.height());
        croppedPixmap = pixmap.copy( cropRect ).scaled( photoButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
	}
	else
	{
        int newHeight = photoButton->rect().height() * (float)pixmap.width() / (float)photoButton->width();
		int margin = ( pixmap.height() - newHeight ) / 2;

		QRect cropRect = QRect( 0, margin, pixmap.width(), pixmap.height() - 2 * margin );
        croppedPixmap = pixmap.copy( cropRect ).scaled( photoButton->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
	}

    photoButton->setIcon( QIcon( croppedPixmap ) );
    photoButton->setIconSize( photoButton->size() );

	emit uploadPicture( pixmap );
}

void MetroView::wheelEvent( QWheelEvent* wheelEvent )
{
	calendar->setVisible( false );

	if( adminMode )		// Scrolling through weeks disabled for now
		return;

	int scrollDist = this->width() /*Style::getDayWidth()*/;	// Scroll one column or the whole week
	if( wheelEvent->delta() > 0 )
	{
		if( weekMoveAnimation->state() != QAbstractAnimation::Running )
		{
			// Notify week of scroll and check if not already loading
			bool canScroll = weeksView->scrollStarted( AllWeeksView::eToLeftDirection );
			if( !canScroll )
				return;

			// Update animations
			weekMoveAnimation->setStartValue( weeksView->pos() );
			weekMoveAnimation->setEndValue( QPointF( weeksView->pos().x() + scrollDist, weeksView->pos().y() ) );
			weekMoveAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() + scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			parallelAnimations->start();
		}
		else
		{
			return;

			parallelAnimations->stop();

			weekMoveAnimation->setStartValue( weekMoveAnimation->currentValue().toPointF() );
			weekMoveAnimation->setEndValue( QPointF( weekMoveAnimation->endValue().toPointF().x() + scrollDist, weeksView->pos().y() ) );

			int numDays = (int)((weekMoveAnimation->endValue().toPointF().x() - weekMoveAnimation->startValue().toPointF().x()) / Style::getDayWidth());
			weekMoveAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() + scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			parallelAnimations->start();
		}
	} 
	else if( wheelEvent->delta() < 0 )
	{
		if( weekMoveAnimation->state() != QAbstractAnimation::Running )
		{
			// Notify week of scroll and check if not already loading
			bool canScroll = weeksView->scrollStarted( AllWeeksView::eToRightDirection );
			if( !canScroll )
				return;

			// Update animations
			weekMoveAnimation->setStartValue( weeksView->pos() );
			weekMoveAnimation->setEndValue( QPointF( weeksView->pos().x() - scrollDist, weeksView->pos().y() ) );
			weekMoveAnimation->setDuration( kWeekAnimationTime );

			backgroundAnimation->setStartValue( background->getOffset() );
			backgroundAnimation->setEndValue( QPointF( background->getOffset().x() - scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime );

			parallelAnimations->start();
		}
		else
		{
			return;

			parallelAnimations->stop();

			weekMoveAnimation->setStartValue( weekMoveAnimation->currentValue().toPointF() );
			weekMoveAnimation->setEndValue( QPointF( weekMoveAnimation->endValue().toPointF().x() - scrollDist, weeksView->pos().y() ) );

			int numDays = (int)((weekMoveAnimation->startValue().toPointF().x() - weekMoveAnimation->endValue().toPointF().x()) / Style::getDayWidth());
			weekMoveAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			backgroundAnimation->setStartValue( backgroundAnimation->currentValue().toPointF() );
			backgroundAnimation->setEndValue( QPointF( backgroundAnimation->endValue().toPointF().x() - scrollDist * kBackgroundScrollRatio, background->getOffset().y() ) );
			backgroundAnimation->setDuration( kWeekAnimationTime * numDays/2 );

			parallelAnimations->start();
		}
	}
}

void MetroView::resizeEvent( QResizeEvent * event )
{
	QGraphicsView::resizeEvent( event );

	background->setFixedSize( this->size() );
	background->setMaximumSize( this->size() );
	background->adjustSize();

	QSize size = event->size();
	if( allDishesView )
		size.setWidth( size.width() - allDishesView->width() - 2 * Style::getDishSpacing() );

	weeksView->mainWindowResized( size );

	// Move to right of screen
	if( adminMode && allDishesView )
		allDishesView->move( this->width() - allDishesView->width() - 2 * Style::getDishSpacing(), weeksView->y() );

	alignControls();
}

void MetroView::setWeekDateText( const Week &currentWeek )
{
	QLocale romanian;

	QDate startDate = currentWeek.getStartDate();
	QDate endDate = currentWeek.getEndDate();
	if( startDate.month() != endDate.month() )
	{
		weekDateButton->setText( " " + QString::number( startDate.day() ) + " " + romanian.monthName( startDate.month() )
			+ " - " + QString::number( endDate.day() ) + " " + romanian.monthName( endDate.month() ) + " " );
	}
	else
	{
		weekDateButton->setText( " " + QString::number( startDate.day() ) + " - " + QString::number( endDate.day() ) 
			+ " " + romanian.monthName( endDate.month() ) + " " );
	}

	weekDateButton->adjustSize();

	weekDateInAnimation->start();
}

void MetroView::alignControls()
{
	if( Controller::getUser()->getRole() == User::eAdmin )
	{
		administrateButton->move( this->width() - administrateButton->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
		userLabel->move( administrateButton->x() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );

		if( adminMode )
		{
			publishButton->move( administrateButton->x() - ( publishButton->width() - administrateButton->width() ) / 2, administrateButton->y() + administrateButton->height() * kButtonsYSpacingRatio );
			changeBackgroundButton->move( weekPrefixLabel->x(), publishButton->y() );

            photoButton->move( ( this->width() - Style::getDayWidth() - photoButton->width() ) / 2.f, ( this->height() - photoButton->height() ) / 2.f );
		}
	}
	else
	{
		userLabel->move( this->width() - userLabel->width() - kDateUsernameSideOffset, kDateUsernameTopOffset );
	}
}

QPixmap MetroView::takePicture( bool fromCamera )
{
	QPixmap pixmap;
#ifdef Q_OS_ANDROID
    __android_log_print( ANDROID_LOG_VERBOSE, "LunchApp", "takePicture()" );

	selectedFileName = "#";

	if( fromCamera )
	{
		QAndroidJniObject::callStaticMethod<void>("org/qtproject/LunchApp/LunchApp",
			"takePicture",
			"()V");
	}
	else
	{
		QAndroidJniObject::callStaticMethod<void>("org/qtproject/LunchApp/LunchApp",
			"openAnImage",
			"()V");
	}

	while(selectedFileName == "#")
		qApp->processEvents();

    __android_log_print( ANDROID_LOG_VERBOSE, "LunchApp", "%s", selectedFileName.toStdString().c_str() );

	if( QFile( selectedFileName ).exists() )
	{
		pixmap = QPixmap( selectedFileName );

		__android_log_print( ANDROID_LOG_VERBOSE, "LunchApp", "file ok" );
	}
	else
	{
		__android_log_print( ANDROID_LOG_VERBOSE, "LunchApp", "file doesn't exist" );
	}
#else
	pixmap = QPixmap( "C:/Users/a.f.dascalu/Desktop/Android Backgrounds/Grand_Canyon_of_the_Colorado_2560x1600.jpg" );
#endif

	return pixmap;
}

void MetroView::addCameraWidget()
{
	/* QML approach */
	this->setAttribute( Qt::WA_DontCreateNativeAncestors );

	QQuickView *view = new QQuickView();

	QWidget *container = QWidget::createWindowContainer( view, this );
	container->setMinimumSize( 200, 200 );
	container->setMaximumSize( 200, 200 );
	container->setFocusPolicy( Qt::TabFocus );
	container->move( 300, 300 );

	view->setSource( QUrl("qrc:///QML/test.qml") );

	container->show();

	/* C++ approach */
	// 	if( QCameraInfo::availableCameras().count() == 0 )
	// 	{
	// 		return;
	// 	}

	// Take picture
	//	QCamera* camera = new QCamera( QCamera::BackFace );

	// 	QCameraViewfinder* viewfinder = new QCameraViewfinder();
	// 	viewfinder->move( selectedDishLargeImage->pos() );
	// 	viewfinder->show();

	//     camera->setViewfinder( viewfinder );
	//
	//     QCameraImageCapture* imageCapture = new QCameraImageCapture( camera );
	//
	//     camera->setCaptureMode( QCamera::CaptureStillImage );
	// 	camera->start();
}
