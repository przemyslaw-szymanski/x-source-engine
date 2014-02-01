#ifndef CDEBUGDIALOG_H
#define CDEBUGDIALOG_H

#include <QDialog>

namespace Ui {
class CDebugDialog;
}

class CDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDebugDialog(QWidget *parent = 0);
    ~CDebugDialog();

	void SetDrawCallCount(int iCount);
	void SetIndexedDrawCallCount(int iCount);

private:
    Ui::CDebugDialog *ui;
};

#endif // CDEBUGDIALOG_H
