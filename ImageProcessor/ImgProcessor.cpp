#include "ImgProcessor.h"  

#include <QApplication>  
#include <QDebug>  
#include <QFileDialog>  
#include <QFile>  
#include <QTextStream>  
#include <QtPrintSupport/QPrintDialog>  
#include <QtPrintSupport/QPrinter>  
#include <QPainter>  
#include <QColorDialog>  
#include <QColor>  
#include <QTextList>  

ImageProcessor::ImageProcessor(QWidget *parent)
	: QMainWindow(parent)
{
	//�����趨����  
	setWindowTitle(tr("my word"));
	
	//������ͼ���label���ı�����ĵط�,���������Ϊ���Ĳ���  
	showWidget = new ShowWidget(this);    //����һ������  
	setCentralWidget(showWidget);   //���������Ϊ���Ĳ���

	//����������ĳ�ʼ��  
	listLabel = new QLabel(tr("����:"));   //����  
	listComboBox = new QComboBox;
	listComboBox->addItem("Standard");      //��׼  
	listComboBox->addItem("QTextListFormat::ListDisc");
	listComboBox->addItem("QTextListFormat::ListCircle");
	listComboBox->addItem("QTextListFormat::ListSquare");
	listComboBox->addItem("QTextListFormat::ListDecimal");
	listComboBox->addItem("QTextListFormat::ListLowerAlpha");
	listComboBox->addItem("QTextListFormat::ListUpperAlpha");
	listComboBox->addItem("QTextListFormat::ListLowerRoman");
	listComboBox->addItem("QTextListFormat::ListUpperRoman");

	//�¼��Ĺ���  
	connect(listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));   //ѡ�����������ĵڼ�����Ȼ������Ӧ����ʾ  

	//  qDebug()<<"11111111111111";  
	//�����й�����Ĳ���  
	//�ڹ�������Ƕ��ؼ�  
	//��������  
	fontLabel1 = new QLabel(tr("���壺"));   //�����ǩ  
	fontComboBox = new QFontComboBox;   //����ѡ������  
	fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);   //��ʾ���еĿ����ŵ�����  

	//�ֺ�  
	fontLabel2 = new QLabel(tr("�ֺţ�"));   //�ֺ�  
	sizeComboBox = new QComboBox;
	QFontDatabase db;   //ʵ�����ֺ������б���������ֲ�ͬ���ֺ���Ŀ����������ڱ�ʾ��ǰϵͳ�����п��õĸ�ʽ��Ϣ  
	foreach(int size, db.standardSizes())   //�ڶ����������ؿ��ñ�׼�ֺŵ��б�  
	{
		sizeComboBox->addItem(QString::number(size)); //QString::number(int size, int base)�ڶ���������ʾ�������base���Ƶ���  
	}

	//����  
	boldBtn = new QToolButton;
	boldBtn->setIcon(QIcon("./image/bold.png"));
	boldBtn->setCheckable(true);    //��ʾ����ѡ��  

	//б��  
	italicBtn = new QToolButton;
	italicBtn->setIcon(QIcon("./image/talic.png"));
	italicBtn->setCheckable(true);

	//�»���  
	underlineBtn = new QToolButton;
	underlineBtn->setIcon(QIcon("./image/underline.png"));
	underlineBtn->setCheckable(true);

	//��ɫ  
	colorBtn = new QToolButton;
	colorBtn->setIcon(QIcon(":/color.png"));
	colorBtn->setCheckable(true);


	createActions();      //��������  
	//  qDebug()<<"222222222222222222";  
	createMenus();    //�����˵�    
	//  qDebug()<<"3333333333333333";  
	createToolBars();   //����  

	//  qDebug()<<"......";  
	//��Ӧ�Ĳۺ���������  
	connect(fontComboBox, SIGNAL(activated(QString)), this, SLOT(ShowFontComboBox(QString)));
	connect(sizeComboBox, SIGNAL(activated(QString)), this, SLOT(ShowSizeSpinBox(QString)));
	connect(boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
	connect(italicBtn, SIGNAL(clicked()), this, SLOT(ShowItalicBtn()));
	connect(underlineBtn, SIGNAL(clicked()), this, SLOT(ShowUnderlineBtn()));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));
	connect(showWidget->text, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(ShowCurrentFormatChanged(QTextCharFormat&)));

	connect(showWidget->text->document(), SIGNAL(undoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(showWidget->text->document(), SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(showWidget->text, SIGNAL(cursorPositionChanged()), this, SLOT(ShowCursorPositionChanged()));

	//���ͼ����سɹ�  
	if (img.load("./image/227.png"))
	{
		//��imageLabel�з�ͼ��  
		showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
	}

}

//ʵ���ı�������  
void ImageProcessor::ShowList(int index)
{
	QTextCursor cursor = showWidget->text->textCursor();
	if (index != 0)
	{
		QTextListFormat::Style style = QTextListFormat::ListDisc;   //Ĭ�����������  

		switch (index)
		{
		default:
		case 1:
			style = QTextListFormat::ListDisc; break;
		case 2:
			style = QTextListFormat::ListCircle; break;
		case 3:
			style = QTextListFormat::ListSquare; break;
		case 4:
			style = QTextListFormat::ListDecimal; break;
		case 5:
			style = QTextListFormat::ListLowerAlpha; break;
		case 6:
			style = QTextListFormat::ListUpperAlpha; break;
		case 7:
			style = QTextListFormat::ListLowerRoman; break;
		case 8:
			style = QTextListFormat::ListUpperRoman; break;
		}
		
		cursor.beginEditBlock();    //��ʼ��������ֵ  
									//�����ʾ��ʽ������Ϣ  
		QTextBlockFormat blockFmt = cursor.blockFormat();   //����������ع��ѡ���Ķ����ʽ  
		QTextListFormat listFmt;

		if (cursor.currentList())    //�жϵ�ǰ�Ƿ���ѡ�и�ʽ  
		{
			listFmt = cursor.currentList()->format();   //�趨��ʽ  
		}
		else
		{
			listFmt.setIndent(blockFmt.indent() + 1);   //�趨����ֵ  
			blockFmt.setIndent(0);    //�趨��������Ϊ0  
			cursor.setBlockFormat(blockFmt);      //�趨ʵ��  
		}
		listFmt.setStyle(style);    //�趨��ʽ  
		cursor.endEditBlock();    //��������  
	}
	else
	{
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}

}

//ʵ�ֶ���Ķ���  
void ImageProcessor::ShowAlignment(QAction *act)
{
	if (act == leftAction)
		showWidget->text->setAlignment(Qt::AlignLeft);    //���ܵ���Ӧ���ź�Ӧ�õ��ı���  
	if (act == rightAction)
		showWidget->text->setAlignment(Qt::AlignRight);
	if (act == centerAction)
		showWidget->text->setAlignment(Qt::AlignCenter);
	if (act == justifyAction)
		showWidget->text->setAlignment(Qt::AlignJustify);
}

//�Ѷ�Ӧ��״̬��ʾ�ڹ�������  
void ImageProcessor::ShowCursorPositionChanged()
{
	if (showWidget->text->alignment() == Qt::AlignLeft)
		leftAction->setChecked(true);   //�ı�Ϊѡ��״̬  
	if (showWidget->text->alignment() == Qt::AlignRight)
		rightAction->setChecked(true);
	if (showWidget->text->alignment() == Qt::AlignCenter)
		centerAction->setChecked(true);
	if (showWidget->text->alignment() == Qt::AlignJustify)
		justifyAction->setChecked(true);
}

//��������ʵ��  
void ImageProcessor::createToolBars()
{
	//���򹤾���  
	listToolBar = addToolBar("list");
	listToolBar->addWidget(listLabel);
	listToolBar->addWidget(listComboBox);
	listToolBar->addSeparator();
	listToolBar->addActions(actGrp->actions());   //�������鶼��ӽ�ȥ  

	//�ļ�������  
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(PrintTextAction);
	fileTool->addAction(PrintImageAction);

	//�༭������  
	zoomTool = addToolBar("Edit");
	zoomTool->addAction(copyAction);
	zoomTool->addAction(cutAction);
	zoomTool->addAction(pasteAction); 
	zoomTool->addSeparator(); //�ָ���  
	zoomTool->addAction(zoomInAction);
	zoomTool->addAction(zoomOutAction);

	this->addToolBarBreak(Qt::TopToolBarArea); //����������
	//��ת����  
	rotateTool = addToolBar("rotate");
	rotateTool->addAction(rotate90Action);
	rotateTool->addAction(rotate180Action);
	rotateTool->addAction(rotate270Action);

	//����������  
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);

	//���ֵĹ�����
	
	fontToolBar = addToolBar("Font");
	fontToolBar->addWidget(fontLabel1);   //����  
	fontToolBar->addWidget(fontComboBox);     //����������  
	fontToolBar->addWidget(fontLabel2);   //�ֺ�  
	fontToolBar->addWidget(sizeComboBox);   //�ֺ�����  
	fontToolBar->addSeparator();    //�ָ���  
	fontToolBar->addWidget(boldBtn);    //�Ӵ�  
	fontToolBar->addWidget(italicBtn);    //б  
	fontToolBar->addWidget(underlineBtn);   //�»���  
	fontToolBar->addSeparator();
	fontToolBar->addWidget(colorBtn);   //��ɫ  

	fileTool->setAllowedAreas(Qt::TopToolBarArea | Qt::LeftToolBarArea);
}

//�����ַ���ʽ  
//��������ڵ��ַ���ʽ�ı��ʱ�򣬵������������������������µĸ�ʽ���������ϵĿؼ���ʾ����  
void ImageProcessor::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
	fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
	sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
	boldBtn->setChecked(fmt.font().bold());
	italicBtn->setChecked(fmt.fontItalic());
	underlineBtn->setChecked(fmt.fontUnderline());
}

