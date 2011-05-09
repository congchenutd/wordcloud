#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QtGui/QWidget>
#include <QLabel>
#include <QMap>
#include "ui_WordCloudWidget.h"

class FlowLayout;
class WordLabel;

typedef QMap<QString, WordLabel*> WordList;

class WordCloudWidget : public QWidget
{
	Q_OBJECT

public:
	WordCloudWidget(QWidget* parent = 0, Qt::WFlags flags = 0);
	void addWord(WordLabel* word);
	void addWord(const QString& text, int size);
	void highLight(const QStringList& words);

private slots:
	void onDel(const QString& word);

private:
	Ui::WordCloudWidgetClass ui;
	FlowLayout* layout;
	WordList wordList;
};

class WordLabel : public QLabel
{
	Q_OBJECT

public:
	WordLabel(const QString& text, int size, QWidget* parent);
	void setHighLighted(bool highLighted);
	void setSelected   (bool selected);
	bool isHighLighted() const { return highLighted; }
	bool isSelected   () const { return selected;    }

protected:
	virtual void mousePressEvent (QMouseEvent* event);
	virtual void paintEvent      (QPaintEvent* event);
	virtual void contextMenuEvent(QContextMenuEvent* event);

private slots:
	void onAdd();
	void onDel();

signals:
	void addRequested(QString);
	void delRequested(QString);

private:
	bool selected;
	bool highLighted;
};

#endif // WORDCLOUDWIDGET_H
