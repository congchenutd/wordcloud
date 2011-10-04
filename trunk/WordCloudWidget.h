#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QMap>
#include <QScrollArea>

class FlowLayout;
class WordLabel;

typedef QMap<QString, WordLabel*> WordList;
typedef enum {EXACTLY, START_WITH} SearchCriteria;

class WordCloudWidget : public QWidget
{
	Q_OBJECT

public:
	WordCloudWidget(QWidget* parent = 0);
	void addWord(WordLabel* word);
	void addWord(const QString& text, int size = 12);
	void highLight(const QStringList& words);
	void removeWord(WordLabel* word);
	void removeWord(const QString& text);
	QList<WordLabel*> getSelected() const;
	void normalizeSizes();
	WordLabel* findWord(const QString& text) const;
	QList<WordLabel*> findWord(const QString& text, SearchCriteria criteria = EXACTLY);
	void unselectAll();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent*);
	virtual void keyPressEvent(QKeyEvent* event);

private slots:
	void onSizeChanged();

signals:
	void doubleClicked(QString);

private:
	FlowLayout* layout;
	WordList wordList;
};

class WordLabel : public QLabel
{
	Q_OBJECT

public:
	WordLabel(const QString& text, int s, QWidget* parent);
	void setSize       (int s);                          // font size
	void setHighLighted(bool highLighted);
	void setSelected   (bool selected);
	int  getSize()       const { return size;        }
	bool isHighLighted() const { return highLighted; }
	bool isSelected   () const { return selected;    }

protected:
	virtual void paintEvent(QPaintEvent* event);

signals:
	void sizeChanged();

private:
	bool selected;
	bool highLighted;
	int  size;          // font size
};

#endif // WORDCLOUDWIDGET_H
