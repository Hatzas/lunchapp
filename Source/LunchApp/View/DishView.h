#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>

#include "SelectedEffect.h"
#include "DishRatingView.h"
#include "../Model/Dish.h"


class DishView : public QWidget
{
	Q_OBJECT

public:
								DishView( QWidget* parent );						// Placeholder constructor
								DishView( QWidget *parent, const Dish& dish, bool editMode = false );
								~DishView();

			void				setDisabled( bool disabled );
			bool				getDisabled()						{ return disabled; }

	const	Dish&				getDish()							{ return dish; }
	const	QPixmap&			getScaledPixmap()					{ return dishPixmap; }

			bool				operator<( const DishView& right ) const;

public slots:
			void				comboSelectionChanged( int selection );

protected:
	virtual bool				event( QEvent* event );
	virtual void				wheelEvent( QWheelEvent* event );
	
	virtual void				enterEvent( QEvent* event );
	virtual void				leaveEvent( QEvent* event );

	virtual void				mouseMoveEvent( QMouseEvent* event );
	virtual void				mousePressEvent( QMouseEvent* event );
	virtual void				mouseReleaseEvent( QMouseEvent* event );

	virtual void				mouseDoubleClickEvent( QMouseEvent* event );

private:
			Dish						dish;
			bool						disabled;			// true when a user selection excludes other dishes (like from the same course)
			bool						isPlaceholder;
			bool						editMode;

			QLabel*						imageLabel;
			QLabel*						ribbonLabel;
			QLabel*						identifierLabel;
			QLabel*						detailsLabel;
			DishRatingView*				ratingView;

			QPropertyAnimation*			detailsAnimation;
			QPropertyAnimation*			ratingViewAnimation;

			SelectedEffect*				selectedEffect;
			QPixmap						monochromePixmap;
			QPixmap						dishPixmap;

			bool						mousePressed;

			void				init();
			void				initPlaceholder();
			void				resizeByUserPreference( QSize baseSize );

			QPixmap				getRibbonByCourse( int courseNum );

			void				setSelected( bool selected );
};


/************************************************************************/
/*							Operators		                           */
/************************************************************************/
inline bool operator<( const DishView& first, const DishView& second )
{
	return first.operator<( second );
}

inline bool DishView::operator<( const DishView& right ) const
{
	if( width() * height() < right.width() * right.height() )
		return true;
	else
		return false;
}