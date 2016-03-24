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
	this->setStyleSheet( "background-color: rgba(64,64,64,80%)" );

	// Add rating text
	QPalette textPalette;
	textPalette.setColor( QPalette::Text, Qt::white );
	QFont textFont( kFontName, 11 );
	textFont.setBold( true );

	numLikesLabel = new QLabel( this );
	numLikesLabel->setText( "___" );
	numLikesLabel->setFont( textFont );
	numLikesLabel->setPalette( textPalette );
	numLikesLabel->setAlignment( Qt::AlignCenter );
	numLikesLabel->adjustSize();

	numDislikesLabel = new QLabel( this );
	numDislikesLabel->setText( "____" );
	numDislikesLabel->setFont( textFont );
	numDislikesLabel->setPalette( textPalette );
	numDislikesLabel->setAlignment( Qt::AlignCenter );
	numDislikesLabel->adjustSize();

	// Add buttons
	QPixmap likePixmap = QPixmap( "Resources/like2.png" );
	likeButton = new QPushButton( this );
	likeButton->setCheckable( true );
	likeButton->setStyleSheet( kRatingButtonsStyleSheet );
	likeButton->setIcon( likePixmap );
	likeButton->setIconSize( likePixmap.size() );
	likeButton->setFixedHeight( numLikesLabel->height() );
	likeButton->adjustSize();

	QPixmap dislikePixmap = QPixmap( "Resources/dislike2.png" );
	dislikeButton = new QPushButton( this );
	dislikeButton->setCheckable( true );
	dislikeButton->setStyleSheet(kRatingButtonsStyleSheet);
	dislikeButton->setIcon( dislikePixmap );
	dislikeButton->setIconSize( dislikePixmap.size() );
	dislikeButton->setFixedHeight( numDislikesLabel->height() );
	dislikeButton->adjustSize();

	// Move
	likeButton->move( numLikesLabel->width(), 0 );
	dislikeButton->move( likeButton->x() + likeButton->width(), 0 );
	numDislikesLabel->move( dislikeButton->x() + dislikeButton->width(), 0 );

	this->adjustSize();

	updateCounterLabels();
	if( dish.getUserRating() == Dish::eLiked )
		likeButton->setChecked( true );
	else if( dish.getUserRating() == Dish::eDisliked )
		dislikeButton->setChecked( true );

	connectSignals();
}

void DishRatingView::connectSignals()
{
	connect( likeButton, SIGNAL( toggled( bool ) ), SLOT( likeToggled( bool ) ) );
	connect( dislikeButton, SIGNAL( toggled( bool ) ), SLOT( dislikeToggled( bool ) ) );
}

void DishRatingView::likeToggled( bool checked /* = false */ )
{
	if( checked )
	{
		dislikeButton->setChecked( false );
		dish.setNumLikes( dish.getNumLikes() + 1 );
	}
	else
	{
		dish.setNumLikes( dish.getNumLikes() - 1 );
	}

	updateCounterLabels();
}

void DishRatingView::dislikeToggled( bool checked /* = false */ )
{
	if( checked )
	{
		likeButton->setChecked( false );
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
		numLikesLabel->setText( "  " + QString::number( dish.getNumLikes() ) + " " );
	}
	else if( dish.getNumLikes() < 100 )
	{
		numLikesLabel->setText( " " + QString::number( dish.getNumLikes() ) + " " );
	}
	else
	{
		numLikesLabel->setText( QString::number( dish.getNumLikes() ) + " " );
		numLikesLabel->adjustSize();
	}

	if( dish.getNumDislikes() < 10 )
	{
		numDislikesLabel->setText( " " + QString::number( dish.getNumDislikes() ) + "  " );
	}
	else if( dish.getNumDislikes() < 100 )
	{
		numDislikesLabel->setText( " " + QString::number( dish.getNumDislikes() ) + " " );
	}
	else
	{
		numDislikesLabel->setText( " " + QString::number( dish.getNumDislikes() ) );
		numDislikesLabel->adjustSize();
	}
}