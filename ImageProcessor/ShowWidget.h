#pragma once
#pragma execution_character_set("utf-8")
#include <qwidget.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qimage.h>

class ShowWidget :public QWidget
{
	Q_OBJECT
public:
	explicit ShowWidget(QWidget *parent = 0);
	QImage img;
	QLabel *imageLabel;
	QTextEdit *text;
signals:

public slots:
};