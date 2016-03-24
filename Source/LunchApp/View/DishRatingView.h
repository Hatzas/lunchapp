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

			void			init();

public slots:
	virtual void			likeClicked( bool checked = false );
	virtual void			dislikeClicked( bool checked = false );

private:
	QPushButton*	likeButton;
	QPushButton*	dislikeButton;


			void			connectSignals();
};
