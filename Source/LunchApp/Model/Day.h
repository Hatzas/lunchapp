#pragma once

#include "Dish.h"

class Day
{
public:
							Day( QString name, const std::vector<Dish>& dishesVect );

	void					setName( QString name )								{ this->name = name; }
	QString					getName()											{ return name; }

	void					setDishes( const std::vector<Dish>& dishesVect )	{ this->dishesVect = dishesVect; }
	std::vector<Dish>&		getDishes()											{ return dishesVect; }

private:
	QString					name;
	std::vector<Dish>		dishesVect;

							Day();
};
