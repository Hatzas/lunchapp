#include "Dish.h"

void Dish::addDishType( QString dishType )
{
	dishTypes.append( dishType );
}

QString Dish::getDishType()
{
	return dishTypes.at( typeIndex );
}
