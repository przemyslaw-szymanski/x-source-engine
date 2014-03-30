#include "cdebugdialog.h"
#include "ui_cdebugdialog.h"

CDebugDialog::CDebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDebugDialog)
{
    ui->setupUi(this);
}

CDebugDialog::~CDebugDialog()
{
    delete ui;
}

void CDebugDialog::SetDrawCallCount(int iCount)
{
	if( !this->isVisible() )
		return;
	QString s = QString( "%1" ).arg( iCount );
	this->ui->DrawCallLabel->setText( s );
}

void CDebugDialog::SetIndexedDrawCallCount(int iCount)
{
	this->ui->IndexedDrawCallLabel->setText(QString( "%1" ).arg( iCount ));
}

void CDebugDialog::SetFrameTime(float iCount)
{
	this->ui->FrameTimeLabel->setText(QString( "%1" ).arg( iCount ));
}