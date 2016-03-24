#include "DishRatingView.h"

#include "Style.h"


DishRatingView::DishRatingView( QWidget *parent, const Dish& dish )
	: QWidget(parent)
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

	QLabel* numLikesLabel = new QLabel( this );
	numLikesLabel->setText( " 52" );
	numLikesLabel->setFont( textFont );
	numLikesLabel->setPalette( textPalette );
	numLikesLabel->setAlignment( Qt::AlignCenter );
	numLikesLabel->adjustSize();

	QLabel* numDislikesLabel = new QLabel( this );
	numDislikesLabel->setText( "10 " );
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

	likeButton->move( numLikesLabel->width(), 0 );
	dislikeButton->move( likeButton->x() + likeButton->width(), 0 );
	numDislikesLabel->move( dislikeButton->x() + dislikeButton->width(), 0 );

	this->adjustSize();

	connectSignals();
}

void DishRatingView::connectSignals()
{
	connect( likeButton, SIGNAL( clicked( bool ) ), SLOT( likeClicked( bool ) ) );
	connect( dislikeButton, SIGNAL( clicked( bool ) ), SLOT( dislikeClicked( bool ) ) );
}

void DishRatingView::likeClicked( bool checked /* = false */ )
{
	if( checked )
		dislikeButton->setChecked( false );
}

void DishRatingView::dislikeClicked( bool checked /* = false */ )
{
	if( checked )
		likeButton->setChecked( false );
}