#pragma once

#include <QStringList>
#include <QPixmap>

class Dish
{
public:
	enum EUserInterest
	{
		eLow,
		eMedium,
		eHigh
	};

	enum EDishRating
	{
		eNotRated,
		eWow,
		eHappy,
		eMeah,
		eYuck
	};

					Dish();
					Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum );

	QString			getName() const										{ return name; }
	void			setName( QString name )								{ this->name = name; }

	QString			getIngredients() const								{ return ingredients; }
	void			setIngredients( QString ingredients )				{ this->ingredients = ingredients; }

	const QPixmap&	getPixmap() const									{ return pixmap; }
	void			setPixmap( QPixmap& pixmap )						{ this->pixmap = pixmap; }

	int				getCourseNum() const								{ return courseNum; }
	void			setCourseNum( int courseNum )						{ this->courseNum = courseNum; }

	QString			getType()											{ return type; }
	void			setType( QString dish )								{ this->type = type; }

	bool			getUserSelected() const								{ return userSelected; };
	void			setUserSelected( bool selected )					{ this->userSelected = selected; }

	EUserInterest	getUserInterest() const								{ return userInterest; };
	void			setUserInterest( EUserInterest userPreference )		{ this->userInterest = userPreference; }

	int				getNumWows()										{ return numWows; }
	void			setNumWows( int numLikes )							{ this->numWows = numLikes; }

	int				getNumHappies()										{ return numHappies; }
	void			setNumHappies( int numLikes )						{ this->numHappies = numLikes; }

	int				getNumMeahs()										{ return numMeahs; }
	void			setNumMeahs( int numLikes )							{ this->numMeahs = numLikes; }

	int				getNumYucks()										{ return numYucks; }
	void			setNumYucks( int numDislikes )						{ this->numYucks = numDislikes; }

	EDishRating		getUserRating()										{ return userRating; }
	void			setUserRating( EDishRating userRating )				{ this->userRating = userRating; }

private:
	QString				name;
	QString				ingredients;
	QPixmap				pixmap;
	int					courseNum;			// 1 == Course 1, 2 == Course 2, etc.
	QString				type;

	bool				userSelected;
	EUserInterest		userInterest;

	int					numWows;
	int					numHappies;
	int					numMeahs;
	int					numYucks;

	EDishRating			userRating;
};

Q_DECLARE_METATYPE( Dish );