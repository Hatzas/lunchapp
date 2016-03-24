#pragma once

#include <vector>
#include <QDate>

#include "Day.h"

class Week
{
public:
							Week();
							Week( QString startEndDate, const std::vector<Day>& daysVect );		// This is used just for debug purposes
							Week( QDate startDate, QDate endDate, const std::vector<Day>& daysVect );

	std::vector<Day>&		getDays()											{ return daysVect; }
	void					setDays( const std::vector<Day>& daysVect )			{ this->daysVect = daysVect; }

	QString					getStartEndDate() const								{ return startEndDate; }
	void					setStartEndDate( QString startEndDate )				{ this->startEndDate = startEndDate; }

	QDate					getStartDate() const								{ return startDate; }
	void					setStartDate( QDate startDate )						{ this->startDate = startDate; }

	QDate					getEndDate() const									{ return endDate; }
	void					setEndDate( QDate endDate )							{ this->endDate = endDate; }

private:
	QDate				startDate;
	QDate				endDate;
	std::vector<Day>	daysVect;

	QString				startEndDate;		// Formatted date string
};

Q_DECLARE_METATYPE( Week );


/************************************************************************/
/*							Operators		                           */
/************************************************************************/
inline bool operator>( const Week& first, const Week& second )
{
	if( first.getStartDate() > second.getEndDate() )
		return true;
	else
		return false;
}

inline bool operator<( const Week& first, const Week& second )
{
	if( first.getEndDate() < second.getStartDate() )
		return true;
	else
		return false;
}