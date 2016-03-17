#pragma once

#include <QStringList>
#include <QPixmap>

class Dish
{
public:
					Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum );

	void			addDishType( QString dishType );

	QString			getName()									{ return name; }
	void			setName( QString name )						{ this->name = name; }

	QString			getIngredients()							{ return ingredients; }
	void			setIngredients( QString ingredients )		{ this->ingredients = ingredients; }

	QPixmap&		getPixmap()									{ return pixmap; }
	void			setPixmap( QPixmap& pixmap )				{ this->pixmap = pixmap; }

	int				getCourseNum()								{ return courseNum; }
	void			setCourseNum( int courseNum )				{ this->courseNum = courseNum; }

	bool			isSelected()								{ return selected; };
	void			setSelected( bool selected )				{ this->selected = selected; }

	int				getUserPreference()							{ return userPreference; };
	void			setUserPreference( int userPreference )		{ this->userPreference = userPreference; }

private:
	QString			name;
	QString			ingredients;
	QPixmap			pixmap;
	int				courseNum;			// 1 == Course 1, 2 == Course 2, etc.

	bool			selected;
	int				userPreference;		// 0 = Best, the more the lower

	QStringList		courseNames;

					Dish();
};
