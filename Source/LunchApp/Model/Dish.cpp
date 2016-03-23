#include "Dish.h"

Dish::Dish()
{

}

Dish::Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum )
	: name( name )
	, ingredients( ingredients )
	, pixmap( pixmap )
	, courseNum( courseNum )
	, selected( false )
	, userPreference( 1 )
{

}
