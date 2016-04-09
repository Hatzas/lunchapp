#pragma once

#include "Week.h"

class InterestCruncher
{
public:
	static	InterestCruncher	*getInstance();

			void				crunchUserInterest( Week& week );

private:
	static	InterestCruncher	*instance;

								InterestCruncher() {};
};