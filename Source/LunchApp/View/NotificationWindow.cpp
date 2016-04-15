#include "NotificationWindow.h"

#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>

#include "../MainWindow.h"

QVector<NotificationWindow *>		NotificationWindow::windows;
int									NotificationWindow::lastIdx		= 0;
QSystemTrayIcon*					NotificationWindow::appIcon	= NULL;

static const QSize					kNotificationSize				= QSize( 400, 100 );
static const int					kNotificationOffsetY			= 20;


NotificationWindow::NotificationWindow( MainWindow* mainWindow, const QString &text, const std::vector<QPixmap>& iconsVect, const int duration )
    : QWidget( 0,
              Qt::Tool |
              Qt::FramelessWindowHint |
              Qt::WindowSystemMenuHint |
              Qt::WindowStaysOnTopHint )
	, mainWindow( mainWindow )
	, text( text )
	, iconsVect( iconsVect )
	, mousePressed( false )
{
	if( appIcon == NULL )
		return;

	init();

    windows.append( this );
    lastIdx++;

	// Start close timer
	timer = new QTimer( this );
	timer->setSingleShot( true );
	connect( timer, SIGNAL( timeout() ), this, SLOT( moveOut() ) );
	timer->start( duration - kNotificationAnimationTime );
}

void NotificationWindow::setup( QSystemTrayIcon* trayIcon )
{
	NotificationWindow::appIcon = trayIcon;
}

void NotificationWindow::init()
{
	setAttribute( Qt::WA_TranslucentBackground );

	/* Create objects */
	QLabel* iconLabel = NULL;
    for( size_t i = 0 ; i < iconsVect.size() ; i++ )
	{
		iconLabel = new QLabel( this );
		iconLabel->setPixmap( iconsVect[i].scaled( kNotificationSize - QSize( 20, 20 ), Qt::KeepAspectRatio ) );
		iconLabel->setMargin( 10 );
		iconLabel->adjustSize();

		iconLabel->move( 0, iconLabel->height() * i );
	}
	
	QFont titleFont( kFontName, 12 );
	titleFont.setBold( true );

	QLabel* titleLabel = new QLabel( this );
	titleLabel->setText( kAppName );
	titleLabel->setFont( titleFont );
	titleLabel->setMargin( 10 );
	titleLabel->adjustSize();

	textLabel = new QLabel( this );
	textLabel->setText( text );
	textLabel->setWordWrap( true );
	textLabel->setFont( QFont( kFontName, 10 ) );
	textLabel->setContentsMargins( 10, 0, 10, 10 );
	textLabel->setFixedWidth( kNotificationSize.width() - iconLabel->width() );
	textLabel->adjustSize();

	QPixmap closePixmap = QPixmap( RESOURCES_ROOT"close.png" );
	closeButton = new QPushButton( this );
	closeButton->setStyleSheet( kButtonsStyleSheet );
	closeButton->setIcon( closePixmap );
	closeButton->setFixedSize( closePixmap.size() );
	closeButton->adjustSize();

	/* Animations */
	moveAnimation = new QPropertyAnimation( this, "pos" );
	moveAnimation->setEasingCurve( QEasingCurve::OutCirc );
	moveAnimation->setDuration( kNotificationAnimationTime );

	/* Move and resize */
	titleLabel->move( iconLabel->x() + iconLabel->width(), 0 );
	textLabel->move( titleLabel->x(), titleLabel->y() + titleLabel->height() );
	closeButton->move( kNotificationSize.width() - closeButton->width() * 1.7f, closeButton->height() * 0.5f );

//	setFixedSize( kNotificationSize );
	adjustSize();

	/* Signals & Slots */
	connect( closeButton, SIGNAL( clicked() ), this, SLOT( moveOut() ) );

	moveIn();
}

void NotificationWindow::paintEvent( QPaintEvent* event )
{
	QPainter painter( this );
	painter.fillRect( rect(), QBrush( Qt::white ) );

	QWidget::paintEvent( event );
}

void NotificationWindow::mousePressEvent( QMouseEvent* /*event*/ )
{
	mousePressed = true;
}

void NotificationWindow::mouseReleaseEvent( QMouseEvent* /*event*/ )
{
	if( mousePressed )
	{
		timer->stop();

		showApp();
		//moveOut();
	}
}

bool NotificationWindow::close()
{
	if( windows.size() == 0 )	// This might crash on release builds
		return true;

    auto i = windows.indexOf( this );
	if( i < 0 )
		return true;

    windows.remove( i );
    if( windows.empty() )
        lastIdx = 0;

    return QWidget::close();
}

void NotificationWindow::moveIn()
{
	auto desktopRect = QApplication::desktop()->screenGeometry();
	auto trayIconRect = appIcon->geometry();

	// Compute Y position according to the taskbar position
	int posY = 0;
	if( trayIconRect.y() > desktopRect.height() / 2 )						// taskbar down
	{
		// Stack notifications
		if( windows.size() > 0 )
		{
			posY = windows.last()->y() - this->height() - kNotificationOffsetY / 2;
		}
		else
		{
			posY = trayIconRect.y() - this->height() - kNotificationOffsetY;
		}
	}
	else																	// taskbar up
	{
		// Stack notifications
		if( windows.size() > 0 )
		{
			posY = windows.last()->y() + windows.last()->height() + kNotificationOffsetY / 2;
		}
		else
		{
			posY = trayIconRect.y() + trayIconRect.height() + kNotificationOffsetY;
		}
	}

	moveAnimation->setStartValue( QPoint( desktopRect.width(), posY ) );
	moveAnimation->setEndValue( QPoint( desktopRect.width() - this->width(), posY ) );
	moveAnimation->start();
}

void NotificationWindow::moveOut()
{
	auto desktopRect = QApplication::desktop()->screenGeometry();

	moveAnimation->setStartValue( this->pos() );
	moveAnimation->setEndValue( QPoint( desktopRect.width(), this->y() ) );
	moveAnimation->start();

	connect( moveAnimation, SIGNAL( finished() ), this, SLOT( close() ) );
}

void NotificationWindow::showApp()
{
	if( mainWindow->isHidden() )
	{
		mainWindow->show();
	}
}