//������ɫ  
void ImageProcessor::ShowColorBtn()
{
	QColor color = QColorDialog::getColor(Qt::red, this);   //��һ������ָ��Ĭ��ѡ�����ɫ���ڶ��������˱�׼�Ի���ĸ�����  

	if (color.isValid())
	{
		QTextCharFormat fmt;
		fmt.setForeground(color);   //������ɫ  
		showWidget->text->mergeCurrentCharFormat(fmt);
	}
}

//�»���  
void ImageProcessor::ShowUnderlineBtn()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underlineBtn->isChecked());
	showWidget->text->mergeCurrentCharFormat(fmt);
}

//����������б  
void ImageProcessor::ShowItalicBtn()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italicBtn->isChecked());
	showWidget->text->mergeCurrentCharFormat(fmt);
}

//�������ּӴ�  
void ImageProcessor::ShowBoldBtn()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
	showWidget->text->mergeCurrentCharFormat(fmt);    //Ӧ�ý�ȥ  
}

//�����ֺ�  
void ImageProcessor::ShowSizeSpinBox(QString spinValue)
{
	QTextCharFormat fmt; //�����ַ���ʽ  
	fmt.setFontPointSize(spinValue.toFloat());    //���������С  
	showWidget->text->mergeCurrentCharFormat(fmt);//���µĸ�ʽӦ�õ����ѡ���ڵ��ַ�  
}

