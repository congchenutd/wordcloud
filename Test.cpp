#include "Test.h"

Test::Test(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QPalette newPalette = palette();
	newPalette.setColor(QPalette::Window, Qt::white);
	setPalette(newPalette);

	ui.widget->addWord("Hello", 30);
}

Test::~Test()
{

}
