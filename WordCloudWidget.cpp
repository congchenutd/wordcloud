#include "WordCloudWidget.h"
#include "FlowLayout.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEngine>

enum {minFont = 12, maxFont = 24};

WordCloudWidget::WordCloudWidget(QWidget *parent) : QWidget(parent)
{
	layout = new FlowLayout(this, 10, 5, 8);
	setLayout(layout);

//	QPalette newPalette = palette();
//	newPalette.setColor(QPalette::Window, Qt::white);
//	setPalette(newPalette);
}

void WordCloudWidget::addWord(const QString& text, int size)
{
	if(!wordList.contains(text))
	{
		WordLabel* wordLabel = new WordLabel(text, size, this);
		layout->addWidget(wordLabel);
		wordList.insert(text, wordLabel);
		sort();
	}
}

void WordCloudWidget::highLight(const QStringList& words)
{
	foreach(WordLabel* word, wordList)   // unhighLight others
		word->setHighLighted(false);
	foreach(QString word, words)         // highlight the list
		if(WordLabel* label = findWord(word))
			label->setHighLighted(true);
}

void WordCloudWidget::mousePressEvent(QMouseEvent* event)
{
	WordLabel* clicked = static_cast<WordLabel*>(childAt(event->pos()));
	if(!clicked)
		return unselectAll();                     // click on blank area

	// right click on a selected item, ignore
	if(event->button() == Qt::RightButton && clicked->isSelected())
		return;

	if(event->modifiers() == Qt::NoModifier)      
		unselectAll();
	clicked->setSelected(true);                   // select this
}

void WordCloudWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	WordLabel* label = static_cast<WordLabel*>(childAt(event->pos()));
	emit doubleClicked(label != 0 ? label->text() : QString());
}

void WordCloudWidget::unselectAll() {
	foreach(WordLabel* word, wordList)
		word->setSelected(false);
}

QList<WordLabel*> WordCloudWidget::getSelected() const
{
	QList<WordLabel*> result;
	foreach(WordLabel* word, wordList)
		if(word->isSelected())
			result << word;
	return result;
}

void WordCloudWidget::removeWord(WordLabel* word)
{
	if(word == 0 || !wordList.contains(word->text()))
		return;
	layout->removeWidget(word);
	wordList.remove(word->text());
	word->deleteLater();
}

//void WordCloudWidget::removeWord(const QString& text)
//{
//	WordList::Iterator it = wordList.find(text);
//	if(it != wordList.end())
//		removeWord(it.value());
//}

void WordCloudWidget::normalizeSizes()
{
	int minSize = 1000;
	int maxSize = 0;
	foreach(WordLabel* word, wordList)            // find min/max
	{
		minSize = qMin(minSize, word->getSize());
		maxSize = qMax(maxSize, word->getSize());
	}
	foreach(WordLabel* word, wordList)
	{
		int size = maxSize == minSize ? maxFont :
			maxFont * (word->getSize() - minSize) / (maxSize - minSize) + minFont;
		word->setSize(size);
	}
}

WordLabel* WordCloudWidget::findWord(const QString& text) const
{
	WordList::ConstIterator it = wordList.find(text);
	return (it != wordList.end()) ? it.value() : 0;
}

//QList<WordLabel*> WordCloudWidget::findWord(const QString& text, SearchCriteria criteria)
//{
//	QList<WordLabel*> result;
//	if(criteria == EXACTLY)
//	{
//		WordList::ConstIterator it = wordList.find(text);
//		if(it != wordList.end())
//			result << it.value();
//	}
//	else if(criteria == START_WITH) {
//		foreach(WordLabel* label, wordList)
//			if(label->text().startsWith(text, Qt::CaseInsensitive))
//				result << label->text();
//	}
//	return result;
//}

void WordCloudWidget::onSizeChanged() {
	normalizeSizes();
}

void WordCloudWidget::sort()
{
	// clear all words from the layout
	foreach(WordLabel* word, wordList)
		layout->removeWidget(word);

	// insert them again in order
	for(WordList::Iterator it = wordList.begin(); it != wordList.end(); ++it)
		layout->addWidget(it.value());
}

void WordCloudWidget::renameWord(WordLabel* word, const QString& name)
{
	wordList.remove(word->text());  // remove the old one from the map
	wordList.insert(name, word);
	word->setText(name);
	sort();
}

//////////////////////////////////////////////////////////////////////////
WordLabel::WordLabel(const QString& text, int s, QWidget* parent)
: QLabel(text, parent)
{
	selected    = false;
	highLighted = false;
	setSize(s);
}

void WordLabel::setSize(int s)
{
	size = s;
	QFont f = font();
	f.setPixelSize(size);
	setFont(f);
	emit sizeChanged();
}

void WordLabel::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	if(selected || highLighted)
	{
		if(highLighted)    // shadow
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::lightGray);
		}
		if(selected) {     // red rectangle
			painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
		}
		painter.drawRect(rect());
	}
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen());   // use default pen
	painter.drawText(0, QFontMetrics(font()).tightBoundingRect(text()).height(), text());
}

void WordLabel::setHighLighted(bool highLight)
{
	highLighted = highLight;
	update();   // repaint
}

void WordLabel::setSelected(bool select)
{
	selected = select;
	update();   // repaint
}