//��������  
void ImageProcessor::ShowFontComboBox(QString comboStr)
{
	QTextCharFormat fmt;    //�����ַ���ʽ  
	fmt.setFontFamily(comboStr);    //ѡ��������������ø�QTextCharFormat  
	mergeFormat(fmt);   //���µĸ�ʽӦ�õ����ѡ���ڵ��ַ�  
}

//ͨ��QTextCursor����QTextDocument�����޸�  
void ImageProcessor::mergeFormat(QTextCharFormat format)
{
	QTextCursor cursor = showWidget->text->textCursor();    //�õ������  
	if (!cursor.hasSelection())    //�����û�и���ѡ������ѹ�����ڵĴ���Ϊѡ�����Կո�򡰣����򡰡����ȱ����ŷָ�  
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);   //�Ѹ�ʽӦ�õ�������ڵ��ַ���  
	showWidget->text->mergeCurrentCharFormat(format);   //���������������ʽӦ�õ�ѡ���ڵ������ַ���  

}

//�˵���ʵ��  
void ImageProcessor::createMenus()
{
	//�ļ��˵�  
	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(PrintTextAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator(); //�ָ���  
	fileMenu->addAction(exitAction);

	//���Ų˵�  
	zoomMenu = menuBar()->addMenu(tr("zoom"));  //����  
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addAction(aboutAction);
	zoomMenu->addSeparator(); //�ָ���  
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);

	//��ת�˵�  
	rotateMenu = menuBar()->addMenu(tr("rotate"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);

	//����˵�  
	mirrorMenu = menuBar()->addMenu(tr("mirror"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);

}

//������ʵ��  
void ImageProcessor::createActions()
{
	//�Ű�ĸ��ֲ����Ĵ����¼�  
	actGrp = new QActionGroup(this);      //����һ��action��  
	leftAction = new QAction(QIcon("./image/left.png"), "left", actGrp);    //������ͼ��  
	leftAction->setCheckable(true);   //�趨��ť�Ƿ����ѡ��  

	rightAction = new QAction(QIcon("./image/right.png"), "right", actGrp);   //�Ҷ���  
	rightAction->setCheckable(true);

	centerAction = new QAction(QIcon("./image/center.png"), "center", actGrp);   //���ж���  
	centerAction->setCheckable(true);

	justifyAction = new QAction(QIcon("./image/justify.png"), "justify", actGrp);   //���˶���  
	justifyAction->setCheckable(true);

	connect(actGrp, SIGNAL(triggered(QAction*)), this, SLOT(ShowAlignment(QAction*)));    //����ѡ���actionȻ����ʾ��Ӧ�Ľ��  


	//�򿪶���  
	//qDebug()<<"11111111111111";  
	openFileAction = new QAction(QIcon("./image/open.png"), tr("��"), this);    //����ͼƬ������  
	openFileAction->setShortcut(tr("Ctrl+O"));    //���ÿ�ݼ�  
	openFileAction->setStatusTip(tr("��һ���ļ�"));    //������ʾ  
	//qDebug()<<"222222222222222222";  
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));

	//�½�����  
	NewFileAction = new QAction(QIcon("./image/new.png"), tr("�½�"), this);    //����ͼƬ������  
	NewFileAction->setShortcut(tr("Ctrl+N"));    //���ÿ�ݼ�  
	NewFileAction->setStatusTip(tr("�½�һ���ļ�"));    //������ʾ  
	connect(NewFileAction, SIGNAL(triggered()), this, SLOT(ShowNewFile()));   //��������Ӧ�Ĳ�  

	//�˳�����  
	exitAction = new QAction(QIcon("./image/new.png"), tr("�˳�"), this);    //����ͼƬ������  
	exitAction->setShortcut(tr("Ctrl+Q"));    //���ÿ�ݼ�  
	exitAction->setStatusTip(tr("�˳�����"));    //������ʾ  
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//����  
	copyAction = new QAction(QIcon("./image/copy.png"), tr("����"), this);    //����ͼƬ������  
	copyAction->setShortcut(tr("Ctrl+C"));    //���ÿ�ݼ�  
	copyAction->setStatusTip(tr("�����ļ�"));    //������ʾ  
	connect(copyAction, SIGNAL(triggered()), showWidget->text, SLOT(copy()));

	//����  
	cutAction = new QAction(QIcon("./image/cut.png"), tr("����"), this);    //����ͼƬ������  
	cutAction->setShortcut(tr("Ctrl+X"));    //���ÿ�ݼ�  
	cutAction->setStatusTip(tr("�����ļ�"));    //������ʾ  
	connect(cutAction, SIGNAL(triggered()), showWidget->text, SLOT(cut()));

	//ճ��  
	pasteAction = new QAction(QIcon("./image/paste.png"), tr("ճ��"), this);    //����ͼƬ������  
	pasteAction->setShortcut(tr("Ctrl+V"));    //���ÿ�ݼ�  
	pasteAction->setStatusTip(tr("ճ���ļ�"));    //������ʾ  
	connect(pasteAction, SIGNAL(triggered()), showWidget->text, SLOT(paste()));

	//����  
	//QApplication *app;  
	aboutAction = new QAction(tr("����"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(QApplication::aboutQt()));

	//��ӡ�ı�  
	PrintTextAction = new QAction(QIcon("./image/printText.png"), tr("��ӡ�ı�"), this);
	PrintTextAction->setStatusTip(tr("��ӡһ���ı�"));
	connect(PrintTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));   //������Ӧ�Ķ���  

	//��ӡͼ��  
	PrintImageAction = new QAction(QIcon("./image/printImage.png"), tr("��ӡͼƬ"), this);
	PrintImageAction->setStatusTip(tr("��ӡһ��ͼƬ"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));

	//�Ŵ�ͼ��  
	zoomInAction = new QAction(QIcon("./image/zoomin.png"), tr("�Ŵ�"), this);
	zoomInAction->setStatusTip(tr("�Ŵ�ͼƬ"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(ShowZoomIn())); //����ͼƬ�Ŵ�  

	//��С  
	zoomOutAction = new QAction(QIcon("./image/zoomout.png"), tr("��С"), this);
	zoomOutAction->setStatusTip(tr("��СͼƬ"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut())); //����ͼƬ��С  

	//ʵ��ͼƬ��ѡ����  
	//��ת90��  
	rotate90Action = new QAction(QIcon("./image/Rotate90.png"), tr("��ת90"), this);
	rotate90Action->setStatusTip(tr("get rotate90 image"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));
	//180��  
	rotate180Action = new QAction(QIcon("./image/Rotate180.png"), tr("��ת180"), this);
	rotate180Action->setStatusTip(tr("get rotate180 image"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));
	//270��  
	rotate270Action = new QAction(QIcon("./image/Rotate270.png"), tr("��ת270"), this);
	rotate270Action->setStatusTip(tr("get rotate270 image"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));


	//ʵ��ͼ����Ķ���  
	//����  
	mirrorVerticalAction = new QAction(QIcon("./image/mirrorVertical.png"), tr("������"), this);
	mirrorVerticalAction->setStatusTip(tr("mirrorVertical the image"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));
	//����  
	mirrorHorizontalAction = new QAction(QIcon("./image/mirrorHorizontal.png"), tr("������"), this);
	mirrorHorizontalAction->setStatusTip(tr("mirrorHorizontal the image"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));

	//ʵ�ֳ����ͻָ��Ķ���  
	//�����ͻָ�  
	undoAction = new QAction(QIcon("./image/undo.png"),tr("undo"), this);    //����  
	connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(undo()));

	redoAction = new QAction(QIcon("./image/redo.png"),tr("redo"), this);    //����  
	connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(redo()));

}


