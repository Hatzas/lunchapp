#pragma once

#include <QString>
#include <QColor>
#include <QSize>


static const float		kWeekOffsetY					= 100;		// pixels ( spacing from top of view )

static const int		kWeekAnimationTime				= 500;		// milliseconds
static const int		kBirefDishDetailsAnimationTime	= 300;		// milliseconds
static const int		kFullDishDetailsAnimationTime	= 500;		// milliseconds

static const QString	kDetailsOverlayStyleSheet		= "background-color: rgba(255,255,255,80%)";

static const QString	kFontName						= "Segoe UI";

static const float		kSelectedGlowSize				= 2.0f;
static const float		kSelectedShadowSize				= 20.0f;
static const QColor		kSelectedGlowColor				= QColor( 255, 102, 0 );
static const QColor		kDisabledColor					= QColor( 32, 32, 32 );
static const QString	kSelectedStyleSheet				= "border: 2px solid rgb(255, 102, 0)";

static const float		kDishWidth						= 300;		// pixels
static const float		kDishSpacing					= 10;		// pixels
static const float		kDaySpacing						= 30;		// pixels
static const float		kDayWidth						= kDishWidth + kDishSpacing;	// pixels
static const float		kWeekWidth						= 5 * kDayWidth + 4 * kDaySpacing + kDishSpacing;

static const float		kBackgroundScrollRatio			= 0.2f;		// pixels

static const QSize		kLoadingAnimSize				= QSize( 24, 24 );
static const int		kLoadingAnimOffset				= 200;