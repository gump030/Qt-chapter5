#include "ShowWidget.h"
#include <QHBoxLayout>

ShowWidget::ShowWidget(QWidget *parent)
{
	imageLabel = new QLabel;				//显示图片的控件
	imageLabel->setScaledContents(true);	//大小随着窗口可以自动变化

	text = new QTextEdit;					//文本输入框

	QHBoxLayout *mainLayout = new QHBoxLayout(this);	//水平布局
	mainLayout->addWidget(imageLabel);
	mainLayout->addWidget(text);
}