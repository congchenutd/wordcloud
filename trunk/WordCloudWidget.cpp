#include "WordCloudWidget.h"
#include "FlowLayout.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>

WordCloudWidget::WordCloudWidget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	layout = new FlowLayout(this);
	setLayout(layout);

	QPalette newPalette = palette();
	newPalette.setColor(QPalette::Window, Qt::white);
	setPalette(newPalette);
}

void WordCloudWidget::addWord(WordLabel* word)
{
	layout->addWidget(word);
	wordList.insert(word->text(), word);
	connect(word, SIGNAL(delRequested(QString)), this, SLOT(onDel(QString)));
}

void WordCloudWidget::addWord(const QString& text, int size) {
	addWord(new WordLabel(text, size, this));
}

void WordCloudWidget::highLight(const QStringList& words)
{
	foreach(WordLabel* word, wordList)
		word->setHighLighted(false);
	foreach(QString word, words)
	{
		WordList::Iterator it = wordList.find(word);
		if(it != wordList.end())
			it.value()->setHighLighted(true);
	}
}

void WordCloudWidget::onDel(const QString& word)
{
	WordList::Iterator it = wordList.find(word);
	if(it != wordList.end())
	{
		layout->removeWidget(it.value());
		wordList.erase(it);
		delete it.value();
	}
}


//////////////////////////////////////////////////////////////////////////
WordLabel::WordLabel(const QString& text, int size, QWidget* parent)
: QLabel(text, parent)
{
	selected    = false;
	highLighted = false;
	QFont f = font();
	f.setPixelSize(size);
	setFont(f);
}

void WordLabel::mousePressEvent(QMouseEvent* event) {
	if(event->button() == Qt::LeftButton)
		setSelected(!selected);
}

void WordLabel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if(selected || highLighted)
	{
		if(highLighted)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::gray);
		}		
		if(selected) {
			painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
		}
		painter.drawRect(rect());
	}
	QLabel::paintEvent(event);
}

void WordLabel::setHighLighted(bool highLight)
{
	highLighted = highLight;
	update();
}

void WordLabel::setSelected(bool select)
{
	selected = select;
	update();
}

void WordLabel::contextMenuEvent(QContextMenuEvent* event)
{
	QAction actionAdd(tr("Add to paper"), this);
	QAction actionDel(tr("Delete"), this);
	connect(&actionAdd, SIGNAL(triggered()), this, SLOT(onAdd()));
	connect(&actionDel, SIGNAL(triggered()), this, SLOT(onDel()));
	QMenu menu(this);
	menu.addAction(&actionAdd);
	menu.addAction(&actionDel);
	menu.exec(event->globalPos());
}

void WordLabel::onAdd() {
	emit addRequested(text());
}

void WordLabel::onDel() {
	emit delRequested(text());
}
