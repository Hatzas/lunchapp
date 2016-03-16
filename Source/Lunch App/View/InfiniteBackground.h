#pragma once

#include <QWidget>

class InfiniteBackground : public QWidget
{
	Q_OBJECT
	Q_PROPERTY( QPoint offset READ getOffset WRITE setOffset NOTIFY offsetChanged )

public:
						InfiniteBackground( const QPixmap & pixmap, QWidget * parent = 0 );
						~InfiniteBackground();

			void		moveBy( int dx, int dy );

			void		setOffset( QPoint offset );
			QPoint		getOffset() const				{ return imageOffset; }

signals:
			void		offsetChanged();

protected:
	virtual	void		paintEvent( QPaintEvent * event );

	virtual	void		resizeEvent( QResizeEvent * event );
	virtual	void		wheelEvent( QWheelEvent* event );

private:
	QPixmap		pixmap;

	QPoint		imageOffset;
	QPoint		internalOffset;

	bool		mirrored;
};
