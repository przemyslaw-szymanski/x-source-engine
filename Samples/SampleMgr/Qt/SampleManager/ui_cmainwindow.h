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
    QAction *ActionDebugWindow;
    QAction *ActionCullNone;
    QAction *ActionCullSphere;
    QAction *ActionCullAABB;
    QAction *ActionCullSphereAABB;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *SampleTree;
    QWidget *RenderWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *Layout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRender;
    QMenu *menuMode;
    QMenu *menuView_Frustum_Culling;
    QMenu *menuView;

    void setupUi(QMainWindow *CMainWindow)
    {
        if (CMainWindow->objectName().isEmpty())
            CMainWindow->setObjectName(QStringLiteral("CMainWindow"));
        CMainWindow->resize(1006, 616);
        ActionSolid = new QAction(CMainWindow);
        ActionSolid->setObjectName(QStringLiteral("ActionSolid"));
        ActionSolid->setCheckable(true);
        ActionSolid->setChecked(true);
        ActionWireframe = new QAction(CMainWindow);
        ActionWireframe->setObjectName(QStringLiteral("ActionWireframe"));
        ActionWireframe->setCheckable(true);
        ActionDebugWindow = new QAction(CMainWindow);
        ActionDebugWindow->setObjectName(QStringLiteral("ActionDebugWindow"));
        ActionCullNone = new QAction(CMainWindow);
        ActionCullNone->setObjectName(QStringLiteral("ActionCullNone"));
        ActionCullNone->setCheckable(true);
        ActionCullSphere = new QAction(CMainWindow);
        ActionCullSphere->setObjectName(QStringLiteral("ActionCullSphere"));
        ActionCullSphere->setCheckable(true);
        ActionCullAABB = new QAction(CMainWindow);
        ActionCullAABB->setObjectName(QStringLiteral("ActionCullAABB"));
        ActionCullAABB->setCheckable(true);
        ActionCullSphereAABB = new QAction(CMainWindow);
        ActionCullSphereAABB->setObjectName(QStringLiteral("ActionCullSphereAABB"));
        ActionCullSphereAABB->setCheckable(true);
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
        mainToolBar = new QToolBar(CMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CMainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(CMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1006, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRender = new QMenu(menuBar);
        menuRender->setObjectName(QStringLiteral("menuRender"));
        menuMode = new QMenu(menuRender);
        menuMode->setObjectName(QStringLiteral("menuMode"));
        menuView_Frustum_Culling = new QMenu(menuRender);
        menuView_Frustum_Culling->setObjectName(QStringLiteral("menuView_Frustum_Culling"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        CMainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRender->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuRender->addAction(menuMode->menuAction());
        menuRender->addAction(menuView_Frustum_Culling->menuAction());
        menuMode->addAction(ActionSolid);
        menuMode->addAction(ActionWireframe);
        menuView_Frustum_Culling->addAction(ActionCullNone);
        menuView_Frustum_Culling->addAction(ActionCullSphere);
        menuView_Frustum_Culling->addAction(ActionCullAABB);
        menuView_Frustum_Culling->addAction(ActionCullSphereAABB);
        menuView->addAction(ActionDebugWindow);

        retranslateUi(CMainWindow);

        QMetaObject::connectSlotsByName(CMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindow)
    {
        CMainWindow->setWindowTitle(QApplication::translate("CMainWindow", "CMainWindow", 0));
        ActionSolid->setText(QApplication::translate("CMainWindow", "Solid", 0));
        ActionWireframe->setText(QApplication::translate("CMainWindow", "Wireframe", 0));
        ActionDebugWindow->setText(QApplication::translate("CMainWindow", "Debug Window", 0));
        ActionCullNone->setText(QApplication::translate("CMainWindow", "None", 0));
        ActionCullSphere->setText(QApplication::translate("CMainWindow", "Sphere", 0));
        ActionCullAABB->setText(QApplication::translate("CMainWindow", "AABB", 0));
        ActionCullSphereAABB->setText(QApplication::translate("CMainWindow", "Sphere And AABB", 0));
        QTreeWidgetItem *___qtreewidgetitem = SampleTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("CMainWindow", "Sample", 0));
        menuFile->setTitle(QApplication::translate("CMainWindow", "File", 0));
        menuRender->setTitle(QApplication::translate("CMainWindow", "Render", 0));
        menuMode->setTitle(QApplication::translate("CMainWindow", "Mode", 0));
        menuView_Frustum_Culling->setTitle(QApplication::translate("CMainWindow", "View Frustum Culling", 0));
        menuView->setTitle(QApplication::translate("CMainWindow", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class CMainWindow: public Ui_CMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOW_H
