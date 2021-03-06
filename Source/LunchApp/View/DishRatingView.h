#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "..\Model\Dish.h"


class DishRatingView : public QWidget
{
	Q_OBJECT

public:
							DishRatingView( QWidget *parent, const Dish& dish );
							~DishRatingView();
public slots:
	virtual void			wowToggled( bool checked = false );
	virtual void			happyToggled( bool checked = false );
	virtual void			meahToggled( bool checked = false );
	virtual void			yuckToggled( bool checked = false );

protected:
	virtual	void				paintEvent( QPaintEvent * event );

private:
	Dish			dish;

	QPushButton*	wowButton;
	QPushButton*	happyButton;
	QPushButton*	meahButton;
//	QPushButton*	yuckButton;

	QLabel*			numWowsLabel;
	QLabel*			numHappiesLabel;
	QLabel*			numMeahsLabel;
//	QLabel*			numYucksLabel;


			void			init();

			void			connectSignals();
			void			updateCounterLabels();
			void			updateRatingButtons();
};
