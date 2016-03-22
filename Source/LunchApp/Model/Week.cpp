#include "Week.h"

Week::Week()
{

}

Week::Week( QString startEndDate, const std::vector<Day>& daysVect )
	: startEndDate( startEndDate )
	, daysVect( daysVect )
{

}

Week::Week( QDate startDate, QDate endDate, const std::vector<Day>& daysVect )
	: startDate( startDate )
	, endDate( endDate )
	, daysVect( daysVect )
{

}