#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

int main (int argc, char * argv[])
{
	QApplication app(argc, argv);

	QWidget * window = new QWidget;
	window->setWindowTitle("Enter your age");
	QSpinBox * spinBox = new QSpinBox;
	QSlider * slider = new QSlider(Qt::Horizontal);
	spinBox->setRange(0, 130);
	slider->setRange(0, 130);

	QObject::connect(spinBox, SINGAL(valueChanged(int)),
			slider, SLOT(setValue(int)));
	QObject::connect(slider, SINGAL(valueChanged(int)),
			spinBox, SLOT(setvalue(int)));
	
	QHBoxLayout * layout = new QHBoxLayout;
	layout->addWidget(spinBox);
	layout->addWidget(slider);
	window->setLayout(layout);

	window->show();

	return app.exec();
}
