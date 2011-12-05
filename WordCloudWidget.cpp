#include "WordCloudWidget.h"
#include "FlowLayout.h"
#include "Thesaurus.h"
#include "OptionDlg.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEngine>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextStream>

WordCloudWidget::WordCloudWidget(QWidget *parent) : QWidget(parent)
{
	layout = new FlowLayout(this, 10, 5, 8);
	setLayout(layout);

	thesaurus = new BigHugeThesaurus(this);
	connect(thesaurus, SIGNAL(response(QStringList)), this, SLOT(onThesaurusResponse(QStringList)));
}

void WordCloudWidget::addWord(const QString& text, int size)
{
	if(size == -1)
		size = minFont;
	if(!wordList.contains(text))
	{
		WordLabel* wordLabel = new WordLabel(text, size, this);
		layout->addWidget(wordLabel);
		wordList.insert(text, wordLabel);
		sort();   // rearrange the order
	}
}

void WordCloudWidget::highLight(const QStringList& words)
{
	unselectAll();
	unrelateAll();
	foreach(WordLabel* label, wordList)         // highlight the list
		label->setHighLighted(words.contains(label->text()));
}

// select/unselect labels
void WordCloudWidget::mousePressEvent(QMouseEvent* event)
{
	// click on blank area
	WordLabel* clicked = static_cast<WordLabel*>(childAt(event->pos()));
	if(!clicked)
		return unselectAll();

	// right click on a selected item, ignore
	if(event->button() == Qt::RightButton && clicked->isSelected())
		return;

	// single selection, if no modifier
	if(event->modifiers() == Qt::NoModifier)      
		unselectAll();
	controlPressed = event->modifiers() == Qt::ControlModifier;
	clicked->setSelected(true);                   // select this

	// issue request for related words
	thesaurus->request(clicked->text());
}

void WordCloudWidget::onThesaurusResponse(const QStringList& list)
{
	foreach(WordLabel* label, wordList)
		label->setRelated(list.contains(label->text()));
}

// emit a signal with the double clicked word
void WordCloudWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	WordLabel* label = static_cast<WordLabel*>(childAt(event->pos()));
	emit doubleClicked(label != 0 ? label->text() : QString());
}

void WordCloudWidget::unselectAll() {
	foreach(WordLabel* word, wordList)
		word->setSelected(false);
}

void WordCloudWidget::unrelateAll() {
	foreach(WordLabel* word, wordList)
		word->setRelated(false);
}

QList<WordLabel*> WordCloudWidget::getSelected() const
{
	QList<WordLabel*> result;
	foreach(WordLabel* label, wordList)
		if(label->isSelected())
			result << label;
	return result;
}

void WordCloudWidget::removeWord(WordLabel* label)
{
	if(label == 0 || !wordList.contains(label->text()))
		return;
	layout->removeWidget(label);
	wordList.remove(label->text());
	label->deleteLater();
}

// recalculate the sizes
void WordCloudWidget::normalizeSizes()
{
	int minSize = 1000;
	int maxSize = 0;
	foreach(WordLabel* label, wordList)            // find min/max
	{
		minSize = qMin(minSize, label->getSize());
		maxSize = qMax(maxSize, label->getSize());
	}
	foreach(WordLabel* label, wordList)
	{
		int size = (maxSize == minSize) ? maxFont :
			(maxFont-minFont) * (label->getSize() - minSize) / (maxSize-minSize) + minFont;
		label->setSize(size);
	}
}

WordLabel* WordCloudWidget::findWord(const QString& text) const
{
	WordList::ConstIterator it = wordList.find(text);
	return (it != wordList.end()) ? it.value() : 0;
}

void WordCloudWidget::onSizeChanged() {
	normalizeSizes();
}

void WordCloudWidget::sort()
{
	// clear all words from the layout
	foreach(WordLabel* label, wordList)
		layout->removeWidget(label);

	// insert them back in order
	for(WordList::Iterator it = wordList.begin(); it != wordList.end(); ++it)
		layout->addWidget(it.value());
}

void WordCloudWidget::renameWord(WordLabel* label, const QString& name)
{
	if(wordList.contains(name))      // duplicated
		return;
	wordList.remove(label->text());  // remove the old one from the map
	wordList.insert(name, label);    // insert the word back, but in a new location in the map
	label->setText(name);
	sort();                          // reorder the widgets in the layout
}

void WordCloudWidget::setSizeRange(int min, int max)
{
	minFont = min;
	maxFont = max;
}

void WordCloudWidget::search(const QString& target)
{
	if(target.isEmpty())
		unselectAll();
	else
		foreach(WordLabel* label, wordList)
			label->setSelected(label->text().contains(target, Qt::CaseInsensitive));
}

//////////////////////////////////////////////////////////////////////////
WordLabel::WordLabel(const QString& text, int s, QWidget* parent)
: QLabel(text, parent)
{
	selected    = false;
	highLighted = false;
	related     = false;
	setSize(s);
}

void WordLabel::setSize(int s)
{
	size = s;
	QFont f = font();
	f.setPointSize(size);
	setFont(f);
	emit sizeChanged();
}

void WordLabel::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	// special effects
	if(selected || highLighted || related)
	{
		if(highLighted)    // shadow
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::lightGray);
		}
		else if(related)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(Qt::yellow);
		}		
		if(selected) {     // red selection rectangle
			painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
		}
		painter.drawRect(rect());
	}

	// text
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

void WordLabel::setRelated(bool relate)
{
	related = relate;
	update();   // repaint
}
