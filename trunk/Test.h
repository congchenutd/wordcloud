#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include "ui_Test.h"

class Test : public QWidget
{
	Q_OBJECT

public:
	Test(QWidget *parent = 0);
	~Test();

private:
	Ui::Test ui;
};

#endif // TEST_H
