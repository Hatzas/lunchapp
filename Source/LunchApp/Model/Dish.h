#pragma once

#include <QStringList>
#include <QPixmap>

class Dish
{
public:
					Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum );

	void			addDishType( QString dishType );

	QString			getName() const								{ return name; }
	void			setName( QString name )						{ this->name = name; }

	QString			getIngredients() const						{ return ingredients; }
	void			setIngredients( QString ingredients )		{ this->ingredients = ingredients; }

	const QPixmap&	getPixmap() const							{ return pixmap; }
	void			setPixmap( QPixmap& pixmap )				{ this->pixmap = pixmap; }

	int				getCourseNum() const						{ return courseNum; }
	void			setCourseNum( int courseNum )				{ this->courseNum = courseNum; }

	bool			isSelected() const							{ return selected; };
	void			setSelected( bool selected )				{ this->selected = selected; }

	int				getUserPreference() const					{ return userPreference; };
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
