#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtCore/QUrl>

#include "MainWindow.h"
#include "Model/Week.h"
#include "Model/Day.h"
#include "Model/Dish.h"


int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	
	// Register meta-types
	qRegisterMetaType<Week>();
	qRegisterMetaType<Day>();
	qRegisterMetaType<Dish>();

	// Show
	MainWindow mainWindow;
	//mainWindow.showFullScreen();
	mainWindow.show();

	return app.exec();
}