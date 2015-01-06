#ifndef XSE_CMESH_MANAGER_H
#define XSE_CMESH_MANAGER_H

#include "XSECommon.h"
#include "XSEIResourceManager.h"
#include "XSECMesh.h"

namespace XSE
{
	class XST_API CMeshManager : public XST::TCSingleton< CMeshManager >, public IResourceManager
	{
		friend class CEngine;

		public:

										CMeshManager(CMaterialManager* pMatMgr);
			virtual						~CMeshManager();

                    i32					Init() xst_implement;

					i32					PrepareResource(ResourceWeakPtr pRes) xst_implement;

			xst_fi	void				SetDefaultMeshLODCount(u16 usCount)
										{ m_usDefaultLODCount = usCount; }

			xst_fi	u16					GetDefaultMeshLODCount() const
										{ return m_usDefaultLODCount; }

					MeshWeakPtr 		CreateMesh(xst_castring& strName, ul32 ulInputLayout, xst_castring& strGroupName = DEFAULT_GROUP);

					MeshWeakPtr			CreateMesh(xst_castring& strName, ul32 ulInputLayout, BASIC_SHAPE eShape, xst_unknown pShapeOptions, xst_castring& strGroupName = DEFAULT_GROUP);

					MeshWeakPtr			CreateMesh(xst_castring& strName, const IInputLayout* pIL, xst_castring& strGroupName = DEFAULT_GROUP);

					MeshWeakPtr			CreateMesh(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);

					MeshWeakPtr			CreateMesh(xst_castring& strName, GroupWeakPtr pResourceGroup);

					MeshWeakPtr			CreateMesh(xst_castring& strName, const IInputLayout* pIL, BASIC_SHAPE eShape, xst_unknown pShapeOptions, xst_castring& strGroupName = DEFAULT_GROUP);

					MeshWeakPtr			LoadMesh(xst_castring& strName, xst_castring& strGroup = DEFAULT_GROUP);

					//i32					DestroyMesh(MeshPtr& pMesh, xst_castring& strGroupName = ALL_GROUPS);

			xst_fi	MeshWeakPtr			GetDefaultMesh()
										{ return m_pDefaultMesh; }

					ResourceWeakPtr  	CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true) xst_implement;

			xst_fi	MeshWeakPtr			CloneMesh(const MeshWeakPtr& pMesh, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true)
										{ return CloneResource( pMesh.GetPtr(), strNewName, bFullClone ); }

					i32					SetDefaultMesh(MeshWeakPtr pMesh);

			xst_fi	CMaterialManager*	GetMaterialManager()
										{ return m_pMatMgr; }

			xst_fi	
			const	CMaterialManager*	GetMaterialManager() const
										{ return m_pMatMgr; }

			xst_fi	IRenderSystem*		GetRenderSystem()
										{ return m_pRenderSystem; }

			xst_fi	
			const	IRenderSystem*		GetRenderSystem() const
										{ return m_pRenderSystem; }


		protected:
			
			virtual	
			Resources::IResource*		_CreateResource(xst_castring& strName, const ResourceHandle& ResHandle, GroupWeakPtr pGroup) xst_implement;

			xst_fi	void				_SetRenderSystem(IRenderSystem* pRS)
										{ m_pRenderSystem = pRS; }

					void				_OnResDestroy(ResourcePtr pRes);

					void				_OnResRemove(ResourcePtr pRes);

					void				_DestroyMeshBuffers(ResourcePtr pRes);

					void				_OnDestroy() xst_implement;

		protected:

			IRenderSystem*		m_pRenderSystem;
			CMaterialManager*	m_pMatMgr;
			MeshPtr				m_pDefaultMesh;
			IInputLayout*		m_pDefaultIL;
			u16					m_usDefaultLODCount;
	};

}//xse

#endif