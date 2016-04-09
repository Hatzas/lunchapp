#include "Dish.h"

#include <QBuffer>


Dish::Dish()
{

}

Dish::Dish( QByteArray inputBuffer )
{
	QBuffer buffer( &inputBuffer );
	buffer.open( QIODevice::ReadOnly );

	QDataStream in( &buffer );

	in >> name;
	in >> ingredients;
	in >> pixmap;
	in >> courseNum;
	in >> type;
	in >> identifier;

	in >> userSelected;
	in >> (int&)userInterest;

	in >> numWows;
	in >> numHappies;
	in >> numMeahs;
	in >> numYucks;

	in >> (int&)userRating;
}

Dish::Dish( QString name, QString ingredients, QPixmap pixmap, int courseNum )
	: name( name )
	, ingredients( ingredients )
	, pixmap( pixmap )
	, courseNum( courseNum )
	, userSelected( false )
	, userInterest( eHigh )
	, numWows( 0 )
	, numHappies( 0 )
	, numMeahs( 0 )
	, numYucks( 0 )
	, userRating( eNotRated )
{

}

Dish::operator QByteArray() const
{
	QByteArray output;

	QBuffer buffer( &output );
	buffer.open( QIODevice::WriteOnly );

	QDataStream out( &buffer );

	out << name;
	out << ingredients;
	out << pixmap;
	out << courseNum;
	out << type;
	out << identifier;

	out << userSelected;
	out << userInterest;

	out << numWows;
	out << numHappies;
	out << numMeahs;
	out << numYucks;

	out << userRating;

	return output;
}
