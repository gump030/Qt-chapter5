/********************************************************************************
** Form generated from reading UI file 'ImgProcessor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGPROCESSOR_H
#define UI_IMGPROCESSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImgProcessorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImgProcessorClass)
    {
        if (ImgProcessorClass->objectName().isEmpty())
            ImgProcessorClass->setObjectName(QStringLiteral("ImgProcessorClass"));
        ImgProcessorClass->resize(600, 400);
        menuBar = new QMenuBar(ImgProcessorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ImgProcessorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImgProcessorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ImgProcessorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ImgProcessorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ImgProcessorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImgProcessorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ImgProcessorClass->setStatusBar(statusBar);

        retranslateUi(ImgProcessorClass);

        QMetaObject::connectSlotsByName(ImgProcessorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImgProcessorClass)
    {
        ImgProcessorClass->setWindowTitle(QApplication::translate("ImgProcessorClass", "ImgProcessor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImgProcessorClass: public Ui_ImgProcessorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMGPROCESSOR_H
