#include "MainWindow.h"

#include "MetroView.h"
#include <Network/DataTransfer.h>


MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.verticalLayout->addWidget( new MetroView(parent) );

	dataTransfer = new DataTransfer(this);
	dataTransfer->getMenu();
}

MainWindow::~MainWindow()
{
}