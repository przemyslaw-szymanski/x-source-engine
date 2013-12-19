/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created: Fri 4. May 14:33:53 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindow
{
public:
    QAction *pMenuSolidAction;
    QAction *pMenuWireframeAction;
    QAction *pMenuResetCameraPositionAction;
    QAction *pMenuCameraPerspective;
    QWidget *pCenterPanel;
    QHBoxLayout *horizontalLayout;
    QWidget *pMainPanel;
    QHBoxLayout *horizontalLayout_2;
    QWidget *pLeftPanel;
    QVBoxLayout *pLeftPanelLayout;
    QToolBox *pSamplesToolBox;
    QWidget *pRightPanel;
    QHBoxLayout *horizontalLayout_3;
    QFrame *pRenderFrame;
    QMenuBar *pMainMenu;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuRender_Mode;
    QMenu *menuCamera;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CMainWindow)
    {
        if (CMainWindow->objectName().isEmpty())
            CMainWindow->setObjectName(QString::fromUtf8("CMainWindow"));
        CMainWindow->resize(711, 532);
        pMenuSolidAction = new QAction(CMainWindow);
        pMenuSolidAction->setObjectName(QString::fromUtf8("pMenuSolidAction"));
        pMenuSolidAction->setCheckable(true);
        pMenuSolidAction->setChecked(true);
        pMenuWireframeAction = new QAction(CMainWindow);
        pMenuWireframeAction->setObjectName(QString::fromUtf8("pMenuWireframeAction"));
        pMenuWireframeAction->setCheckable(true);
        pMenuResetCameraPositionAction = new QAction(CMainWindow);
        pMenuResetCameraPositionAction->setObjectName(QString::fromUtf8("pMenuResetCameraPositionAction"));
        pMenuCameraPerspective = new QAction(CMainWindow);
        pMenuCameraPerspective->setObjectName(QString::fromUtf8("pMenuCameraPerspective"));
        pCenterPanel = new QWidget(CMainWindow);
        pCenterPanel->setObjectName(QString::fromUtf8("pCenterPanel"));
        horizontalLayout = new QHBoxLayout(pCenterPanel);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pMainPanel = new QWidget(pCenterPanel);
        pMainPanel->setObjectName(QString::fromUtf8("pMainPanel"));
        horizontalLayout_2 = new QHBoxLayout(pMainPanel);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pLeftPanel = new QWidget(pMainPanel);
        pLeftPanel->setObjectName(QString::fromUtf8("pLeftPanel"));
        pLeftPanel->setMinimumSize(QSize(100, 0));
        pLeftPanel->setMaximumSize(QSize(500, 16777215));
        pLeftPanelLayout = new QVBoxLayout(pLeftPanel);
        pLeftPanelLayout->setSpacing(3);
        pLeftPanelLayout->setObjectName(QString::fromUtf8("pLeftPanelLayout"));
        pLeftPanelLayout->setSizeConstraint(QLayout::SetMinimumSize);
        pLeftPanelLayout->setContentsMargins(0, 9, 0, 9);
        pSamplesToolBox = new QToolBox(pLeftPanel);
        pSamplesToolBox->setObjectName(QString::fromUtf8("pSamplesToolBox"));

        pLeftPanelLayout->addWidget(pSamplesToolBox);


        horizontalLayout_2->addWidget(pLeftPanel);

        pRightPanel = new QWidget(pMainPanel);
        pRightPanel->setObjectName(QString::fromUtf8("pRightPanel"));
        pRightPanel->setMinimumSize(QSize(500, 0));
        horizontalLayout_3 = new QHBoxLayout(pRightPanel);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pRenderFrame = new QFrame(pRightPanel);
        pRenderFrame->setObjectName(QString::fromUtf8("pRenderFrame"));
        pRenderFrame->setFrameShape(QFrame::Box);
        pRenderFrame->setFrameShadow(QFrame::Sunken);
        pRenderFrame->setLineWidth(1);
        pRenderFrame->setMidLineWidth(0);

        horizontalLayout_3->addWidget(pRenderFrame);


        horizontalLayout_2->addWidget(pRightPanel);


        horizontalLayout->addWidget(pMainPanel);

        CMainWindow->setCentralWidget(pCenterPanel);
        pMainMenu = new QMenuBar(CMainWindow);
        pMainMenu->setObjectName(QString::fromUtf8("pMainMenu"));
        pMainMenu->setGeometry(QRect(0, 0, 711, 21));
        menuFile = new QMenu(pMainMenu);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(pMainMenu);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuRender_Mode = new QMenu(menuEdit);
        menuRender_Mode->setObjectName(QString::fromUtf8("menuRender_Mode"));
        menuCamera = new QMenu(menuEdit);
        menuCamera->setObjectName(QString::fromUtf8("menuCamera"));
        CMainWindow->setMenuBar(pMainMenu);
        statusbar = new QStatusBar(CMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CMainWindow->setStatusBar(statusbar);

        pMainMenu->addAction(menuFile->menuAction());
        pMainMenu->addAction(menuEdit->menuAction());
        menuEdit->addAction(menuRender_Mode->menuAction());
        menuEdit->addSeparator();
        menuEdit->addAction(menuCamera->menuAction());
        menuRender_Mode->addAction(pMenuSolidAction);
        menuRender_Mode->addAction(pMenuWireframeAction);
        menuCamera->addAction(pMenuResetCameraPositionAction);
        menuCamera->addAction(pMenuCameraPerspective);

        retranslateUi(CMainWindow);

        pSamplesToolBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(CMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindow)
    {
        CMainWindow->setWindowTitle(QApplication::translate("CMainWindow", "Sample Manager", 0, QApplication::UnicodeUTF8));
        pMenuSolidAction->setText(QApplication::translate("CMainWindow", "Solid", 0, QApplication::UnicodeUTF8));
        pMenuWireframeAction->setText(QApplication::translate("CMainWindow", "Wireframe", 0, QApplication::UnicodeUTF8));
        pMenuResetCameraPositionAction->setText(QApplication::translate("CMainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        pMenuCameraPerspective->setText(QApplication::translate("CMainWindow", "Perspective", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("CMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("CMainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuRender_Mode->setTitle(QApplication::translate("CMainWindow", "Render Mode", 0, QApplication::UnicodeUTF8));
        menuCamera->setTitle(QApplication::translate("CMainWindow", "Camera", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CMainWindow: public Ui_CMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAIN_H
