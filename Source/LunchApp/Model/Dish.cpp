#include "Dish.h"

Dish::Dish()
{

}

Dish::Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum )
	: name( name )
	, ingredients( ingredients )
	, pixmap( pixmap )
	, courseNum( courseNum )
	, userSelected( false )
	, userPreference( 1 )
	, numLikes( 0 )
	, numDislikes( 0 )
	, userRating( eNotRated )
{

}
