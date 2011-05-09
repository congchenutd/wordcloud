#include "WordCloudWidget.h"
#include <QtGui/QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
	QApplication a(argc, argv);
	WordCloudWidget w;
	w.show();
	w.addWord("Hello", 30);
	w.addWord("world", 24);
	w.addWord("UTD", 16);
	w.addWord("Cong", 22);
	w.addWord("Computer Science", 14);
	w.addWord("Software", 18);
	w.addWord("Visualization", 26);
	w.highLight(QStringList() << "UTD" << "Hello");
	return a.exec();
}
