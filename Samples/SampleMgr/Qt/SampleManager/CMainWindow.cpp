#include "CMainWindow.h"
#include "ui_cmainwindow.h"
#include <qlayout.h>
#include <qdesktopwidget.h>

#if defined(_MSC_VER)
#include "ui/CRenderWidget.h"
#include "CSampleMgr.h"
#endif

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
	QDesktopWidget Desktop;
	QRect ScrSize = Desktop.availableGeometry( Desktop.primaryScreen() );
	QSize Size( ScrSize.width() * 0.8f, ScrSize.height() * 0.8f );
	this->setFixedSize( Size );
    ui->setupUi(this);
#if defined(_MSC_VER)
	m_pRenderWidget = new CRenderWidget(ui->RenderWidget);
	ui->RenderWidget->layout()->addWidget(m_pRenderWidget);
#endif
}

CMainWindow::~CMainWindow()
{
#if defined(_MSC_VER)
	m_pSampleMgr->UnregisterSamples();
	m_pSampleMgr->DestroyEngine();
#endif
    delete ui;
}

void CMainWindow::Init(CSampleMgr* pMgr)
{
#if defined(_MSC_VER)
	m_pSampleMgr = pMgr;
	unsigned int uiHWND = (unsigned int)m_pRenderWidget->winId();
	pMgr->InitEngine(uiHWND);
	m_pRenderWidget->SetSampleMgr(pMgr);
	pMgr->RegisterSamples();

	for(auto &Itr : pMgr->GetSamples())
	{
		AddSample(Itr.second.pSample->GetName(), Itr.second.pSample->GetGroupName());
	}
#endif
}

void CMainWindow::AddSample(const QString& strName, const QString& strGroup)
{
	// Get group
	QTreeWidget* pTree = ui->SampleTree;
    auto List = pTree->findItems( strGroup, Qt::MatchCaseSensitive );
	if( List.size() > 0 )
	{
		if( List[ 0 ]->text( 0 ) == strName )
			return;
		List[ 0 ]->addChild( new QTreeWidgetItem( QStringList( strName ) ) );
	}
	else
	{
		QTreeWidgetItem* pItem = new QTreeWidgetItem( pTree );
		pItem->setText( 0, strGroup );
		pTree->addTopLevelItem( pItem );
		pItem->addChild( new QTreeWidgetItem( QStringList( strName ) ) );
	}
	
}

void CMainWindow::on_SampleTree_itemDoubleClicked(QTreeWidgetItem *pItem, int column)
{
#if defined(_MSC_VER)
	const std::string strName = pItem->text( 0 ).toUtf8().constData();
	m_pSampleMgr->RunSample( strName );
#endif
}

void CMainWindow::on_ActionSolid_toggled(bool arg1)
{

}

void CMainWindow::on_ActionWireframe_toggled(bool arg1)
{

}

void CMainWindow::on_ActionWireframe_changed()
{

}

void CMainWindow::on_ActionWireframe_triggered(bool checked)
{
	ui->ActionSolid->setChecked( false );
#if defined(_MSC_VER)
	if( m_pSampleMgr )
		m_pSampleMgr->RenderWireframe();
#endif
}

void CMainWindow::on_ActionSolid_triggered(bool checked)
{
    ui->ActionSolid->setChecked(false);
#if defined(_MSC_VER)
	if( m_pSampleMgr )
		m_pSampleMgr->RenderSolid();
#endif
}
