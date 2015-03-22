#ifndef XSE_CMATERIAL_MANAGER_H
#define XSE_CMATERIAL_MANAGER_H

#include "XSEIResourceManager.h"
#include "XSECMaterial.h"

namespace XSE
{
	class XST_API CMaterialManager : public IResourceManager, public XST::TCSingleton< CMaterialManager >
	{
		friend class CEngine;
		protected:

			typedef xst_map< const IInputLayout*, MaterialPtr >	DefaultMatMap;

		public:

			static xst_castring		DEFAULT_MAT_COLOR;

		public:

									CMaterialManager(CShaderManager* pShaderMgr);
			virtual					~CMaterialManager();

            virtual i32				Init() xst_implement;

					MaterialPtr		CreateMaterial(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);

					i32				PrepareMaterials(xst_castring& strGroup);

					i32				PrepareResource(ResourceWeakPtr *const ppRes) xst_implement;

					MaterialPtr		LoadMaterial(xst_castring& strName, xst_castring& strGroupName = DEFAULT_GROUP);

					i32				SaveMaterial(xst_castring& strFileName, MaterialPtr pMat);

			xst_fi	MaterialPtr		GetMaterial(xst_castring& strName, xst_castring& strGroupName = ALL_GROUPS)
									{ return this->GetResource( strName, strGroupName ); }

			xst_fi	XST::CLuaApi&	GetLuaApi()
									{ return m_LuaApi; }

			xst_fi	MaterialPtr		GetDefaultMaterial()
									{ return m_pDefaultMat; }	

					MaterialPtr		GetDefaultMaterial(const Resources::CMesh* pMesh);
					MaterialPtr		GetDefaultMaterial(const IInputLayout* pIL);

			xst_fi	CShaderManager*	GetShaderManager() const
									{ return m_pShaderMgr; }

		protected:

			virtual	Resources::IResource*	_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup) xst_implement;
			//virtual	i32				_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator = xst_null);

		protected:

			XST::CLuaApi			m_LuaApi;
			XSE::CShaderManager*	m_pShaderMgr;
			MaterialPtr				m_pDefaultMat;
			DefaultMatMap			m_mDefaultMaterials;
	};

}//xse

#endif