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
	virtual void			likeToggled( bool checked = false );
	virtual void			dislikeToggled( bool checked = false );

private:
	Dish			dish;

	QPushButton*	likeButton;
	QPushButton*	dislikeButton;

	QLabel*			numLikesLabel;
	QLabel*			numDislikesLabel;


			void			init();
			void			connectSignals();
			void			updateCounterLabels();
};
