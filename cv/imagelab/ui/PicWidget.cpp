#include <QtGui>
#include <cmath>

#include "PicWidget.h"
#include "EdgeDector.h"

PicWidget::PicWidget(QWidget * parent)
	: QWidget(parent)
{}

void PicWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	
	QImage image;

	CannyDector cannyDector("../yufu.jpg");
	cannyDector.detectEdges(image);

	/*QPainter imagePainter(&image);
	imagePainter.initFrom(this);
	draw(&imagePainter);
	imagePainter.end();
	*/
	//this->resize(image.width(), image.height());
	image = image.scaled(this->width(), this->height());
	painter.drawImage(0, 0, image);
}
