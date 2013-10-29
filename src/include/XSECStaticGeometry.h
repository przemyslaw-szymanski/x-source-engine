#ifndef XSE_CSTATIC_GEOMETRY_H
#define XSE_CSTATIC_GEOMETRY_H

#include "XSECMesh.h"
#include "XSECMaterial.h"
#include "XSECStaticGeometry.h"
//#include "XSECStaticGeometryGroup.h"

namespace XSE
{
	class XST_API CStaticGeometry : public IRenderableObject
	{
		friend class CStaticGeometryGroup;

		protected:

			typedef CStaticGeometryGroup	Group;
			typedef xst_vector< Group* >	GroupVector;

		public:

			typedef XST::TCIterator< GroupVector >			GroupIterator;
			typedef XST::TCConstIterator< GroupVector >		ConstGroupIterator;
			//typedef CStaticGeometryGroup::BuildDelegate	BuildDelegate;
			typedef fastdelegate::FastDelegate2< xst_unknown, const CStaticGeometryObject* > BuildDelegate;

		public:

										CStaticGeometry(xst_castring& strName, IRenderSystem* pRS, CSceneManager* pSceneMgr, CMeshManager* pMeshMgr, xst_castring& strGroup );
			virtual						~CStaticGeometry();

					void				Render(IRenderSystem* pRS);

					Group*				CreateGroup(const Vec3& vecPos = Vec3::ZERO);

					void				SetVisible(bool bVisible);
					
					void				DisableObject(cu32& uiReason);

					i32					Build();

					void				Update();

					ul32				GetObjectHandle() const
										{ return 0; }

					xst_castring&		GetObjectName() const
										{ return m_strName; }

					void				AddMesh(Group* pGroup, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos = Vec3::ZERO);
					void				AddMesh(ul32 ulGroupID, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos = Vec3::ZERO);
					void				AddMesh(const GroupIterator& GrItr, const MeshPtr& pMesh, const MaterialPtr& pMat, const Vec3& vecPos = Vec3::ZERO);

					void				DestroyGroups();

			xst_fi	GroupIterator		GetGroupIterator()
										{ return GroupIterator( &m_vGroups ); }

			xst_fi	ConstGroupIterator	GetGroupIterator() const
										{ return ConstGroupIterator( &m_vGroups ); }

					CVertexData*		GetVertexData(ul32 ulGroupId, ul32 ulSubGroupId);

			xst_fi	BuildDelegate&		GetOnBuildDelegate()
										{ return m_OnBuild; }

			xst_fi	bool				IsBuilt() const
										{ return m_bIsBuilt; }

		protected:

			virtual	void				_SetSceneNode(CSceneNode* pNode);

					void				_OnObjectBuild(xst_unknown pUserData, const CStaticGeometryObject* pObj) {}

		protected:

			GroupVector		m_vGroups;
			xst_astring		m_strName;
			xst_castring&	m_cstrGroupName;
			CSceneManager*	m_pSceneMgr;
			IRenderSystem*	m_pRenderSystem;
			CMeshManager*	m_pMeshMgr;
			BuildDelegate	m_OnBuild;
			bool			m_bIsBuilt;
	};

	typedef XST::TCObjectSmartPointer< CStaticGeometry >	StaticGeometryPtr;

}//xse

#endif