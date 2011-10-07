#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QMap>
#include <QScrollArea>

class FlowLayout;
class WordLabel;
class QNetworkAccessManager;
class QNetworkReply;

typedef QMap<QString, WordLabel*> WordList;
typedef enum {EXACTLY, START_WITH} SearchCriteria;

class WordCloudWidget : public QWidget
{
	Q_OBJECT

public:
	WordCloudWidget(QWidget* parent = 0);
	void addWord(const QString& text, int size = 12);
	void highLight(const QStringList& words);
	void removeWord(WordLabel* word);
	//void removeWord(const QString& text);
	void renameWord(WordLabel* word, const QString& name);
	QList<WordLabel*> getSelected() const;
	void normalizeSizes();
	WordLabel* findWord(const QString& text) const;
//	QList<WordLabel*> findWord(const QString& text, SearchCriteria criteria = EXACTLY);
	void unselectAll();    // set all words unselected
	void unrelateAll();    // set all words unrelated
	void sort();           // reorder the word widgets in the layout

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent*);

private slots:
	void onSizeChanged();
	void onNetworkReply(QNetworkReply*);

signals:
	void doubleClicked(QString);

private:
	FlowLayout* layout;
	WordList wordList;
	QNetworkAccessManager* networkAccessManager;
};

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

signals:
	void sizeChanged();

private:
	bool selected;      // draw a red selection rectangle
	bool highLighted;   // shadowed
	int  size;          // font size
	bool related;       // related to the selected
};

#endif // WORDCLOUDWIDGET_H
