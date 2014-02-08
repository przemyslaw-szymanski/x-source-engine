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
		//typedef void ( *pfnCullTest )(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);
		using pfnCullTest = void (*)(const CCamera*, const CBoundingVolume&, OBJECT_DISABLE_REASON*);

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

					void			SetViewFrustumCullTestType(const VIEW_FRUSTUM_CULL_TEST_TYPE& eType);

		protected:

					void			_OctRangeCull(COctree* pNode, const CBoundingSphere& CamSphere);
					void			_OctRangeCull(const CBoundingSphere& CamSphere);

					void			_OctFrustumCull(COctree* pNode, const CCamera* pCamera);
					void			_OctFrustumCull(const CCamera* pCamera);

					void			_NodeObjectsFrustumCull(COctree* pNode, const CCamera* pCamera);
					void			_NodeObjectsRangeCull(COctree* pNode, const CBoundingSphere& CamSphere);

		protected:

			xst_astring			m_strSceneNodeName;
			OctreeVec			m_vOctNodes;
			ModelPtr			m_pOctModel;
			IInputLayout*		m_pInputLayout = xst_null;
			COctree*			m_pOctree = xst_null;
			COctreeListener*	m_pOctListener = xst_null;
			pfnCullTest			m_CullTest = xst_null;
			u8					m_byMaxOctreeDepth;
			bool				m_bProcessingStopped;
			bool				m_bProcessSkipped;
	};
}//xse

#endif //XSE_COCTREE_SCENE_PARTITION_SYSTEM_H