#pragma once

#include <QPixmap>
#include <QString>
#include <QColor>
#include <QSize>
#include <QTime>

//#define Q_OS_ANDROID

#ifdef Q_OS_ANDROID
#	define RESOURCES_ROOT		":/Resources/"
#else
#	define RESOURCES_ROOT		"Resources/"
#endif

static const QString	kAppName						= "SmartMeal";
static const QString	kAppIconPath					= RESOURCES_ROOT"icon.png";

static const int		kBaseScreenWidth				= 1920;		// pixels

static const int		kNotificationShowTime			= 6000;		// milliseconds
static const int		kMenuNotificationShowTime		= 1800000;	// milliseconds (30 minutes)
static const int		kNotificationAnimationTime		= 450;		// milliseconds

static const QTime		kDayMenuNotificationTime		= QTime( 9, 0 ); /*QTime( 12, 10 );*/	// 12h 10min

static const float		kDateUsernameTopOffset			= 30;		// pixels ( spacing from top of view )
static const float		kDateUsernameSideOffset			= 40;		// pixels ( spacing from top of view )

static const float		kWeekTopOffset					= 100;		// pixels ( spacing from top of view )

static const int		kWeekAnimationTime				= 500;		// milliseconds
static const int		kBirefDishDetailsAnimationTime	= 300;		// milliseconds
static const int		kFullDishDetailsAnimationTime	= 500;		// milliseconds

static const QString	kDetailsOverlayStyleSheet		= "background-color: rgba(255,255,255,80%)";

static const QString	kButtonsStyleSheet				=  "QPushButton:hover{\
															border: 2px dashed rgba(173,255,47,80%);\
															background-color: rgba(173,255,47,80%);\
															}\
															\
															QPushButton {\
															border: 2px solid rgba(0,0,0,0%);\
															background-color: rgba(0,0,0,0%);\
															}\
															\
															QPushButton:pressed {\
															border: 2px solid rgba(173,255,47,30%);\
															background-color: rgba(173,255,47,30%);\
															}\
															\
															QPushButton:checked {\
															border: 2px solid rgb(173,255,47,80%);\
															background-color: rgb(173,255,47,80%);\
															}";

static const QString	kEditableDayStyleSheet			=  "QLabel{\
															background-color: rgba(128,128,128,10%);\
															}\
															\
															QLabel:hover{\
															border: 2px dashed rgb(0, 191, 255);\
															background-color: rgba(128,128,128,10%);\
															}";
static const QString	kEditableDayDragEnterStyleSheet	=  "QLabel{\
														   border: 2px dashed rgb(0, 191, 255);\
														   background-color: rgba(128,128,128,10%);\
														   }";

static const QString	kFontName						= "Segoe UI";

static const float		kSelectedGlowSize				= 2.0f;
static const float		kSelectedShadowSize				= 20.0f;
static const QColor		kSelectedGlowColor				= QColor( 255, 102, 0 );
static const QColor		kDisabledColor					= QColor( 32, 32, 32 );
static const QString	kSelectedStyleSheet				= "border: 2px solid rgb(255, 0, 0)";
static const QColor		kDishIdentifierColor			= QColor( 255, 255, 0, 255 );

static const QSize		kDishPlaceholderSize			= QSize( 300, 200 );

static const float		kBackgroundScrollRatio			= 0.2f;

class Style
{
public:
	static	void		init();

	static float		getWindowScale()		{ return windowScale; }
	static int			getWindowHeight()		{ return kBaseWindowHeight * windowScale; }

	static float		getDishWidth()			{ return kDishWidth * windowScale; }
	static float		getDishHeight()			{ return kDishHeight * windowScale; }
	static float		getDishSpacing()		{ return kDishSpacing * windowScale; }
	static float		getDaySpacing()			{ return kDaySpacing * windowScale; }
	static float		getDayWidth()			{ return kDayWidth * windowScale; }
	static float		getWeekWidth()			{ return kWeekWidth * windowScale; }


private:
	static	float			windowScale;

	static const int		kBaseWindowHeight;

	static const float		kDishWidth;
	static const float		kDishHeight;
	static const float		kDishSpacing;
	static const float		kDaySpacing;
	static const float		kDayWidth;
	static const float		kWeekWidth;

						Style();
};