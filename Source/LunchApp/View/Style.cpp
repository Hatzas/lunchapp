#include "Style.h"

#include <QApplication>
#include <QDesktopWidget>


		float	Style::windowScale			= 1.0f;

const	int		Style::kBaseWindowHeight	= 900;		// pixels
const	float	Style::kDishWidth			= 300;		// pixels
const	float	Style::kDishSpacing			= 10;		// pixels
const	float	Style::kDaySpacing			= 10;		// pixels
const	float	Style::kDayWidth			= kDishWidth + 2 * kDishSpacing;	// pixels
const	float	Style::kWeekWidth			= 5 * kDayWidth + 4 * kDaySpacing + kDishSpacing;


Style::Style()
{

}

void Style::init()
{
	int screenWidth = QApplication::desktop()->screenGeometry().width();
	windowScale = (float)screenWidth / kBaseScreenWidth;
}