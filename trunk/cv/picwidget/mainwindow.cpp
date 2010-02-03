#include <QtGui>
#include <QDockWidget>

#include "mainwindow.h"
#include "PicWidget.h"

MainWindow::MainWindow()
{
	this->resize(600, 800);
	picWidget = new PicWidget;
	setCentralWidget(picWidget);
	
	QDockWidget * contentsWindow = new QDockWidget(tr("Table of Contents"), this);
     contentsWindow->setAllowedAreas(Qt::LeftDockWidgetArea
                                   | Qt::RightDockWidgetArea);
     addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);

     QListWidget * headingList = new QListWidget(contentsWindow);
     contentsWindow->setWidget(headingList);
	
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	event->accept();
}
