#pragma once

#include "Dish.h"

class Day
{
public:
							Day();
							Day( QString name, std::vector<Dish> dishesVect = std::vector<Dish>() );

	void					setName( QString name )								{ this->name = name; }
	QString					getName()											{ return name; }

	void					setDishes( const std::vector<Dish>& dishesVect )	{ this->dishesVect = dishesVect; }
	std::vector<Dish>&		getDishes() 										{ return dishesVect; }

private:
	QString					name;
	std::vector<Dish>		dishesVect;
};

Q_DECLARE_METATYPE( Day );