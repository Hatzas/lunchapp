#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtCore/QUrl>

#include "View/Style.h"
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

	QLocale::setDefault( QLocale( QLocale::Romanian, QLocale::Romania ) );

	// Show
	MainWindow mainWindow;
	mainWindow.setWindowTitle( kAppName );

	//mainWindow.showFullScreen();
	mainWindow.show();

	return app.exec();
}