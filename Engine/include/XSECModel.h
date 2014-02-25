#ifndef XSE_CMODEL_H
#define XSE_CMODEL_H

#include "XSEIRenderableObject.h"
#include "XSECMesh.h"

namespace XSE
{

	namespace Resources
	{
		class XST_API CModel : public IRenderableObject, public IResource
		{
			friend class CModelManager;
			friend class CSceneNode;
			friend class CSceneManager;

			typedef i32 (CModel::*pfnRender)();

			public:

				//typedef xst_vector< SMeshLOD >		LODVec;
				typedef xst_vector< MeshPtr >		MeshVec;
				typedef MeshVec::iterator			MeshVecItr;
				typedef MeshVec::const_iterator		MeshVecConstItr;

			public:

										CModel(IRenderSystem* pRS, IInputLayout* pIL, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual					~CModel();

						void			Render(IRenderSystem* pRS) xst_implement;

						void			SetVisible(bool bVisible) xst_implement;

						void			Disable(cu32& uiReason) xst_implement;

                xst_fi  Handle          GetObjectHandle() const xst_implement
                                        { return this->GetResourceHandle(); }

						void			SetMesh(MeshPtr pMesh, cu32& uiId = 0);

						void			SetMaterial(MaterialPtr pMat, cu32& uiId = 0);

				/*xst_fi	xst_castring&	GetObjectName() const
										{ return this->_GetDbgName(); }*/

				virtual void			SetPosition(cf32& fX, cf32& fY, cf32& fZ) xst_implement;

				const CBoundingVolume&	CalcObjectBoundingVolume();

						void			SetLOD(u8 byLodId);

				xst_fi	void			SetMinLOD(u8 byLodId)
										{ m_byMinLod = byLodId; }

				xst_fi	void			SetMaxLOD(u8 byLodId)
										{ m_byMaxLod = byLodId; }

				xst_fi	u8				GetMaxLOD() const
										{ return m_byMaxLod; }

				xst_fi	u8				GetMinLOD() const
										{ return m_byMinLod; }

				xst_fi	u8				GetCurrentLODId()
										{ return m_byCurrLod; }

				xst_fi	u8				GetCurrentLODId() const
										{ return m_byCurrLod; }

				xst_fi	u32				GetMeshCount() const
										{ return m_vMeshes.size(); }

				xst_fi	MeshPtr			GetMesh(cu32& uiId = 0)
										{ return m_vMeshes[ uiId ]; }

						u32				AddMesh(MeshPtr& pMesh, bool bSetModelMaterial = false);

				xst_fi u32				GetMeshCount()
										{ return m_vMeshes.size(); }

				xst_fi	
				const	MeshPtr			GetMesh(cu32& uiId = 0) const
										{ return m_vMeshes[ uiId ]; }

				xst_fi	MeshVec&		GetMeshes()
										{ return m_vMeshes; }

				xst_fi
				const	MeshVec&		GetMeshes() const
										{ return m_vMeshes; }

			protected:

						void			_SetSceneNode(CSceneNode* pNode);

						i32				_Render();

						i32				_RenderEmpty()
										{ return XST_OK; }

			protected:

				MeshVec			m_vMeshes;
				MeshVecItr		m_MeshItr;
				IRenderSystem*	m_pRS;
				pfnRender		m_RenderMethod;
				u8				m_byCurrLod;
				u8				m_byMinLod;
				u8				m_byMaxLod;
		};

		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< CModel >;

	}//resources

	typedef XST::TCObjectSmartPointer< Resources::CModel >	ModelPtr;

}//xse

#endif