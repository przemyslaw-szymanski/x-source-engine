#include "XSECNoScenePartitionSystem.h"

namespace XSE
{
	CNoScenePartitionSystem::CNoScenePartitionSystem(CSceneManager* pSceneMgr) : IScenePartitionSystem( pSceneMgr )
	{
	}

	CNoScenePartitionSystem::~CNoScenePartitionSystem()
	{
		m_pRS = xst_null;
	}

	i32 CNoScenePartitionSystem::Draw()
	{
		return XST_OK;
	}

}//xse