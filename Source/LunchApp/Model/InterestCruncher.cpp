#include "InterestCruncher.h"

#include <QtGlobal>

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
	std::vector<float> grading;
	float minGrade;
	float maxGrade;

	qsrand( QTime::currentTime().msec() );

	std::vector<Day>& days = week.getDays();
    for( size_t i = 0; i < days.size(); ++i )
	{
		grading.clear();
		minGrade = PERSONAL_RATING_WEIGHT + CHOICE_WEIGHT + EXTERNAL_RATING_WEIGHT;
		maxGrade = - minGrade;

		std::vector<Dish>& dayDishes = days[i].getDishes();
        for( size_t j = 0; j < dayDishes.size(); ++j )
		{
			float grade;
			Dish& dish = dayDishes[j];

			switch( dish.getUserRating() )
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

			if( dish.getNumUserOffers() != 0 )
			{
				float rateOfChoice = ( (float) dish.getNumUserPicks() ) / dish.getNumUserOffers();
				grade += ( rateOfChoice - 1.0f / 3 ) * ( rateOfChoice > 1.0f / 3 ? 3 / 2 : 3 ) * CHOICE_WEIGHT;		// assumed an average of 3 dishes per course
			}

			float noExternalRatings = dish.getNumMeahs() + dish.getNumHappies() + dish.getNumWows();
			if( noExternalRatings != 0 )
			{
				float externalRating = ( - dish.getNumMeahs() * EXTERNAL_RATING_WEIGHT +
					0.5 * dish.getNumHappies() * EXTERNAL_RATING_WEIGHT +
					dish.getNumWows() * EXTERNAL_RATING_WEIGHT ) /
					( dish.getNumMeahs() + dish.getNumHappies() + dish.getNumWows() );
				grade += externalRating;
			}

			grading.push_back( grade );
			if( minGrade > grade )
				minGrade = grade;
			if( maxGrade < grade )
				maxGrade = grade;
		}

		int highs = 0;
        for( size_t j = 0; j < dayDishes.size(); ++j )
		{
			Dish& dish = dayDishes[j];

			Dish::EUserInterest userInterest;
			if( minGrade != maxGrade )
			{
				if( grading[j] < minGrade + ( maxGrade - minGrade ) / 3 )
				{
					userInterest = Dish::eLow;
				}
				else if( grading[j] > maxGrade - ( maxGrade - minGrade ) / 6 && highs < MAX_HIGH_INTEREST )
				{
					userInterest = Dish::eHigh;
				}
				else
				{
					userInterest = Dish::eMedium;
				}
			}
			else		// random shuffle interest if all are the same
			{
				userInterest = Dish::EUserInterest( qrand() % 3 );
			}

			dish.setUserInterest( userInterest );

			if( userInterest == Dish::eHigh )
				++highs;
		}

		// At least one should be high
		if( highs == 0 )
		{
            for( size_t j = 0; j < dayDishes.size(); ++j )
			{
				if( grading[j] == maxGrade )
				{
					dayDishes[j].setUserInterest( Dish::eHigh );
					break;
				}
			}
		}
	}
}
