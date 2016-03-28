#include "DishRatingView.h"

#include "Style.h"


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
 	//this->setAutoFillBackground( true );
	//this->setStyleSheet( kRatingBackgroundStyleSheet );

	// Add buttons
	QPixmap wowPixmap = QPixmap( "Resources/wow.png" );
	wowButton = new QPushButton( this );
	wowButton->setCheckable( true );
	wowButton->setAutoExclusive( true );
	wowButton->setStyleSheet( kRatingButtonsStyleSheet );
	wowButton->setIcon( wowPixmap );
	wowButton->setIconSize( wowPixmap.size() );
	//wowButton->setFixedHeight( numLikesLabel->height() );
	wowButton->adjustSize();

	QPixmap happyPixmap = QPixmap( "Resources/happy.png" );
	happyButton = new QPushButton( this );
	happyButton->setCheckable( true );
	happyButton->setAutoExclusive( true );
	happyButton->setStyleSheet( kRatingButtonsStyleSheet );
	happyButton->setIcon( happyPixmap );
	happyButton->setIconSize( happyPixmap.size() );
	happyButton->adjustSize();

	QPixmap meahPixmap = QPixmap( "Resources/meah.png" );
	meahButton = new QPushButton( this );
	meahButton->setCheckable( true );
	meahButton->setAutoExclusive( true );
	meahButton->setStyleSheet( kRatingButtonsStyleSheet );
	meahButton->setIcon( meahPixmap );
	meahButton->setIconSize( meahPixmap.size() );
	meahButton->adjustSize();

	QPixmap yuckPixmap = QPixmap( "Resources/yuck.png" );
	yuckButton = new QPushButton( this );
	yuckButton->setCheckable( true );
	yuckButton->setAutoExclusive( true );
	yuckButton->setStyleSheet( kRatingButtonsStyleSheet );
	yuckButton->setIcon( yuckPixmap );
	yuckButton->setIconSize( yuckPixmap.size() );
	yuckButton->adjustSize();

	// Add rating text
	QFont textFont( kFontName, 6 );

	numWowsLabel = new QLabel( this );
	numWowsLabel->setText( "__0_" );
	numWowsLabel->setFont( textFont );
	numWowsLabel->setAlignment( Qt::AlignCenter );
	numWowsLabel->adjustSize();

	numHappiesLabel = new QLabel( this );
	numHappiesLabel->setText( "__0_" );
	numHappiesLabel->setFont( textFont );
	numHappiesLabel->setAlignment( Qt::AlignCenter );
	numHappiesLabel->adjustSize();

	numMeahsLabel = new QLabel( this );
	numMeahsLabel->setText( "__0_" );
	numMeahsLabel->setFont( textFont );
	numMeahsLabel->setAlignment( Qt::AlignCenter );
	numMeahsLabel->adjustSize();

	numYucksLabel = new QLabel( this );
	numYucksLabel->setText( "__0_" );
	numYucksLabel->setFont( textFont );
	numYucksLabel->setAlignment( Qt::AlignCenter );
	numYucksLabel->adjustSize();

	// Move
	wowButton->move( 0, 0 );
	happyButton->move( wowButton->width(), 0 );
	meahButton->move( happyButton->x() + happyButton->width(), 0 );
	yuckButton->move( meahButton->x() + meahButton->width(), 0  );

	numWowsLabel->move( wowButton->x() + wowButton->width() - numWowsLabel->width(), wowButton->y() + wowButton->height() - numWowsLabel->height() );
	numHappiesLabel->move( happyButton->x() + happyButton->width() - numHappiesLabel->width(), happyButton->y() + happyButton->height() - numHappiesLabel->height() );
	numMeahsLabel->move( meahButton->x() + meahButton->width() - numMeahsLabel->width(), meahButton->y() + meahButton->height() - numMeahsLabel->height() );
	numYucksLabel->move( yuckButton->x() + yuckButton->width() - numYucksLabel->width(), yuckButton->y() + yuckButton->height() - numYucksLabel->height() );

	this->adjustSize();

	updateCounterLabels();
	if( dish.getUserRating() == Dish::eLiked )
		wowButton->setChecked( true );
	else if( dish.getUserRating() == Dish::eDisliked )
		yuckButton->setChecked( true );

	connectSignals();
}

void DishRatingView::connectSignals()
{
	connect( wowButton, SIGNAL( toggled( bool ) ), SLOT( wowToggled( bool ) ) );
	connect( happyButton, SIGNAL( toggled( bool ) ), SLOT( happyToggled( bool ) ) );
	connect( meahButton, SIGNAL( toggled( bool ) ), SLOT( meahToggled( bool ) ) );
	connect( yuckButton, SIGNAL( toggled( bool ) ), SLOT( yuckToggled( bool ) ) );
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
	numYucksLabel->setText( QString::number( dish.getNumYucks() ) );
}