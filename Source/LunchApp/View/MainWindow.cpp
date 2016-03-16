#include "MainWindow.h"

#include "MetroView.h"

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.verticalLayout->addWidget( new MetroView(parent) );
}

MainWindow::~MainWindow()
{
}