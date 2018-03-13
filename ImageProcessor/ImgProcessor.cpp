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
	//首先设定标题  
	setWindowTitle(tr("my word"));
	
	//创建放图像的label和文本输入的地方,把这个设置为中心部件  
	showWidget = new ShowWidget(this);    //创建一个对象  
	setCentralWidget(showWidget);   //把这个设置为中心部件

	//排序下拉框的初始化  
	listLabel = new QLabel(tr("排序:"));   //排序  
	listComboBox = new QComboBox;
	listComboBox->addItem("Standard");      //标准  
	listComboBox->addItem("QTextListFormat::ListDisc");
	listComboBox->addItem("QTextListFormat::ListCircle");
	listComboBox->addItem("QTextListFormat::ListSquare");
	listComboBox->addItem("QTextListFormat::ListDecimal");
	listComboBox->addItem("QTextListFormat::ListLowerAlpha");
	listComboBox->addItem("QTextListFormat::ListUpperAlpha");
	listComboBox->addItem("QTextListFormat::ListLowerRoman");
	listComboBox->addItem("QTextListFormat::ListUpperRoman");

	//事件的关联  
	connect(listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));   //选择的下拉排序的第几个，然后在相应的显示  

	//  qDebug()<<"11111111111111";  
	//创建有关字体的部件  
	//在工具栏上嵌入控件  
	//设置字体  
	fontLabel1 = new QLabel(tr("字体："));   //字体标签  
	fontComboBox = new QFontComboBox;   //字体选项下拉  
	fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);   //显示所有的可缩放的字体  

	//字号  
	fontLabel2 = new QLabel(tr("字号："));   //字号  
	sizeComboBox = new QComboBox;
	QFontDatabase db;   //实现在字号下拉列表框中填充各种不同的字号条目，这个类用于表示当前系统中所有可用的格式信息  
	foreach(int size, db.standardSizes())   //第二个参数返回可用标准字号的列表  
	{
		sizeComboBox->addItem(QString::number(size)); //QString::number(int size, int base)第二个参数表示这个返回base进制的数  
	}

	//粗体  
	boldBtn = new QToolButton;
	boldBtn->setIcon(QIcon("./image/bold.png"));
	boldBtn->setCheckable(true);    //表示可以选中  

	//斜体  
	italicBtn = new QToolButton;
	italicBtn->setIcon(QIcon("./image/talic.png"));
	italicBtn->setCheckable(true);

	//下划线  
	underlineBtn = new QToolButton;
	underlineBtn->setIcon(QIcon("./image/underline.png"));
	underlineBtn->setCheckable(true);

	//颜色  
	colorBtn = new QToolButton;
	colorBtn->setIcon(QIcon(":/color.png"));
	colorBtn->setCheckable(true);


	createActions();      //创建动作  
	//  qDebug()<<"222222222222222222";  
	createMenus();    //创建菜单    
	//  qDebug()<<"3333333333333333";  
	createToolBars();   //工具  

	//  qDebug()<<"......";  
	//相应的槽函数的连接  
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

	//如果图像加载成功  
	if (img.load("./image/227.png"))
	{
		//在imageLabel中放图像  
		showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
	}

}

//实现文本的排序  
void ImageProcessor::ShowList(int index)
{
	QTextCursor cursor = showWidget->text->textCursor();
	if (index != 0)
	{
		QTextListFormat::Style style = QTextListFormat::ListDisc;   //默认排序是这个  

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
		
		cursor.beginEditBlock();    //开始设置缩进值  
									//这个显示格式化的信息  
		QTextBlockFormat blockFmt = cursor.blockFormat();   //后面这个返回光标选定的段落格式  
		QTextListFormat listFmt;

		if (cursor.currentList())    //判断当前是否有选中格式  
		{
			listFmt = cursor.currentList()->format();   //设定格式  
		}
		else
		{
			listFmt.setIndent(blockFmt.indent() + 1);   //设定缩进值  
			blockFmt.setIndent(0);    //设定段落缩进为0  
			cursor.setBlockFormat(blockFmt);      //设定实现  
		}
		listFmt.setStyle(style);    //设定格式  
		cursor.endEditBlock();    //结束设置  
	}
	else
	{
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}

}

