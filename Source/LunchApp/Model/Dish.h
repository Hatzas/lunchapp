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
		eLiked,
		eDisliked
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

	int				getNumLikes()										{ return numLikes; }
	void			setNumLikes( int numLikes )							{ this->numLikes = numLikes; }

	int				getNumDislikes()									{ return numDislikes; }
	void			setNumDislikes( int numDislikes )					{ this->numDislikes = numDislikes; }

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

	int					numLikes;
	int					numDislikes;

	EDishRating			userRating;
};

Q_DECLARE_METATYPE( Dish );