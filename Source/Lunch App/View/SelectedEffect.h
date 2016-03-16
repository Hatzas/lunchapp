#pragma once

#include <QGraphicsEffect>
#include <QPainterPath>


class SelectedEffect : public QGraphicsEffect
{
	Q_OBJECT

public:
					SelectedEffect( QObject *parent  = 0 );
					~SelectedEffect();

			void	enable()						{ enabled = true; update(); }
			void	disable()						{ enabled = false; update(); }
			bool	isEnabled()						{ return enabled; }

	virtual QRectF	boundingRectFor( const QRectF & rect ) const;

protected:
	virtual void	draw( QPainter * painter );

	virtual void	sourceChanged( ChangeFlags flags );

private:
	bool			enabled;

	QPainterPath	surroundingPath;
};