//实现段落的对齐  
void ImageProcessor::ShowAlignment(QAction *act)
{
	if (act == leftAction)
		showWidget->text->setAlignment(Qt::AlignLeft);    //接受到相应的信号应用到文本上  
	if (act == rightAction)
		showWidget->text->setAlignment(Qt::AlignRight);
	if (act == centerAction)
		showWidget->text->setAlignment(Qt::AlignCenter);
	if (act == justifyAction)
		showWidget->text->setAlignment(Qt::AlignJustify);
}

//把对应的状态显示在工具栏上  
void ImageProcessor::ShowCursorPositionChanged()
{
	if (showWidget->text->alignment() == Qt::AlignLeft)
		leftAction->setChecked(true);   //改变为选中状态  
	if (showWidget->text->alignment() == Qt::AlignRight)
		rightAction->setChecked(true);
	if (showWidget->text->alignment() == Qt::AlignCenter)
		centerAction->setChecked(true);
	if (showWidget->text->alignment() == Qt::AlignJustify)
		justifyAction->setChecked(true);
}

//工具栏的实现  
void ImageProcessor::createToolBars()
{
	//排序工具条  
	listToolBar = addToolBar("list");
	listToolBar->addWidget(listLabel);
	listToolBar->addWidget(listComboBox);
	listToolBar->addSeparator();
	listToolBar->addActions(actGrp->actions());   //把整个组都添加进去  

	//文件工具条  
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(PrintTextAction);
	fileTool->addAction(PrintImageAction);

	//编辑工具条  
	zoomTool = addToolBar("Edit");
	zoomTool->addAction(copyAction);
	zoomTool->addAction(cutAction);
	zoomTool->addAction(pasteAction); 
	zoomTool->addSeparator(); //分隔符  
	zoomTool->addAction(zoomInAction);
	zoomTool->addAction(zoomOutAction);

	this->addToolBarBreak(Qt::TopToolBarArea); //工具栏换行
	//旋转工具  
	rotateTool = addToolBar("rotate");
	rotateTool->addAction(rotate90Action);
	rotateTool->addAction(rotate180Action);
	rotateTool->addAction(rotate270Action);

	//撤销和重做  
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);

	//文字的工具栏
	
	fontToolBar = addToolBar("Font");
	fontToolBar->addWidget(fontLabel1);   //字体  
	fontToolBar->addWidget(fontComboBox);     //字体下拉框  
	fontToolBar->addWidget(fontLabel2);   //字号  
	fontToolBar->addWidget(sizeComboBox);   //字号下拉  
	fontToolBar->addSeparator();    //分割线  
	fontToolBar->addWidget(boldBtn);    //加粗  
	fontToolBar->addWidget(italicBtn);    //斜  
	fontToolBar->addWidget(underlineBtn);   //下划线  
	fontToolBar->addSeparator();
	fontToolBar->addWidget(colorBtn);   //颜色  

	fileTool->setAllowedAreas(Qt::TopToolBarArea | Qt::LeftToolBarArea);
}

//设置字符格式  
//当光标所在的字符格式改变的时候，调用这个函数，这个函数根据新的格式将工具栏上的控件显示更新  
void ImageProcessor::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
	fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
	sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
	boldBtn->setChecked(fmt.font().bold());
	italicBtn->setChecked(fmt.fontItalic());
	underlineBtn->setChecked(fmt.fontUnderline());
}

//设置颜色  
void ImageProcessor::ShowColorBtn()
{
	QColor color = QColorDialog::getColor(Qt::red, this);   //第一个参数指定默认选择的颜色，第二个定义了标准对话框的父窗口  

	if (color.isValid())
	{
		QTextCharFormat fmt;
		fmt.setForeground(color);   //设置颜色  
		showWidget->text->mergeCurrentCharFormat(fmt);
	}
}

