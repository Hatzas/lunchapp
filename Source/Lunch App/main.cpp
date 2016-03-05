#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>
#include <QtCore/QUrl>

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	QQuickWindow::setDefaultAlphaBuffer( true );
	QQmlApplicationEngine engine( QUrl("qrc:///View/photosurface.qml") );

	return app.exec();
}