#pragma once

#include <vector>
#include <QDateTime>

#include "Day.h"

class Week
{
public:
							Week( QString startEndDate, const std::vector<Day>& daysVect );		// This is used just for debug purposes
							Week( QDateTime startDate, QDateTime endDate, const std::vector<Day>& daysVect );

	std::vector<Day>&		getDays()											{ return daysVect; }
	void					setDays( const std::vector<Day>& daysVect )			{ this->daysVect = daysVect; }

	QString					getStartEndDate() const								{ return startEndDate; }
	void					setStartEndDate( QString startEndDate )				{ this->startEndDate = startEndDate; }

	QDateTime				getStartDate()										{ return startDate; }
	void					setStartDate( QDateTime startDate )					{ this->startDate = startDate; }

	QDateTime				getEndDate()										{ return endDate; }
	void					setEndDate( QDateTime endDate )						{ this->endDate = endDate; }

private:
	QDateTime			startDate;
	QDateTime			endDate;
	std::vector<Day>	daysVect;

	QString				startEndDate;		// Formatted date string
							Week();
};
