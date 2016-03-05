#pragma once

#include "Day.h"
#include <QList>

class Week
{
	
private:
	Day	mondayMeal;
	Day	tuesdayMeal;
	Day	wednesdayMeal;
	Day	thursdayMeal;
	Day fridayMeal;

public:
	Day		getMondayMeal() { return mondayMeal; }
	void	setMondayMeal() { this->mondayMeal = mondayMeal; }

	Day		getTuesdayMeal() { return tuesdayMeal; }
	void	setTuesdayMeal() { this->tuesdayMeal = tuesdayMeal; }

	Day		getWednesdayMeal() { return wednesdayMeal; }
	void	setWednesdayMeal() { this->wednesdayMeal = wednesdayMeal; }

	Day		getThursdayMeal() { return thursdayMeal; }
	void	setThursdayMeal() { this->thursdayMeal = thursdayMeal; }

	Day		getFridayMeal() { return fridayMeal; }
	void	setFridayMeal() { this->fridayMeal = fridayMeal; }

};
