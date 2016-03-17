#pragma once

#include <vector>
#include "Day.h"

class Week
{
public:
							Week( QString startEndDate, const std::vector<Day>& daysVect );

	QString					getStartEndDate() const								{ return startEndDate; }
	void					setStartEndDate( QString startEndDate )				{ this->startEndDate = startEndDate; }

	std::vector<Day>&		getDays()											{ return daysVect; }
	void					setDays( const std::vector<Day>& daysVect )			{ this->daysVect = daysVect; }

private:
	QString				startEndDate;
	std::vector<Day>	daysVect;

							Week();
};