//下划线  
void ImageProcessor::ShowUnderlineBtn()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underlineBtn->isChecked());
	showWidget->text->mergeCurrentCharFormat(fmt);
}

//设置字体倾斜  
void ImageProcessor::ShowItalicBtn()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italicBtn->isChecked());
	showWidget->text->mergeCurrentCharFormat(fmt);
}

//设置文字加粗  
void ImageProcessor::ShowBoldBtn()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
	showWidget->text->mergeCurrentCharFormat(fmt);    //应用进去  
}

//设置字号  
void ImageProcessor::ShowSizeSpinBox(QString spinValue)
{
	QTextCharFormat fmt; //文体字符格式  
	fmt.setFontPointSize(spinValue.toFloat());    //设置字体大小  
	showWidget->text->mergeCurrentCharFormat(fmt);//将新的格式应用到光标选区内的字符  
}

//设置字体  
void ImageProcessor::ShowFontComboBox(QString comboStr)
{
	QTextCharFormat fmt;    //文体字符格式  
	fmt.setFontFamily(comboStr);    //选择的字体名称设置给QTextCharFormat  
	mergeFormat(fmt);   //将新的格式应用到光标选区内的字符  
}

//通过QTextCursor来对QTextDocument进行修改  
void ImageProcessor::mergeFormat(QTextCharFormat format)
{
	QTextCursor cursor = showWidget->text->textCursor();    //得到这个类  
	if (!cursor.hasSelection())    //若光标没有高亮选区，则把光标所在的词作为选区，以空格或“，”或“。”等标点符号分割  
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);   //把格式应用到光标所在的字符上  
	showWidget->text->mergeCurrentCharFormat(format);   //调用这个函数将格式应用到选区内的所有字符上  

}

