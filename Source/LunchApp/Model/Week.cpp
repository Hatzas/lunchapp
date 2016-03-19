#include "Week.h"

Week::Week( QString startEndDate, const std::vector<Day>& daysVect )
	: startEndDate( startEndDate )
	, daysVect( daysVect )
{

}

Week::Week( QDateTime startDate, QDateTime endDate, const std::vector<Day>& daysVect )
	: startDate( startDate )
	, endDate( endDate )
	, daysVect( daysVect )
{

}