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
	, userInterest( eHigh )
	, numWows( 0 )
	, numHappies( 0 )
	, numMeahs( 0 )
	, numYucks( 0 )
	, userRating( eNotRated )
{

}
