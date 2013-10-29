#ifndef XSE_COCTREE_SCENE_PARTITION_SYSTEM_H
#define XSE_COCTREE_SCENE_PARTITION_SYSTEM_H

#include "XSEIScenePartitionSystem.h"
#include "XSECModel.h"

namespace XSE
{
	class COctree;
	class COctreeListener;

	class COctreeScenePartitionSystem : public IScenePartitionSystem
	{
		friend class CSceneManager;
		friend class COctreeListener;

		typedef xst_vector< COctree* >	OctreeVec;

		public:
									
									COctreeScenePartitionSystem(CSceneManager* pSceneMgr);
			virtual					~COctreeScenePartitionSystem();

					i32				Init();

					void			Update();
			
					void			AddObject(CObject* pObj);
			
					void			RemoveObject(CObject* pObj);

					void			StartProcessing();

					void			StopProcessing();

					void			SkipProcess();

		protected:

					void			_OctRangeCull(COctree* pNode, const CBoundingSphere& CamSphere);
					void			_OctRangeCull(const CBoundingSphere& CamSphere);

					void			_OctFrustumCull(COctree* pNode, const CCamera* pCamera);

		protected:

			ModelPtr			m_pOctModel;
			IInputLayout*		m_pInputLayout;
			COctree*			m_pOctree;
			COctreeListener*	m_pOctListener;
			xst_astring			m_strSceneNodeName;
			OctreeVec			m_vOctNodes;
			u8					m_byMaxOctreeDepth;
			bool				m_bProcessingStopped;
			bool				m_bProcessSkipped;
	};
}//xse

#endif //XSE_COCTREE_SCENE_PARTITION_SYSTEM_H