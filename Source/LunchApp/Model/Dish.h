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
					Dish( QByteArray inputBuffer );
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
    void			setType( QString type )								{ this->type = type; }

	QString			getIdentifier()										{ return identifier; }
	void			setIdentifier( QString identifier )					{ this->identifier = identifier; }

	int				getNumWows()										{ return numWows; }
	void			setNumWows( int numLikes )							{ this->numWows = numLikes; }

	int				getNumHappies()										{ return numHappies; }
	void			setNumHappies( int numLikes )						{ this->numHappies = numLikes; }

	int				getNumMeahs()										{ return numMeahs; }
	void			setNumMeahs( int numLikes )							{ this->numMeahs = numLikes; }

	int				getNumYucks()										{ return numYucks; }
	void			setNumYucks( int numDislikes )						{ this->numYucks = numDislikes; }

	int				getNumUserOffers()									{ return numUserOffers; }
	void			setNumUserOffers( int numUserOffers )				{ this->numUserOffers = numUserOffers; }

	int				getNumUserPicks()									{ return numUserPicks; }
	void			setNumUserPicks( int numUserPicks )					{ this->numUserPicks = numUserPicks; }

	EDishRating		getUserRating()										{ return userRating; }
	void			setUserRating( EDishRating userRating )				{ this->userRating = userRating; }

	bool			getUserSelected() const								{ return userSelected; };
	void			setUserSelected( bool selected )					{ this->userSelected = selected; }

	EUserInterest	getUserInterest() const								{ return userInterest; };
	void			setUserInterest( EUserInterest userPreference )		{ this->userInterest = userPreference; }

	operator		QByteArray() const;

private:
	QString				name;
	QString				ingredients;
	QPixmap				pixmap;
	int					courseNum;			// 1 == Course 1, 2 == Course 2, etc.
	QString				type;				// Dish type (salad, soup...)
	QString				identifier;

	int					numWows;
	int					numHappies;
	int					numMeahs;
	int					numYucks;			// No negative ratings

	int					numUserOffers;
	int					numUserPicks;

	EDishRating			userRating;

	bool				userSelected;

	EUserInterest		userInterest;
};

Q_DECLARE_METATYPE( Dish );
