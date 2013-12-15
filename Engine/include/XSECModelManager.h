#ifndef XSE_CMODEL_MANAGER_H
#define XSE_CMODEL_MANAGER_H

#include "XSEIResourceManager.h"
#include "XSECModel.h"

namespace XSE
{
	class XST_API CModelManager : public XST::TCSingleton< CModelManager >, public IResourceManager
	{
		friend class CEngine;
		public:

										CModelManager(CMeshManager* pMeshMgr);
										~CModelManager();

				i32						PrepareResource(ResourcePtr pRes);

				ModelPtr				CreateModel(xst_castring& strName, xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(xst_castring& strName, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions = xst_null, xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strNodeName,  xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strNodeName, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions = xst_null, xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(xst_castring& strName, CSceneNode* pNode, BASIC_SHAPE eShape, IInputLayout* pLayout, xst_unknown pOptions = xst_null, xst_castring& strGroup = DEFAULT_GROUP);
				ModelPtr				CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strModelGroup, xst_castring& strMeshGroup);
				ModelPtr				CreateModel(CSceneManager* pSceneMgr, xst_castring& strName, xst_castring& strMeshName, xst_castring& strNodeName, xst_castring& strModelGroup, xst_castring& strMeshGroup);

				xst_fi i32				DestroyModel(xst_castring& strModelName, bool bDestroySceneNodeIfEmpty = true, xst_castring& strGroupName = ALL_GROUPS)
										{ return DestroyModel( (ModelPtr)GetResource( strModelName, strGroupName ), bDestroySceneNodeIfEmpty, strGroupName ); }

				i32						DestroyModel(ModelPtr& pModel, bool bDestroySceneNodeIfEmpty = true, xst_castring& strGroupName = ALL_GROUPS);

			xst_fi	CMeshManager*		GetMeshManager()
										{ return m_pMeshMgr; }

			xst_fi	
			const	CMeshManager*		GetMeshManager() const
										{ return m_pMeshMgr; }

			xst_fi	IRenderSystem*		GetRenderSystem()
										{ return m_pRenderSystem; }

			xst_fi	
			const IRenderSystem*		GetRenderSystem() const
										{ return m_pRenderSystem; }

					ResourcePtr			CloneResource(const Resources::IResource* pSrcRes, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true);

			xst_fi	ModelPtr			CloneModel(const ModelPtr& pSrcModel, xst_castring& strNewName = XST::StringUtil::EmptyAString, bool bFullClone = true)
										{ return CloneResource( pSrcModel.GetPointer(), strNewName, bFullClone ); }

		protected:

				Resources::IResource*	_CreateResource(xst_castring& strName, cul32& ulHandle, GroupPtr pGroup);

			xst_fi	void				_SetRenderSystem(IRenderSystem* pRS)
										{ m_pRenderSystem = pRS; }

				i32						_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);


		protected:

			IRenderSystem*	m_pRenderSystem;
			CMeshManager*	m_pMeshMgr;
	};
};

#endif