//ˮƽ����  
void ImageProcessor::ShowMirrorHorizontal()
{
	if (img.isNull())
		return;
	img = img.mirrored(true, false);    //ˮƽ����  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//��ֱ����  
void ImageProcessor::ShowMirrorVertical()
{
	if (img.isNull())
		return;
	img = img.mirrored(false, true);    //��ֱ����  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//��ת270��  
void ImageProcessor::ShowRotate270()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(270);    //��ת90��  
	img = img.transformed(matrix);    //ͼ����ת֮�������µõ���ֵ  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//��ת180��  
void ImageProcessor::ShowRotate180()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(180);    //��ת90��  
	img = img.transformed(matrix);    //ͼ����ת֮�������µõ���ֵ  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//��ת90��  
void ImageProcessor::ShowRotate90()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(90);    //��ת90��  
	img = img.transformed(matrix);    //ͼ����ת֮�������µõ���ֵ  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//ʵ��ͼƬ�ķ�С  
void ImageProcessor::ShowZoomOut()
{
	//�����ж��Ƿ�����Ƭ  
	if (img.isNull())
		return;   //û����Ƭ���ǾͲ��÷Ŵ���  
	QMatrix martix;   //һ��ͼƬ�Ĵ�Ŷ���  
	martix.scale(0.5, 0.5);   //���Ϳ���С2��  
	img = img.transformed(martix);    //��ͼƬ�Ĳ�����ʵ  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));   //�ı���ͼƬ�Ż�ȥ  

}

//ʵ��ͼƬ�ķŴ�  
void ImageProcessor::ShowZoomIn()
{
	//�����ж��Ƿ�����Ƭ  
	if (img.isNull())
		return;   //û����Ƭ���ǾͲ��÷Ŵ���  
	QMatrix martix;   //һ��ͼƬ�Ĵ�Ŷ���  
	martix.scale(2, 2);   //���Ϳ��Ŵ�2��  
	img = img.transformed(martix);    //��ͼƬ�Ĳ�����ʵ  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));   //�ı���ͼƬ�Ż�ȥ  

}

//ʵ�ִ�ӡͼ���ܵĺ���  
void ImageProcessor::ShowPrintImage()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer, this);
	//�Ƿ���ȷ��  
	if (printDialog.exec())
	{
		QPainter painter(&printer);
		QRect rect = painter.viewport();    //���QPainter������ӽǾ�������  
		QSize size = img.size();    //ͼƬ�ĳߴ�  
		size.scale(rect.size(), Qt::KeepAspectRatio);

		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());   //���þ�������  
		painter.drawImage(0, 0, img);
	}

}

