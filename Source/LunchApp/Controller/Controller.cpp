#include "Controller.h"

Controller::Controller(QObject *parent)
	: QObject(parent)
{

}

Controller::~Controller()
{

}

void Controller::SelectionChangedOn( const Dish& dish )
{
	// Store selection in database
}
