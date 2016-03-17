#pragma once

#include <QString>
#include <QColor>


static const float		kWeekOffsetY					= 100;		// pixels ( spacing from top of view )

static const int		kWeekAnimationTime				= 500;		// milliseconds
static const int		kBirefDishDetailsAnimationTime	= 300;		// milliseconds
static const int		kFullDishDetailsAnimationTime	= 500;		// milliseconds

static const QString	kDetailsOverlayStyleSheet		= "background-color: rgba(255,255,255,80%)";

static const QString	kFontName						= "Segoe UI";

static const float		kSelectedGlowSize				= 1.0f;
static const float		kSelectedShadowSize				= 20.0f;
static const QColor		kSelectedGlowColor				= QColor( 0, 255, 0 );

static const float		kDishWidth						= 300;		// pixels
static const float		kDishSpacing					= 20;		// pixels
static const float		kDayWidth						= kDishWidth + kDishSpacing;	// pixels

static const float		kBackgroundScrollRatio			= 0.2f;		// pixels