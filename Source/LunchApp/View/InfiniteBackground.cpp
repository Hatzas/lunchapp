#include "InfiniteBackground.h"

#include <QPainter>
#include <QPaintEvent>

#include "MetroView.h"


InfiniteBackground::InfiniteBackground( const QPixmap & pixmap, QWidget * parent /* = 0 */ )
	: QWidget( parent )
	, pixmap( pixmap )
	, mirrored( false )
{
}

InfiniteBackground::~InfiniteBackground()
{
}

void InfiniteBackground::moveBy( int dx, int dy )
{
	mirrored = ( ( internalOffset.x() + dx ) / this->width() ) % 2 != 0;

	internalOffset.setX( ( internalOffset.x() + dx ) % this->width() );
	internalOffset.setY( internalOffset.y() + dy );

	update();
}

void InfiniteBackground::setOffset( QPoint offset )
{
	imageOffset = offset;

	mirrored = ( imageOffset.x() / this->width() ) % 2 != 0;

	internalOffset.setX( imageOffset.x()  % this->width() );
	internalOffset.setY( imageOffset.y() );

	update();
}

void InfiniteBackground::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );

	QRect destRect = QRect( 0, 0, this->width(), this->height() );

	float scale = this->height() / pixmap.height();
	QRect sourceRect = QRect( 0, 0, this->width() * scale, pixmap.height() * scale );

	painter.save();
	if( internalOffset.x() > 0 )
	{
		QTransform transform;
		transform.translate( internalOffset.x(), 0 );
		transform.scale( -1, 1 );
		painter.setTransform( transform );

		painter.drawPixmap( destRect, pixmap, sourceRect );
	}
	else if( internalOffset.x() < 0 )
	{
		QTransform transform;
		transform.translate( internalOffset.x() + 2 * this->width(), 0 );
		transform.scale( -1, 1 );
		painter.setTransform( transform );

		painter.drawPixmap( destRect, pixmap, sourceRect );
	}
	painter.restore();

	if( mirrored )
	{
		QTransform transform;
		transform.translate( this->width(), 0 );
		transform.scale( -1, 1 );
		painter.setTransform( transform );
	}

	QRect targetRect = QRect( internalOffset, QSize( this->width(), this->height() ) );
	painter.drawPixmap( targetRect, pixmap, sourceRect );
}

void InfiniteBackground::resizeEvent( QResizeEvent * event )
{
	pixmap.scaledToHeight( this->height() );

	QWidget::resizeEvent( event );
}

void InfiniteBackground::wheelEvent( QWheelEvent* wheelEvent )
{
	return ((MetroView*)this->parent())->wheelEvent( wheelEvent );
}