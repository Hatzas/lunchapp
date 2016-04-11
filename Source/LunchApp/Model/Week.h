#pragma once

#include <vector>
#include <QDate>

#include "Day.h"

class Week
{
public:
							Week();
							Week( QDate startDate, QDate endDate, const std::vector<Day>& daysVect );

	std::vector<Day>&		getDays()											{ return daysVect; }
	void					setDays( const std::vector<Day>& daysVect )			{ this->daysVect = daysVect; }

	QDate					getStartDate() const								{ return startDate; }
	void					setStartDate( QDate startDate )						{ this->startDate = startDate; }

	QDate					getEndDate() const									{ return endDate; }
	void					setEndDate( QDate endDate )							{ this->endDate = endDate; }

	bool					isFirstAvailable() const							{ return firstAvailable; }
	void					setFirstAvailable( bool first )						{ firstAvailable = first; }

	bool					isLastAvailable() const								{ return lastAvailable; }
	void					setLastAvailable( bool last )						{ lastAvailable = last; }

	bool					isEmpty() const										{ return daysVect.size() == 0; }

private:
	QDate				startDate;
	QDate				endDate;
	std::vector<Day>	daysVect;

	bool				lastAvailable;			// True if no more weeks in database after this
	bool				firstAvailable;			// True if no more weeks in database before this
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