//菜单的实现  
void ImageProcessor::createMenus()
{
	//文件菜单  
	fileMenu = menuBar()->addMenu(tr("file"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(PrintTextAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator(); //分隔符  
	fileMenu->addAction(exitAction);

	//缩放菜单  
	zoomMenu = menuBar()->addMenu(tr("zoom"));  //缩放  
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addAction(aboutAction);
	zoomMenu->addSeparator(); //分隔符  
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);

	//旋转菜单  
	rotateMenu = menuBar()->addMenu(tr("rotate"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);

	//镜像菜单  
	mirrorMenu = menuBar()->addMenu(tr("mirror"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);

}

//动作的实现  
void ImageProcessor::createActions()
{
	//排版的各种操作的触发事件  
	actGrp = new QActionGroup(this);      //创建一个action组  
	leftAction = new QAction(QIcon("./image/left.png"), "left", actGrp);    //左对齐的图标  
	leftAction->setCheckable(true);   //设定按钮是否可以选定  

	rightAction = new QAction(QIcon("./image/right.png"), "right", actGrp);   //右对齐  
	rightAction->setCheckable(true);

	centerAction = new QAction(QIcon("./image/center.png"), "center", actGrp);   //居中对齐  
	centerAction->setCheckable(true);

	justifyAction = new QAction(QIcon("./image/justify.png"), "justify", actGrp);   //两端对齐  
	justifyAction->setCheckable(true);

	connect(actGrp, SIGNAL(triggered(QAction*)), this, SLOT(ShowAlignment(QAction*)));    //接受选择的action然后显示相应的结果  


	//打开动作  
	//qDebug()<<"11111111111111";  
	openFileAction = new QAction(QIcon("./image/open.png"), tr("打开"), this);    //设置图片，名字  
	openFileAction->setShortcut(tr("Ctrl+O"));    //设置快捷键  
	openFileAction->setStatusTip(tr("打开一个文件"));    //设置提示  
	//qDebug()<<"222222222222222222";  
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));

	//新建动作  
	NewFileAction = new QAction(QIcon("./image/new.png"), tr("新建"), this);    //设置图片，名字  
	NewFileAction->setShortcut(tr("Ctrl+N"));    //设置快捷键  
	NewFileAction->setStatusTip(tr("新建一个文件"));    //设置提示  
	connect(NewFileAction, SIGNAL(triggered()), this, SLOT(ShowNewFile()));   //连接上相应的槽  

	//退出动作  
	exitAction = new QAction(QIcon("./image/new.png"), tr("退出"), this);    //设置图片，名字  
	exitAction->setShortcut(tr("Ctrl+Q"));    //设置快捷键  
	exitAction->setStatusTip(tr("退出程序"));    //设置提示  
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//复制  
	copyAction = new QAction(QIcon("./image/copy.png"), tr("复制"), this);    //设置图片，名字  
	copyAction->setShortcut(tr("Ctrl+C"));    //设置快捷键  
	copyAction->setStatusTip(tr("复制文件"));    //设置提示  
	connect(copyAction, SIGNAL(triggered()), showWidget->text, SLOT(copy()));

	//剪切  
	cutAction = new QAction(QIcon("./image/cut.png"), tr("剪切"), this);    //设置图片，名字  
	cutAction->setShortcut(tr("Ctrl+X"));    //设置快捷键  
	cutAction->setStatusTip(tr("剪切文件"));    //设置提示  
	connect(cutAction, SIGNAL(triggered()), showWidget->text, SLOT(cut()));

	//粘贴  
	pasteAction = new QAction(QIcon("./image/paste.png"), tr("粘贴"), this);    //设置图片，名字  
	pasteAction->setShortcut(tr("Ctrl+V"));    //设置快捷键  
	pasteAction->setStatusTip(tr("粘贴文件"));    //设置提示  
	connect(pasteAction, SIGNAL(triggered()), showWidget->text, SLOT(paste()));

	//关于  
	//QApplication *app;  
	aboutAction = new QAction(tr("关于"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(QApplication::aboutQt()));

	//打印文本  
	PrintTextAction = new QAction(QIcon("./image/printText.png"), tr("打印文本"), this);
	PrintTextAction->setStatusTip(tr("打印一个文本"));
	connect(PrintTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));   //连接相应的动作  

	//打印图像  
	PrintImageAction = new QAction(QIcon("./image/printImage.png"), tr("打印图片"), this);
	PrintImageAction->setStatusTip(tr("打印一张图片"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));

	//放大图像  
	zoomInAction = new QAction(QIcon("./image/zoomin.png"), tr("放大"), this);
	zoomInAction->setStatusTip(tr("放大图片"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(ShowZoomIn())); //进行图片放大  

	//缩小  
	zoomOutAction = new QAction(QIcon("./image/zoomout.png"), tr("缩小"), this);
	zoomOutAction->setStatusTip(tr("缩小图片"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut())); //进行图片放小  

	//实现图片的选择动作  
	//旋转90°  
	rotate90Action = new QAction(QIcon("./image/Rotate90.png"), tr("旋转90"), this);
	rotate90Action->setStatusTip(tr("get rotate90 image"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));
	//180°  
	rotate180Action = new QAction(QIcon("./image/Rotate180.png"), tr("旋转180"), this);
	rotate180Action->setStatusTip(tr("get rotate180 image"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));
	//270°  
	rotate270Action = new QAction(QIcon("./image/Rotate270.png"), tr("旋转270"), this);
	rotate270Action->setStatusTip(tr("get rotate270 image"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));


	//实现图像镜像的动作  
	//纵向  
	mirrorVerticalAction = new QAction(QIcon("./image/mirrorVertical.png"), tr("纵向镜像"), this);
	mirrorVerticalAction->setStatusTip(tr("mirrorVertical the image"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));
	//横向  
	mirrorHorizontalAction = new QAction(QIcon("./image/mirrorHorizontal.png"), tr("横向镜像"), this);
	mirrorHorizontalAction->setStatusTip(tr("mirrorHorizontal the image"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));

	//实现撤销和恢复的动作  
	//撤销和恢复  
	undoAction = new QAction(QIcon("./image/undo.png"),tr("undo"), this);    //撤销  
	connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(undo()));

	redoAction = new QAction(QIcon("./image/redo.png"),tr("redo"), this);    //重做  
	connect(undoAction, SIGNAL(triggered()), showWidget->text, SLOT(redo()));

}


//水平镜像  
void ImageProcessor::ShowMirrorHorizontal()
{
	if (img.isNull())
		return;
	img = img.mirrored(true, false);    //水平镜像  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//垂直镜像  
void ImageProcessor::ShowMirrorVertical()
{
	if (img.isNull())
		return;
	img = img.mirrored(false, true);    //垂直镜像  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//旋转270度  
void ImageProcessor::ShowRotate270()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(270);    //旋转90度  
	img = img.transformed(matrix);    //图像旋转之后再重新得到赋值  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//旋转180度  
void ImageProcessor::ShowRotate180()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(180);    //旋转90度  
	img = img.transformed(matrix);    //图像旋转之后再重新得到赋值  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//旋转90度  
void ImageProcessor::ShowRotate90()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(90);    //旋转90度  
	img = img.transformed(matrix);    //图像旋转之后再重新得到赋值  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//实现图片的放小  
void ImageProcessor::ShowZoomOut()
{
	//首先判断是否有照片  
	if (img.isNull())
		return;   //没有照片，那就不用放大了  
	QMatrix martix;   //一个图片的存放对象  
	martix.scale(0.5, 0.5);   //长和宽都放小2倍  
	img = img.transformed(martix);    //对图片的操作落实  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));   //改变后的图片放回去  

}

//实现图片的放大  
void ImageProcessor::ShowZoomIn()
{
	//首先判断是否有照片  
	if (img.isNull())
		return;   //没有照片，那就不用放大了  
	QMatrix martix;   //一个图片的存放对象  
	martix.scale(2, 2);   //长和宽都放大2倍  
	img = img.transformed(martix);    //对图片的操作落实  
	showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));   //改变后的图片放回去  

}

//实现打印图像功能的函数  
void ImageProcessor::ShowPrintImage()
{
	QPrinter printer;
	QPrintDialog printDialog(&printer, this);
	//是否点击确定  
	if (printDialog.exec())
	{
		QPainter painter(&printer);
		QRect rect = painter.viewport();    //获得QPainter对象的视角矩形区域  
		QSize size = img.size();    //图片的尺寸  
		size.scale(rect.size(), Qt::KeepAspectRatio);

		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());   //设置矩形区域  
		painter.drawImage(0, 0, img);
	}

}

//实现打印文本功能  
void ImageProcessor::ShowPrintText()
{
	//  QP  
	QPrinter printer;   //打印  
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())    //是否点击确定  
	{
		QTextDocument *doc = showWidget->text->document();    //得到文档对象  
		doc->print(&printer);   //开始打印  
	}

}

