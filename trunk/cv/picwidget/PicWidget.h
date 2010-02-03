#ifndef PICWIDGET_H
#define	PICWIDGET_H

#include <QWidget>

class PicWidget : public QWidget
{
	Q_OBJECT

public:
	PicWidget(QWidget * parent = 0);

protected:
	void paintEvent(QPaintEvent * event);
};

#endif
