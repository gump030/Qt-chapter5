#include "ShowWidget.h"
#include <QHBoxLayout>

ShowWidget::ShowWidget(QWidget *parent)
{
	imageLabel = new QLabel;				//��ʾͼƬ�Ŀؼ�
	imageLabel->setScaledContents(true);	//��С���Ŵ��ڿ����Զ��仯

	text = new QTextEdit;					//�ı������

	QHBoxLayout *mainLayout = new QHBoxLayout(this);	//ˮƽ����
	mainLayout->addWidget(imageLabel);
	mainLayout->addWidget(text);
}