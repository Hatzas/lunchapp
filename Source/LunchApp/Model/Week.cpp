#include "Week.h"

Week::Week()
	: lastAvailable( false )
	, firstAvailable( false )
{

}

Week::Week( QDate startDate, QDate endDate, const std::vector<Day>& daysVect )
	: startDate( startDate )
	, endDate( endDate )
	, daysVect( daysVect )
	, lastAvailable( false )
	, firstAvailable( false )
{

}