#ifndef XSE_CSCENE_DEBUG_H
#define XSE_CSCENE_DEBUG_H

#include "XSECommon.h"
#include "XSECMesh.h"

namespace XSE
{
	class CSceneDebug
	{
		friend class CSceneManager;

		public:

								CSceneDebug(CSceneManager* pMgr);
			virtual				~CSceneDebug();

			void				ShowPartitionSystem(bool bShow);

			void				ShowObjectsAABB(bool bShow);

			void				SetAABBMesh(MeshPtr pMesh)
								{ m_pAABBMesh = pMesh; }

			MeshPtr				GetAABBMesh()
								{ return m_pAABBMesh; }

			const	MeshPtr&	GetAABBMesh() const
								{ return m_pAABBMesh; }

		protected:

			i32					_CreateObjectAABBMesh(CObject* pObj);

			void				_UpdateObjectAABBMesh(CObject* pObj);
	
		protected:

			CSceneManager*	m_pMgr;
			MeshPtr			m_pAABBMesh;
	};
}

#endif XSE_CSCENE_DEBUG_H