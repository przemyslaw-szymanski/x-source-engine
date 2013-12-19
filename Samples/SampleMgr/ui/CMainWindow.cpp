/****************************************************************************
** Meta object code from reading C++ file 'CMainWindow.h'
**
** Created: Thu 19. Apr 19:10:51 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CMainWindow.h"
#include "../CSampleMgr.h"
#include "CRenderWidget.h"
#include "CRenderInfo.h"

CMainWindow::CMainWindow(QWidget* pParent, Qt::WFlags iFlags) : QMainWindow( pParent, iFlags )
{
	this->setMinimumSize( 1200, 700 );
	m_MainWndUi.setupUi( this );
	m_MainWndUi.pLeftPanel->setMaximumWidth( 200 );
	m_MainWndUi.pRenderFrame->setMinimumSize( 820, 620 );
	m_pRenderWidget = new CRenderWidget( m_MainWndUi.pRenderFrame );
	m_pRenderWidget->setMinimumSize( 800, 600 );
	m_pRenderWidget->setFocusPolicy( Qt::ClickFocus );
	QVBoxLayout* pRenderLayout = new QVBoxLayout( m_MainWndUi.pRenderFrame );
	pRenderLayout->addWidget( m_pRenderWidget );
	m_MainWndUi.pRenderFrame->setLayout( pRenderLayout );

	//CRenderInfo::GetSingleton().m_pParent = m_MainWndUi.pRenderFrame;

	this->setFocusPolicy( Qt::StrongFocus );
	

	connect( m_MainWndUi.pMenuResetCameraPositionAction, SIGNAL( triggered(bool) ), this, SLOT( OnMenuCameraResetPositionActionClicked(bool) ) );
	connect( m_MainWndUi.pMenuSolidAction, SIGNAL( triggered(bool) ), this, SLOT( OnMenuSolidActionChanged(bool) ) );
	connect( m_MainWndUi.pMenuWireframeAction, SIGNAL( triggered(bool) ), this, SLOT( OnMenuWireframeActionChanged(bool) ) );
}

CMainWindow::~CMainWindow()
{
	
}

void CMainWindow::keyPressEvent(QKeyEvent* pEvent)
{
	if( m_pSampleMgr != NULL && m_pSampleMgr->GetCurrentSample() != NULL )
	{
		m_pRenderWidget->keyPressEvent( pEvent );
	}
}

void CMainWindow::OnSampleListCurrentChanged(int iIndex)
{
	int id = iIndex;
}

void CMainWindow::OnMenuCameraResetPositionActionClicked(bool bChecked)
{
	m_pSampleMgr->ResetCamera();
}

void CMainWindow::OnMenuSolidActionChanged(bool bChecked)
{
	m_pSampleMgr->RenderSolid();
	m_MainWndUi.pMenuWireframeAction->setChecked( false );
}

void CMainWindow::OnMenuWireframeActionChanged(bool bChecked)
{
	m_pSampleMgr->RenderWireframe();
	m_MainWndUi.pMenuSolidAction->setChecked( false );
}

bool CMainWindow::Init(CSampleMgr* pMgr)
{
	m_pSampleMgr = pMgr;

	u32 uiHWND = (u32)m_pRenderWidget->winId();
	m_pSampleMgr->InitEngine( uiHWND );

	m_pRenderWidget->SetSampleMgr( m_pSampleMgr );

	m_pSampleMgr->RegisterSamples();

	//g_SampleMgr.RunSample( "CustomGeometry" );

	CSampleMgr::SampleMap::const_iterator Itr;
	xst_stl_foreach( Itr, m_pSampleMgr->GetSamples() )
	{
		AddSample( Itr->second.pSample->GetName(), Itr->second.pSample->GetGroupName() );
	}

	//g_SampleMgr.RunSample( "CustomGeometry" );

	return true;
}

void CMainWindow::AddSample(const QString& strName, const QString& strGroup)
{
	QToolBox* pBox = m_MainWndUi.pSamplesToolBox;
	//m_MainWndUi.SamplesToolBox->addItem( new QPushButton( strName, this ), strGroup );
	//m_MainWndUi.pSamplesToolBox->insertItem( 0, new QPushButton( strName ), strGroup );
	//Get group tab
	std::string strGroupName( (const char*)strGroup.toAscii() );
	GroupWidget* pWgt = GetTab( strGroupName );
	if( pWgt == NULL )
	{
		pWgt = new GroupWidget();
		m_mGroups.insert( GroupMap::value_type( strGroupName, pWgt ) );
		pBox->addItem( pWgt, strGroup );

		connect( pWgt, SIGNAL( itemDoubleClicked(QListWidgetItem*) ), this, SLOT( OnItemDoubleClicked(QListWidgetItem*) ) );
	}

	QListWidgetItem* pItem = new QListWidgetItem( strName );

	pWgt->addItem( pItem );
	pWgt->sortItems();
}

CMainWindow::GroupWidget* CMainWindow::GetTab(const std::string& strName)
{
	GroupMap::iterator Itr = m_mGroups.find( strName );
	if( Itr == m_mGroups.end() )
	{
		return NULL;
	}

	return Itr->second;
}

void CMainWindow::OnItemDoubleClicked(QListWidgetItem* pItem)
{
	//this->setFocus();
	//this->setFocus( Qt::ActiveWindowFocusReason );
	m_pRenderWidget->setFocus( Qt::FocusReason::MouseFocusReason );
	m_pSampleMgr->RunSample( (const char*)pItem->text().toAscii() );
}