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

	void createActions();		//��������
	void createMenus();			//�����˵�
	void createToolBars();		//����������

	void loadFile(QString filename);
	void mergeFormat(QTextCharFormat);
private:
	//�����Ű湦��  
	QLabel * listLabel;    //��ǩ��������  
	QComboBox *listComboBox;    //���ֲ�ͬ������,������  
	QActionGroup *actGrp;   //һ��action��  
	QAction *leftAction;    //�����  
	QAction *rightAction;   //�Ҷ���  
	QAction *centerAction;    //���ж���  
	QAction *justifyAction;     //���˶���  
	QToolBar *listToolBar;    //������򹤾ߵĹ�����  

							  //���ֹ�������ʵ�ֶ����ֵı༭����  
	QLabel *fontLabel1;   //��ǩ������  
	QFontComboBox *fontComboBox;    //�����ѡ������  
	QLabel *fontLabel2;   //�ֺ�  
	QComboBox *sizeComboBox;    //�ֺ�����  
	QToolButton *boldBtn;   //�Ӵ�  
	QToolButton *italicBtn;   //б��  
	QToolButton *underlineBtn;    //�»���  
	QToolButton *colorBtn;    //������ɫѡ��  

							  //���һ�������Щ���崦���ߵĹ�����  
	QToolBar *fontToolBar;

	//����˵���  
	QMenu *fileMenu;
	QMenu *zoomMenu;
	QMenu *rotateMenu;
	QMenu *mirrorMenu;

	QImage img;   //ͼƬ  
	QString fileName;     //�ļ���  
	ShowWidget *showWidget;     //��ʾ��  

								//�ļ��˵���  
	QAction *openFileAction;
	QAction *NewFileAction;
	QAction *PrintTextAction;
	QAction *PrintImageAction;
	QAction *exitAction;

	//�༭�˵���  
	QAction *copyAction;
	QAction *cutAction;
	QAction *pasteAction;
	QAction *aboutAction;
	QAction *zoomInAction;
	QAction *zoomOutAction;

	//��ת�˵���  
	QAction *rotate90Action;
	QAction *rotate180Action;
	QAction *rotate270Action;

	//����˵���  
	QAction *mirrorVerticalAction;
	QAction *mirrorHorizontalAction;


	QAction *undoAction;
	QAction *redoAction;

	//������  
	QToolBar *fileTool;
	QToolBar *zoomTool;
	QToolBar *rotateTool;
	QToolBar *mirrorTool;
	QToolBar *doToolBar;


	protected slots:    //��Ӻ�����  
	void ShowList(int);   //��ʾ�����б�  
	void ShowAlignment(QAction *act);   //չʾ��ͬ��������  
	void ShowCursorPositionChanged();   //���λ�øı䣿����  

										//-------------------------------------------------------------------------------  
	void ShowNewFile();   //�½�һ���µ��ļ�  
	void ShowOpenFile();    //���ļ�  
	void ShowPrintText();   //��ӡ���ܵ�ʵ��  
	void ShowPrintImage();    //��ӡͼƬ  
	void ShowZoomIn();    //����ͼƬ�����Ź���  
	void ShowZoomOut();   //ͼƬ��С  
	void ShowRotate90();    //��ת90��  
	void ShowRotate180();   //180��  
	void ShowRotate270();   //270��  
	void ShowMirrorVertical();    //ˮƽ����  
	void ShowMirrorHorizontal();    //��ֱ����  
									//-------------------------------------------------------------------------------  
									//������ִ������Ӧ�ۺ���  
	void ShowFontComboBox(QString comboStr);    //��ʾ����ĸı�  
	void ShowSizeSpinBox(QString spinValue);    //�ֺŵĸı�  
	void ShowBoldBtn();   //�Ӵ�  
	void ShowItalicBtn();   //б��  
	void ShowUnderlineBtn();      //�»���  
	void ShowColorBtn();      //��ɫ�ĸı�  
	void ShowCurrentFormatChanged(const QTextCharFormat &fmt);    //�����ַ���ʽ  
																  //��������ڵ��ַ���ʽ�ı��ʱ�򣬵������������������������µĸ�ʽ���������ϵĿؼ���ʾ����  
};