//ʵ�ִ�ӡ�ı�����  
void ImageProcessor::ShowPrintText()
{
	//  QP  
	QPrinter printer;   //��ӡ  
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())    //�Ƿ���ȷ��  
	{
		QTextDocument *doc = showWidget->text->document();    //�õ��ĵ�����  
		doc->print(&printer);   //��ʼ��ӡ  
	}

}

//���ļ�  
void ImageProcessor::ShowOpenFile()
{
	fileName = QFileDialog::getOpenFileName(this);    //�õ��ļ���  
	if (!fileName.isEmpty())   //�ļ���Ϊ��  
	{
		if (showWidget->text->document()->isEmpty()) //���Ĳ��������ݵĸ��ڵ㲻�ǿ�  
		{
			loadFile(fileName);   //����ļ�  
		}
		else
		{
			ImageProcessor *newImageProcessor = new ImageProcessor;   //����һ���µ�my word�ĵ�  
			newImageProcessor->show();    //��ʾ����  
			newImageProcessor->loadFile(fileName);    //����ļ�  
		}
	}
}

//ʵ��loadFile����,����ļ��Ĵ��������ٴν���ѧϰ  
void ImageProcessor::loadFile(QString filename)
{
	printf("file name:%s\n", (char *)filename.data());    //����ļ���  

	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream textStream(&file);
		while (!textStream.atEnd())    //ѭ��������  
		{
			showWidget->text->append(textStream.readLine());    //���ĵ�����ļ�  
			printf("read line\n");
		}
		printf("end\n");
	}

}

//�½��ļ���  
void ImageProcessor::ShowNewFile()
{
	//  qDebug()<<"????";  
	ImageProcessor *newImageProcessor = new ImageProcessor;
	newImageProcessor->show();    //һ���µ��ĵ���ʾ  
}
