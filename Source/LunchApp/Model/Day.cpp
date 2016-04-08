#include "Day.h"

Day::Day()
{

}

Day::Day( QString name, std::vector<Dish> dishesVect /*= std::vector<Dish>()*/ )
	: name( name )
	, dishesVect( dishesVect )
{

}