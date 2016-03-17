#pragma once

#include <QStringList>
#include <QImage>

class Dish
{

private:
			QStringList		dishTypes;
			QImage			preview;
			QString			description;
			int				typeIndex;

public:
			QString		getDescription() { return description; }
			void		setDescription( QString description ) { this->description = description; }

			QString		getDishType();
			void		setDishType( int typeIndex ) { this->typeIndex = typeIndex; }

			void		addDishType( QString dishType );

};