//打开文件  
void ImageProcessor::ShowOpenFile()
{
	fileName = QFileDialog::getOpenFileName(this);    //得到文件名  
	if (!fileName.isEmpty())   //文件不为空  
	{
		if (showWidget->text->document()->isEmpty()) //中心部件的内容的根节点不是空  
		{
			loadFile(fileName);   //添加文件  
		}
		else
		{
			ImageProcessor *newImageProcessor = new ImageProcessor;   //创建一个新的my word文档  
			newImageProcessor->show();    //显示出来  
			newImageProcessor->loadFile(fileName);    //添加文件  
		}
	}
}

//实现loadFile函数,这个文件的处理后面会再次进行学习  
void ImageProcessor::loadFile(QString filename)
{
	printf("file name:%s\n", (char *)filename.data());    //输出文件名  

	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream textStream(&file);
		while (!textStream.atEnd())    //循环到后面  
		{
			showWidget->text->append(textStream.readLine());    //给文档添加文件  
			printf("read line\n");
		}
		printf("end\n");
	}

}

//新建文件槽  
void ImageProcessor::ShowNewFile()
{
	//  qDebug()<<"????";  
	ImageProcessor *newImageProcessor = new ImageProcessor;
	newImageProcessor->show();    //一个新的文档显示  
}
