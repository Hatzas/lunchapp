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
	this->setAutoFillBackground( true );
	this->setStyleSheet( kRatingBackgroundStyleSheet );

	// Add rating text
	QPalette textPalette;
	textPalette.setColor( QPalette::Text, Qt::white );
	QFont textFont( kFontName, 9 );
	textFont.setBold( true );

	numWowsLabel = new QLabel( this );
	numWowsLabel->setText( "_____" );
	numWowsLabel->setFont( textFont );
	numWowsLabel->setPalette( textPalette );
	numWowsLabel->setAlignment( Qt::AlignCenter );
	numWowsLabel->adjustSize();

	numHappiesLabel = new QLabel( this );
	numHappiesLabel->setText( "_____" );
	numHappiesLabel->setFont( textFont );
	numHappiesLabel->setPalette( textPalette );
	numHappiesLabel->setAlignment( Qt::AlignCenter );
	numHappiesLabel->adjustSize();

	numMeahsLabel = new QLabel( this );
	numMeahsLabel->setText( "_____" );
	numMeahsLabel->setFont( textFont );
	numMeahsLabel->setPalette( textPalette );
	numMeahsLabel->setAlignment( Qt::AlignCenter );
	numMeahsLabel->adjustSize();

	numYucksLabel = new QLabel( this );
	numYucksLabel->setText( "_____" );
	numYucksLabel->setFont( textFont );
	numYucksLabel->setPalette( textPalette );
	numYucksLabel->setAlignment( Qt::AlignCenter );
	numYucksLabel->adjustSize();

	// Add buttons
	QPixmap wowPixmap = QPixmap( "Resources/wow.png" );
	wowButton = new QPushButton( this );
	wowButton->setCheckable( true );
	wowButton->setStyleSheet( kRatingButtonsStyleSheet );
	wowButton->setIcon( wowPixmap );
	wowButton->setIconSize( wowPixmap.size() );
	//wowButton->setFixedHeight( numLikesLabel->height() );
	wowButton->adjustSize();

	QPixmap happyPixmap = QPixmap( "Resources/happy.png" );
	happyButton = new QPushButton( this );
	happyButton->setCheckable( true );
	happyButton->setStyleSheet( kRatingButtonsStyleSheet );
	happyButton->setIcon( happyPixmap );
	happyButton->setIconSize( happyPixmap.size() );
	//happyButton->setFixedHeight( numLikesLabel->height() );
	happyButton->adjustSize();

	QPixmap meahPixmap = QPixmap( "Resources/meah.png" );
	meahButton = new QPushButton( this );
	meahButton->setCheckable( true );
	meahButton->setStyleSheet( kRatingButtonsStyleSheet );
	meahButton->setIcon( meahPixmap );
	meahButton->setIconSize( meahPixmap.size() );
	//meahButton->setFixedHeight( numLikesLabel->height() );
	meahButton->adjustSize();

	QPixmap yuckPixmap = QPixmap( "Resources/yuck.png" );
	yuckButton = new QPushButton( this );
	yuckButton->setCheckable( true );
	yuckButton->setStyleSheet( kRatingButtonsStyleSheet );
	yuckButton->setIcon( yuckPixmap );
	yuckButton->setIconSize( yuckPixmap.size() );
	//yuckButton->setFixedHeight( numDislikesLabel->height() );
	yuckButton->adjustSize();

	// Move
	wowButton->move( 0, 0 );
	happyButton->move( wowButton->width(), 0 );
	meahButton->move( happyButton->x() + happyButton->width(), 0 );
	yuckButton->move( meahButton->x() + meahButton->width(), 0  );

	numWowsLabel->move( wowButton->x() + wowButton->width() / 2 - numWowsLabel->width() / 2, meahButton->height() );
	numHappiesLabel->move( happyButton->x() + happyButton->width() / 2 - numWowsLabel->width() / 2, meahButton->height() );
	numMeahsLabel->move( meahButton->x() + meahButton->width() / 2 - numWowsLabel->width() / 2, meahButton->height() );
	numYucksLabel->move( yuckButton->x() + yuckButton->width() / 2 - numWowsLabel->width() / 2, meahButton->height() );

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
	connect( yuckButton, SIGNAL( toggled( bool ) ), SLOT( yuckToggled( bool ) ) );
}

void DishRatingView::wowToggled( bool checked /* = false */ )
{
	if( checked )
	{
		yuckButton->setChecked( false );
		dish.setNumLikes( dish.getNumLikes() + 1 );
	}
	else
	{
		dish.setNumLikes( dish.getNumLikes() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::yuckToggled( bool checked /* = false */ )
{
	if( checked )
	{
		wowButton->setChecked( false );
		dish.setNumDislikes( dish.getNumDislikes() + 1 );
	}
	else
	{
		dish.setNumDislikes( dish.getNumDislikes() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::updateCounterLabels()
{
	if( dish.getNumLikes() < 10 )
	{
		numWowsLabel->setText( QString::number( dish.getNumLikes() ));
	}
	else if( dish.getNumLikes() < 100 )
	{
		numWowsLabel->setText( QString::number( dish.getNumLikes() ) );
	}
	else
	{
		numWowsLabel->setText( QString::number( dish.getNumLikes() ) );
		numWowsLabel->adjustSize();
	}

	if( dish.getNumDislikes() < 10 )
	{
		numYucksLabel->setText( QString::number( dish.getNumDislikes() ) );
	}
	else if( dish.getNumDislikes() < 100 )
	{
		numYucksLabel->setText( QString::number( dish.getNumDislikes() ) );
	}
	else
	{
		numYucksLabel->setText( QString::number( dish.getNumDislikes() ) );
		numYucksLabel->adjustSize();
	}
}