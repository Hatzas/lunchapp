#include "DishRatingView.h"

#include <QPainter>

#include "Style.h"


static const int kBorderSize = 1;	// pixels


DishRatingView::DishRatingView( QWidget *parent, const Dish& dish )
	: QWidget(parent)
	, dish( dish )
{
	init();
}

DishRatingView::~DishRatingView()
{

}

void DishRatingView::init()
{
	// Add buttons
	QPixmap wowPixmap = QPixmap( "Resources/wow.png" );
	wowButton = new QPushButton( this );
	wowButton->setCheckable( true );
	wowButton->setAutoExclusive( true );
	wowButton->setStyleSheet( kButtonsStyleSheet );
	wowButton->setIcon( wowPixmap );
	wowButton->setIconSize( wowPixmap.size() );
	wowButton->adjustSize();

	QPixmap happyPixmap = QPixmap( "Resources/happy.png" );
	happyButton = new QPushButton( this );
	happyButton->setCheckable( true );
	happyButton->setAutoExclusive( true );
	happyButton->setStyleSheet( kButtonsStyleSheet );
	happyButton->setIcon( happyPixmap );
	happyButton->setIconSize( happyPixmap.size() );
	happyButton->adjustSize();

	QPixmap meahPixmap = QPixmap( "Resources/meah.png" );
	meahButton = new QPushButton( this );
	meahButton->setCheckable( true );
	meahButton->setAutoExclusive( true );
	meahButton->setStyleSheet( kButtonsStyleSheet );
	meahButton->setIcon( meahPixmap );
	meahButton->setIconSize( meahPixmap.size() );
	meahButton->adjustSize();

// 	QPixmap yuckPixmap = QPixmap( "Resources/yuck.png" );		// disabled so no negative feedback
// 	yuckButton = new QPushButton( this );
// 	yuckButton->setCheckable( true );
// 	yuckButton->setAutoExclusive( true );
// 	yuckButton->setStyleSheet( kButtonsStyleSheet );
// 	yuckButton->setIcon( yuckPixmap );
// 	yuckButton->setIconSize( yuckPixmap.size() );
// 	yuckButton->adjustSize();

	// Add rating text
	QFont textFont( kFontName, 7 );
//	textFont.setWeight( QFont::DemiBold );

	numWowsLabel = new QLabel( this );
	numWowsLabel->setText( "____" );
	numWowsLabel->setFont( textFont );
	numWowsLabel->setAlignment( Qt::AlignCenter );
	numWowsLabel->adjustSize();

	numHappiesLabel = new QLabel( this );
	numHappiesLabel->setText( "____" );
	numHappiesLabel->setFont( textFont );
	numHappiesLabel->setAlignment( Qt::AlignCenter );
	numHappiesLabel->adjustSize();

	numMeahsLabel = new QLabel( this );
	numMeahsLabel->setText( "____" );
	numMeahsLabel->setFont( textFont );
	numMeahsLabel->setAlignment( Qt::AlignCenter );
	numMeahsLabel->adjustSize();

// 	numYucksLabel = new QLabel( this );
// 	numYucksLabel->setText( "____" );
// 	numYucksLabel->setFont( textFont );
// 	numYucksLabel->setAlignment( Qt::AlignCenter );
// 	numYucksLabel->adjustSize();

	QPalette palette = numWowsLabel->palette();
	palette.setColor( numWowsLabel->foregroundRole(), Qt::black );

	numWowsLabel->setPalette( palette );
	numHappiesLabel->setPalette( palette );
	numMeahsLabel->setPalette( palette );

	// Move
	wowButton->move( 0, kBorderSize );
	happyButton->move( wowButton->width(), kBorderSize );
	meahButton->move( happyButton->x() + happyButton->width(), kBorderSize );
//	yuckButton->move( meahButton->x() + meahButton->width(), kBorderSize  );

	numWowsLabel->move( wowButton->x() + wowButton->width() - numWowsLabel->width(), wowButton->y() + wowButton->height() - numWowsLabel->height() );
	numHappiesLabel->move( happyButton->x() + happyButton->width() - numHappiesLabel->width(), happyButton->y() + happyButton->height() - numHappiesLabel->height() );
	numMeahsLabel->move( meahButton->x() + meahButton->width() - numMeahsLabel->width(), meahButton->y() + meahButton->height() - numMeahsLabel->height() );
//	numYucksLabel->move( yuckButton->x() + yuckButton->width() - numYucksLabel->width(), yuckButton->y() + yuckButton->height() - numYucksLabel->height() );

	this->adjustSize();
	this->setMinimumHeight( this->height() + kBorderSize );

	updateCounterLabels();
	updateRatingButtons();

	connectSignals();
}

void DishRatingView::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );

	QColor bkgColor( 255, 255, 255 );
	bkgColor.setAlphaF( 0.8f );
	painter.fillRect( rect(), bkgColor );

	QWidget::paintEvent( event );
}

void DishRatingView::connectSignals()
{
	connect( wowButton, SIGNAL( toggled( bool ) ), SLOT( wowToggled( bool ) ) );
	connect( happyButton, SIGNAL( toggled( bool ) ), SLOT( happyToggled( bool ) ) );
	connect( meahButton, SIGNAL( toggled( bool ) ), SLOT( meahToggled( bool ) ) );
//	connect( yuckButton, SIGNAL( toggled( bool ) ), SLOT( yuckToggled( bool ) ) );
}

void DishRatingView::wowToggled( bool checked /* = false */ )
{
	if( checked )
	{
		dish.setNumWows( dish.getNumWows() + 1 );
	}
	else
	{
		dish.setNumWows( dish.getNumWows() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::happyToggled( bool checked /* = false */ )
{
	if( checked )
	{
		dish.setNumHappies( dish.getNumHappies() + 1 );
	}
	else
	{
		dish.setNumHappies( dish.getNumHappies() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::meahToggled( bool checked /* = false */ )
{
	if( checked )
	{
		dish.setNumMeahs( dish.getNumMeahs() + 1 );
	}
	else
	{
		dish.setNumMeahs( dish.getNumMeahs() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::yuckToggled( bool checked /* = false */ )
{
	if( checked )
	{
		dish.setNumYucks( dish.getNumYucks() + 1 );
	}
	else
	{
		dish.setNumYucks( dish.getNumYucks() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::updateCounterLabels()
{
	numWowsLabel->setText( QString::number( dish.getNumWows() ));
	numHappiesLabel->setText( QString::number( dish.getNumHappies() ));
	numMeahsLabel->setText( QString::number( dish.getNumMeahs() ));
//	numYucksLabel->setText( QString::number( dish.getNumYucks() ) );
}

void DishRatingView::updateRatingButtons()
{
	wowButton->setChecked( dish.getUserRating() == Dish::eWow );
	happyButton->setChecked( dish.getUserRating() == Dish::eHappy );
	meahButton->setChecked( dish.getUserRating() == Dish::eMeah );
//	yuckButton->setChecked( dish.getUserRating() == Dish::eYuck );
}
