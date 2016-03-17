#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtCore/QUrl>

#include "View/MainWindow.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	
	MainWindow mainWindow;
	//mainWindow.showFullScreen();
	mainWindow.show();

	return app.exec();
}