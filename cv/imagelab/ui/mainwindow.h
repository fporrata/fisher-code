#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PicWidget.h"
#include <QListWidget>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent * event);

private:
	PicWidget * picWidget;
//	QDockWidget * contentsWindow;
//	QListWidget * headingList;
};

#endif
