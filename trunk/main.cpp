#include "WordCloudWidget.h"
#include <QtGui/QApplication>
#include <QTime>
#include <QScrollArea>
#include "Test.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Test w;
	w.show();
	return a.exec();
}
