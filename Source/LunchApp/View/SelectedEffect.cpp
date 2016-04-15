#include "SelectedEffect.h"

#include <QPainter>
#include "Style.h"

SelectedEffect::SelectedEffect( QObject *parent )
	: QGraphicsEffect(parent)
	, enabled( false )
{
}

SelectedEffect::~SelectedEffect()
{
}

QRectF SelectedEffect::boundingRectFor( const QRectF& /*rect*/ ) const
{
	return sourceBoundingRect();

// 	QRectF sourceRect = sourceBoundingRect();
// 
// 	return QRectF( sourceRect.x() - kSelectedGlowSize, sourceRect.y() - kSelectedGlowSize, 
// 					sourceRect.x() + sourceRect.width() + kSelectedGlowSize,
// 					sourceRect.y() + sourceRect.height() + kSelectedGlowSize );
}

void SelectedEffect::draw( QPainter* painter )
{
	// Draw path
	painter->save();

	QRectF boundingRect = sourceBoundingRect();
	painter->drawPixmap( boundingRect.x(), boundingRect.y(), boundingRect.width(), boundingRect.height(), sourcePixmap() );

	if( enabled )
	{
		painter->setPen( QPen( kSelectedGlowColor, kSelectedGlowSize ) );
		painter->drawPath( surroundingPath );
	}

	painter->restore();
}

void SelectedEffect::sourceChanged( ChangeFlags /*flags*/ )
{
	updateBoundingRect();

	// Create path around source
	QRectF sourceRect = sourceBoundingRect();

	surroundingPath = QPainterPath();
	surroundingPath.moveTo( sourceRect.x() + kSelectedGlowSize / 2 , sourceRect.y() + kSelectedGlowSize / 2 );
	surroundingPath.lineTo( sourceRect.x() + kSelectedGlowSize / 2 , sourceRect.y() + sourceRect.height() - kSelectedGlowSize / 2 );
	surroundingPath.lineTo( sourceRect.x() + sourceRect.width() - kSelectedGlowSize / 2, sourceRect.y() + sourceRect.height() - kSelectedGlowSize / 2 );
	surroundingPath.lineTo( sourceRect.x() + sourceRect.width() - kSelectedGlowSize / 2, sourceRect.y() + kSelectedGlowSize / 2 );
	surroundingPath.lineTo( sourceRect.x() + kSelectedGlowSize / 2 , sourceRect.y() + kSelectedGlowSize / 2 );
}
