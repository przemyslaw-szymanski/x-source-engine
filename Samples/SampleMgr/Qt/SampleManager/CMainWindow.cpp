#include "CMainWindow.h"
#include "ui_cmainwindow.h"
#include "ui/CRenderWidget.h"
#include "CSampleMgr.h"
#include <qlayout.h>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
	m_pRenderWidget = new CRenderWidget(ui->RenderWidget);
	ui->RenderWidget->layout()->addWidget(m_pRenderWidget);
}

CMainWindow::~CMainWindow()
{
	m_pSampleMgr->UnregisterSamples();
	m_pSampleMgr->DestroyEngine();
    delete ui;
}

void CMainWindow::Init(CSampleMgr* pMgr)
{
	m_pSampleMgr = pMgr;
	unsigned int uiHWND = (unsigned int)m_pRenderWidget->winId();
	pMgr->InitEngine(uiHWND);
	m_pRenderWidget->SetSampleMgr(pMgr);
	pMgr->RegisterSamples();

	for(auto &Itr : pMgr->GetSamples())
	{
		AddSample(Itr.second.pSample->GetName(), Itr.second.pSample->GetGroupName());
	}
}

void CMainWindow::AddSample(const QString& strName, const QString& strGroup)
{
	// Get group
	QTreeWidget* pTree = ui->SampleTree;
	auto& List = pTree->findItems( strGroup, Qt::MatchCaseSensitive );
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
	const std::string strName = pItem->text( 0 ).toUtf8().constData();
	m_pSampleMgr->RunSample( strName );
}
