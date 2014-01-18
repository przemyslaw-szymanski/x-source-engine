/********************************************************************************
** Form generated from reading UI file 'cmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAINWINDOW_H
#define UI_CMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindow
{
public:
    QAction *ActionSolid;
    QAction *ActionWireframe;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *SampleTree;
    QWidget *RenderWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *Layout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRender;
    QMenu *menuMode;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CMainWindow)
    {
        if (CMainWindow->objectName().isEmpty())
            CMainWindow->setObjectName(QStringLiteral("CMainWindow"));
        CMainWindow->resize(1006, 616);
        ActionSolid = new QAction(CMainWindow);
        ActionSolid->setObjectName(QStringLiteral("ActionSolid"));
        ActionSolid->setCheckable(true);
        ActionWireframe = new QAction(CMainWindow);
        ActionWireframe->setObjectName(QStringLiteral("ActionWireframe"));
        ActionWireframe->setCheckable(true);
        centralWidget = new QWidget(CMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        SampleTree = new QTreeWidget(centralWidget);
        SampleTree->setObjectName(QStringLiteral("SampleTree"));
        SampleTree->setMaximumSize(QSize(300, 16777215));

        horizontalLayout->addWidget(SampleTree);

        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        RenderWidget->setMinimumSize(QSize(400, 0));
        verticalLayout = new QVBoxLayout(RenderWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Layout = new QVBoxLayout();
        Layout->setSpacing(6);
        Layout->setObjectName(QStringLiteral("Layout"));

        verticalLayout->addLayout(Layout);


        horizontalLayout->addWidget(RenderWidget);


        horizontalLayout_2->addLayout(horizontalLayout);

        CMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1006, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRender = new QMenu(menuBar);
        menuRender->setObjectName(QStringLiteral("menuRender"));
        menuMode = new QMenu(menuRender);
        menuMode->setObjectName(QStringLiteral("menuMode"));
        CMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CMainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRender->menuAction());
        menuRender->addAction(menuMode->menuAction());
        menuMode->addAction(ActionSolid);
        menuMode->addAction(ActionWireframe);

        retranslateUi(CMainWindow);

        QMetaObject::connectSlotsByName(CMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindow)
    {
        CMainWindow->setWindowTitle(QApplication::translate("CMainWindow", "CMainWindow", 0));
        ActionSolid->setText(QApplication::translate("CMainWindow", "Solid", 0));
        ActionWireframe->setText(QApplication::translate("CMainWindow", "Wireframe", 0));
        QTreeWidgetItem *___qtreewidgetitem = SampleTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("CMainWindow", "Sample", 0));
        menuFile->setTitle(QApplication::translate("CMainWindow", "File", 0));
        menuRender->setTitle(QApplication::translate("CMainWindow", "Render", 0));
        menuMode->setTitle(QApplication::translate("CMainWindow", "Mode", 0));
    } // retranslateUi

};

namespace Ui {
    class CMainWindow: public Ui_CMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOW_H
