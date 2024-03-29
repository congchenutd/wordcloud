#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QScrollArea>

// A simple word cloud implementation

class FlowLayout;
class WordLabel;
class Thesaurus;

typedef QMap<QString, WordLabel*> WordList;
typedef enum {EXACTLY, START_WITH} SearchCriteria;

class WordCloudWidget : public QWidget
{
	Q_OBJECT

public:
	WordCloudWidget(QWidget* parent = 0);
	void addWord(const QString& text, int size = -1);
	void highLight(const QStringList& words);
	void removeWord(WordLabel* word);
	void renameWord(WordLabel* word, const QString& name);
	QList<WordLabel*> getSelected() const;
	void normalizeSizes();                               // recalculate sizes
	WordLabel* findWord(const QString& text) const;
	void unselectAll();                                  // set all words unselected
	void unrelateAll();                                  // set all words unrelated
	void sort();                                         // reorder the labels
	bool controlIsPressed() const { return controlPressed; }
	void setSizeRange(int min, int max);
	void search(const QString& target);

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent*);

private slots:
	void onSizeChanged();
	void onThesaurusResponse(const QStringList& list);

signals:
    void doubleClicked(const QString&);

private:
	FlowLayout* layout;
	WordList wordList;
	Thesaurus* thesaurus;
	int minFont, maxFont;
	bool controlPressed;   // ctrl pressed when the context menu is triggered
};

// The words
class WordLabel : public QLabel
{
	Q_OBJECT

public:
    WordLabel(const QString& text, int s, QWidget* parent);
	void setSize       (int s);                          // font size
	void setHighLighted(bool highLighted);
	void setRelated    (bool relate);
	void setSelected   (bool selected);
	int  getSize()       const { return size;        }
	bool isHighLighted() const { return highLighted; }   // shadow
	bool isSelected   () const { return selected;    }   // red rectangle

protected:
	virtual void paintEvent(QPaintEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);

signals:
	void sizeChanged();
    void doubleClicked(const QString&);

private:
	bool selected;      // draw a red selection rectangle
	bool highLighted;   // shadowed
	int  size;          // font size
	bool related;       // related to the selected
};

#endif // WORDCLOUDWIDGET_H
