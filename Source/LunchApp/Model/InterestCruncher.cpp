#include "InterestCruncher.h"

#define PERSONAL_RATING_WEIGHT	6765
#define CHOICE_WEIGHT			4181
#define EXTERNAL_RATING_WEIGHT	2584
#define MAX_HIGH_INTEREST		2

InterestCruncher* InterestCruncher::instance = NULL;

InterestCruncher *InterestCruncher::getInstance()
{
	if( instance == NULL )
		instance = new InterestCruncher();

	return instance;
}

void InterestCruncher::crunchUserInterest( Week& week )
{
	std::vector<Day>& days = week.getDays();
	
	std::vector<float> grading;
	float min;
	float max;

	for( int i = 0; i < days.size(); ++i )
	{
		grading.clear();
		min = PERSONAL_RATING_WEIGHT + CHOICE_WEIGHT + EXTERNAL_RATING_WEIGHT;
		max = - min;

		for( int j = 0; j < days[i].getDishes().size(); ++j )
		{
			float grade;

			switch( days[i].getDishes()[j].getUserRating() )
			{
			case Dish::eMeah:
				grade = - PERSONAL_RATING_WEIGHT;
				break;
			case Dish::eHappy:
				grade = 0.5 * PERSONAL_RATING_WEIGHT;
				break;
			case Dish::eWow:
				grade = PERSONAL_RATING_WEIGHT;
				break;
			default:	// Dish::eNotRated etc.
				grade = 0;
			}

			if( days[i].getDishes()[j].getNumUserOffers() != 0 )
			{
				float rateOfChoice = ( (float) days[i].getDishes()[j].getNumUserPicks() ) / days[i].getDishes()[j].getNumUserOffers();
				grade += ( rateOfChoice - 1.0f / 3 ) * ( rateOfChoice > 1.0f / 3 ? 3 / 2 : 3 ) * CHOICE_WEIGHT;		// assumed an average of 3 dishes per course
			}

			float noExternalRatings = days[i].getDishes()[j].getNumMeahs() + days[i].getDishes()[j].getNumHappies() + days[i].getDishes()[j].getNumWows();
			if( noExternalRatings != 0 )
			{
				float externalRating = ( - days[i].getDishes()[j].getNumMeahs() * EXTERNAL_RATING_WEIGHT +
					0.5 * days[i].getDishes()[j].getNumHappies() * EXTERNAL_RATING_WEIGHT +
					days[i].getDishes()[j].getNumWows() * EXTERNAL_RATING_WEIGHT ) /
					( days[i].getDishes()[j].getNumMeahs() + days[i].getDishes()[j].getNumHappies() + days[i].getDishes()[j].getNumWows() );
				grade += externalRating;
			}

			grading.push_back( grade );
			if( min > grade )
				min = grade;
			if( max < grade )
				max = grade;
		}

		Dish::EUserInterest userInterest;
		int highs = 0;
		for( int j = 0; j < days[i].getDishes().size(); ++j )
		{
			if( grading[j] < min + ( max - min ) / 3 )
				userInterest = Dish::eLow;
			else if( grading[j] > max - ( max - min ) / 6 && highs < MAX_HIGH_INTEREST )
			{
				userInterest = Dish::eHigh;
				++highs;
			}
			else
				userInterest = Dish::eMedium;

			days[i].getDishes()[j].setUserInterest( userInterest );
		}

	}
}
