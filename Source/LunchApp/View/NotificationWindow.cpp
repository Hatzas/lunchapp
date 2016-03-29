#include "NotificationWindow.h"

#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>

#include "Style.h"

QVector<NotificationWindow *>		NotificationWindow::windows;
int									NotificationWindow::lastIdx		= 0;

static const QSize					kNotificationSize				= QSize( 400, 100 );
static const int					kNotificationOffsetY			= 20;


NotificationWindow::NotificationWindow( QSystemTrayIcon* trayIcon, const QString &text, const int duration )
    : QWidget( 0,
              Qt::Tool |
              Qt::FramelessWindowHint |
              Qt::WindowSystemMenuHint |
              Qt::WindowStaysOnTopHint )
	, trayIcon( trayIcon )
	, text( text )
{
	init();

    windows.append( this );
    lastIdx++;

	QTimer::singleShot( duration - kNotificationAnimationTime * 1.1f, this, SLOT( moveOut() ) );
	QTimer::singleShot( duration, this, SLOT( close() ) );
}

void NotificationWindow::init()
{
	setAttribute( Qt::WA_TranslucentBackground );
	setFixedSize( kNotificationSize );
	adjustSize();

	/* Create objects */
	QLabel* iconLabel = new QLabel( this );
	iconLabel->setPixmap( QPixmap( kAppIconPath ).scaled( kNotificationSize - QSize( 20, 20 ), Qt::KeepAspectRatio ) );
	iconLabel->setMargin( 10 );
	iconLabel->adjustSize();
	
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

	/* Animations */
	moveAnimation = new QPropertyAnimation( this, "pos" );
	moveAnimation->setEasingCurve( QEasingCurve::OutCirc );
	moveAnimation->setDuration( kNotificationAnimationTime );

	/* Move and resize */
	titleLabel->move( iconLabel->x() + iconLabel->width(), 0 );
	textLabel->move( titleLabel->x(), titleLabel->y() + titleLabel->height() );

	moveIn();
}

void NotificationWindow::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );
	painter.fillRect( rect(), QBrush( Qt::white ) );

	QWidget::paintEvent( event );
}

bool NotificationWindow::close()
{
    auto i = windows.indexOf( this );

    Q_ASSERT( i >= 0 );

    windows.remove( i );
    if( windows.empty() )
        lastIdx = 0;

    return QWidget::close();
}

void NotificationWindow::moveIn()
{
	auto desktopRect = QApplication::desktop()->screenGeometry();
	auto trayIconRect = trayIcon->geometry();

	moveAnimation->setStartValue( QPoint( desktopRect.width(), trayIconRect.y() - this->height() - kNotificationOffsetY ) );
	moveAnimation->setEndValue( QPoint( desktopRect.width() - this->width(), trayIconRect.y() - this->height() - kNotificationOffsetY ) );
	moveAnimation->start();
}

void NotificationWindow::moveOut()
{
	auto desktopRect = QApplication::desktop()->screenGeometry();
	auto trayIconRect = trayIcon->geometry();

	moveAnimation->setStartValue( this->pos() );
	moveAnimation->setEndValue( QPoint( desktopRect.width(), trayIconRect.y() - this->height() - kNotificationOffsetY ) );
	moveAnimation->start();
}
