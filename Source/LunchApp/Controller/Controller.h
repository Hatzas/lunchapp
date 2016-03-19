#pragma once

#include <QObject>
#include "../Model/Dish.h"


class Controller : public QObject
{
	Q_OBJECT

public:
				Controller(QObject *parent);
				~Controller();

public slots:
	void		SelectionChangedOn( const Dish& dish );

private:
	
};
