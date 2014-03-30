/********************************************************************************
** Form generated from reading UI file 'cdebugdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CDEBUGDIALOG_H
#define UI_CDEBUGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CDebugDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *MainWidget;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *DrawCallLabel;
    QLabel *label_2;
    QLabel *IndexedDrawCallLabel;
    QLabel *label_3;
    QLabel *FrameTimeLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CDebugDialog)
    {
        if (CDebugDialog->objectName().isEmpty())
            CDebugDialog->setObjectName(QStringLiteral("CDebugDialog"));
        CDebugDialog->resize(314, 300);
        verticalLayout = new QVBoxLayout(CDebugDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        MainWidget = new QWidget(CDebugDialog);
        MainWidget->setObjectName(QStringLiteral("MainWidget"));
        horizontalLayout = new QHBoxLayout(MainWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(MainWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(80, 0));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        DrawCallLabel = new QLabel(MainWidget);
        DrawCallLabel->setObjectName(QStringLiteral("DrawCallLabel"));
        DrawCallLabel->setMaximumSize(QSize(50, 16777215));

        formLayout->setWidget(0, QFormLayout::FieldRole, DrawCallLabel);

        label_2 = new QLabel(MainWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        IndexedDrawCallLabel = new QLabel(MainWidget);
        IndexedDrawCallLabel->setObjectName(QStringLiteral("IndexedDrawCallLabel"));

        formLayout->setWidget(1, QFormLayout::FieldRole, IndexedDrawCallLabel);

        label_3 = new QLabel(MainWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        FrameTimeLabel = new QLabel(MainWidget);
        FrameTimeLabel->setObjectName(QStringLiteral("FrameTimeLabel"));

        formLayout->setWidget(2, QFormLayout::FieldRole, FrameTimeLabel);


        horizontalLayout->addLayout(formLayout);


        verticalLayout->addWidget(MainWidget);

        buttonBox = new QDialogButtonBox(CDebugDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CDebugDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CDebugDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CDebugDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CDebugDialog);
    } // setupUi

    void retranslateUi(QDialog *CDebugDialog)
    {
        CDebugDialog->setWindowTitle(QApplication::translate("CDebugDialog", "Render Debug", 0));
        label->setText(QApplication::translate("CDebugDialog", "Draw Calls", 0));
        DrawCallLabel->setText(QApplication::translate("CDebugDialog", "0", 0));
        label_2->setText(QApplication::translate("CDebugDialog", "Indexed Draw Calls", 0));
        IndexedDrawCallLabel->setText(QApplication::translate("CDebugDialog", "0", 0));
        label_3->setText(QApplication::translate("CDebugDialog", "FrameTime:", 0));
        FrameTimeLabel->setText(QApplication::translate("CDebugDialog", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class CDebugDialog: public Ui_CDebugDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CDEBUGDIALOG_H
