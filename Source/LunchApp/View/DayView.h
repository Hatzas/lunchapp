#pragma once

#include <vector>
#include <QWidget>
#include <QLabel>

#include "Model/Day.h"
#include "CommonStructs.h"
#include "DayDishesView.h"


class DayView : public QWidget
{
	Q_OBJECT

public:
						DayView( QWidget *parent, Day& day, EMode mode = eNormalMode );
						DayView( QWidget *parent, QString name, std::vector<Dish>& dishes, EMode mode = eNormalMode );		// only intended for editing
						~DayView();

	
			void		mainWindowResized( QSize size );

			void		selectionChangedOn( const Dish& dish );		

protected:
	virtual	bool		event( QEvent *event );
	virtual void		wheelEvent( QWheelEvent* event );

private:
	Day							day;
	QString						name;
	std::vector<Dish>&			dishesVect;
	EMode						mode;

	QLabel*					dayNameLabel;
	DayDishesView*			dishesView;

	void				init();

};
