#pragma once

#include <QtWidgets/QMainWindow>
#include <QImage>  
#include <QLabel>  
#include <QMenu>  
#include <QMenuBar>  
#include <QAction>  
#include <QComboBox>  
#include <QSpinBox>  
#include <QToolBar>  
#include <QFontComboBox>  
#include <QToolButton>  
#include <QTextCharFormat>  
#include "ShowWidget.h"

class ImageProcessor : public QMainWindow
{
	Q_OBJECT

public:
	ImageProcessor(QWidget *parent = Q_NULLPTR);

	void createActions();		//创建动作
	void createMenus();			//创建菜单
	void createToolBars();		//创建工具栏

	void loadFile(QString filename);
	void mergeFormat(QTextCharFormat);
private:
	//文字排版功能  
	QLabel * listLabel;    //标签，“排序”  
	QComboBox *listComboBox;    //各种不同的排序,下拉框  
	QActionGroup *actGrp;   //一个action组  
	QAction *leftAction;    //左对齐  
	QAction *rightAction;   //右对齐  
	QAction *centerAction;    //居中对齐  
	QAction *justifyAction;     //两端对齐  
	QToolBar *listToolBar;    //存放排序工具的工具栏  

							  //文字工具栏，实现对文字的编辑功能  
	QLabel *fontLabel1;   //标签，字体  
	QFontComboBox *fontComboBox;    //字体的选择下拉  
	QLabel *fontLabel2;   //字号  
	QComboBox *sizeComboBox;    //字号下拉  
	QToolButton *boldBtn;   //加粗  
	QToolButton *italicBtn;   //斜体  
	QToolButton *underlineBtn;    //下划线  
	QToolButton *colorBtn;    //字体颜色选择  

							  //添加一个存放这些字体处理工具的工具栏  
	QToolBar *fontToolBar;

	//各项菜单栏  
	QMenu *fileMenu;
	QMenu *zoomMenu;
	QMenu *rotateMenu;
	QMenu *mirrorMenu;

	QImage img;   //图片  
	QString fileName;     //文件名  
	ShowWidget *showWidget;     //显示流  

								//文件菜单项  
	QAction *openFileAction;
	QAction *NewFileAction;
	QAction *PrintTextAction;
	QAction *PrintImageAction;
	QAction *exitAction;

	//编辑菜单项  
	QAction *copyAction;
	QAction *cutAction;
	QAction *pasteAction;
	QAction *aboutAction;
	QAction *zoomInAction;
	QAction *zoomOutAction;

	//旋转菜单项  
	QAction *rotate90Action;
	QAction *rotate180Action;
	QAction *rotate270Action;

	//镜像菜单项  
	QAction *mirrorVerticalAction;
	QAction *mirrorHorizontalAction;


	QAction *undoAction;
	QAction *redoAction;

	//工具栏  
	QToolBar *fileTool;
	QToolBar *zoomTool;
	QToolBar *rotateTool;
	QToolBar *mirrorTool;
	QToolBar *doToolBar;


	protected slots:    //添加函数槽  
	void ShowList(int);   //显示排序列表  
	void ShowAlignment(QAction *act);   //展示不同的排序结果  
	void ShowCursorPositionChanged();   //光标位置改变？？？  

										//-------------------------------------------------------------------------------  
	void ShowNewFile();   //新建一个新的文件  
	void ShowOpenFile();    //打开文件  
	void ShowPrintText();   //打印功能的实现  
	void ShowPrintImage();    //打印图片  
	void ShowZoomIn();    //关于图片的缩放功能  
	void ShowZoomOut();   //图片缩小  
	void ShowRotate90();    //旋转90度  
	void ShowRotate180();   //180度  
	void ShowRotate270();   //270度  
	void ShowMirrorVertical();    //水平镜像  
	void ShowMirrorHorizontal();    //垂直镜像  
									//-------------------------------------------------------------------------------  
									//添加文字处理的相应槽函数  
	void ShowFontComboBox(QString comboStr);    //显示字体的改变  
	void ShowSizeSpinBox(QString spinValue);    //字号的改变  
	void ShowBoldBtn();   //加粗  
	void ShowItalicBtn();   //斜体  
	void ShowUnderlineBtn();      //下划线  
	void ShowColorBtn();      //颜色的改变  
	void ShowCurrentFormatChanged(const QTextCharFormat &fmt);    //设置字符格式  
																  //当光标所在的字符格式改变的时候，调用这个函数，这个函数根据新的格式将工具栏上的控件显示更新  
};
