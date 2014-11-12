#ifndef XSE_CSCENE_MANAGER_H
#define XSE_CSCENE_MANAGER_H

#include "XSECommon.h"
#include "XSECRenderQueue.h"
#include "XSECDynamicGeometry.h"
#include "XSECModel.h"
#include "XSEITerrain.h"
#include "XSEITerrainSystem.h"

namespace XSE
{
	class CSceneDebug;

	class XST_API CSceneManager
	{
		friend class CSceneNode;
		friend class IScenePartitionSystem;
		friend class IRenderWindow;
		friend class CEngine;
		friend class CSceneDebug;

		static xst_astring DEFAULT_SHADER_COLOR;

	public:

        struct SObjectInfo
        {
            CObject*                pObj = xst_null;
            OBJECT_DISABLE_REASON   eDisableReason = ODR::OTHER;
            bool                    bVisible = false;
        };

		//typedef xst_map< ul32, CCamera* >		CameraMap;
		xst_tmapx( ul32, CCamera* )				CameraMap;
		typedef XST::TCIterator< CameraMap >	CameraIterator;

		static xst_castring						DEFAULT_CAMERA;

		typedef CStaticGeometry*				StaticGeometryPtr;
		typedef xst_map< xst_astring, StaticGeometryPtr >	StaticGeometryMap;
		//typedef CDynamicGeometry*				DynamicGeometryPtr;
		typedef xst_map< xst_astring, DynamicGeometryPtr >	DynamicGeometryMap;
		typedef xst_vector< ITerrain* >			TerrainVec;
        typedef xst_vector< SObjectInfo >       ObjectInfoVec;

	protected:

		typedef CameraMap::iterator				_CameraIterator;

	public:

						        CSceneManager(xst_castring& strName, CModelManager* pModelMgr, f32 fSize = 0);
		        virtual			~CSceneManager();

		        i32				Init();

		        void			Update(cf32& fFrameTime);

		        CSceneNode*		CreateNode(xst_castring& strName, bool bAutoDestroy = true);

		        CCamera*		CreateCamera(xst_castring& strName, CAMERA_TYPE eType = CameraTypes::FREE);

		        ModelPtr		CreateModel(xst_castring& strName);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strNodeName);
		        ModelPtr		CreateModel(xst_castring& strName, CSceneNode* pNode);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strMeshGroup);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strNodeName, xst_castring& strMeshName, xst_castring& strMeshGroup);
		        ModelPtr		CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strMeshName, xst_castring& strMeshGroup);
		        ModelPtr		CreateModel(xst_castring& strName, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strNodeName, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, CSceneNode* pNode, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strNodeName, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, CSceneNode* pNode, xst_castring& strMeshName, xst_castring& strMeshGroup, BASIC_SHAPE eShape, IInputLayout* pIL, xst_unknown pShapeOptions);
		        ModelPtr		CreateModel(xst_castring& strName, xst_castring& strNodeName, MeshPtr& pMesh);

		        i32				DestroyCamera(CCamera* pCamera);

		        i32				RemoveCamera(CCamera* pCamera);

		        void			RemoveCameras();

		        void			DestroyCameras();

		        void			SetViewFrustumCullType(const VIEW_FRUSTUM_CULL_TEST_TYPE& eType);

		VIEW_FRUSTUM_CULL_TEST_TYPE	GetViewFrustumCullType() const;

		        i32 		    AddCamera(CCamera* pCamera);

		        i32		        SetCamera(CCamera* pCamera, bool bCompute = true, bool bView = true);

		xst_fi	CCamera*	    GetCurrentCamera() const
		                        { return m_pViewCamera; }

		xst_fi	CCamera*	    GetComputeCamera() const
		                        { return m_pComputeCamera; }

		        CCamera*		GetCamera(xst_castring& strName = DEFAULT_CAMERA);
		        CCamera*		GetCamera(cul32& ulHandle);

		        CameraIterator	GetCameraIterator();

		        CameraMap&		GetCameras();

		        IRenderSystem*	GetRenderSystem();

		const	IRenderSystem*	GetRenderSystem() const;

		xst_fi	CModelManager*	GetModelManager()
		                        { return m_pModelMgr; }

		xst_fi	
		const	CModelManager*	GetModelManager() const
		                        { return m_pModelMgr; }

		        CMeshManager*	GetMeshManager();
		const	CMeshManager*	GetMeshManager() const;

		    CMaterialManager*	GetMaterialManager();
		const	
            CMaterialManager*	GetMaterialManager() const;


		xst_fi	CSceneNode*		GetRootNode()
		                        { return m_pRootNode; }

		        i32				DestroyNode(xst_castring& strName, bool bSearch = false);
		        i32				DestroyNode(CSceneNode* pNode, bool bSearch = false);
		        i32				DestroyNode(cul32& ulHandle, bool bSearch = false);

		xst_fi	xst_castring&	GetName() const
		                        { return m_strName; }

		StaticGeometryPtr		CreateStaticGeometry(xst_castring& strName, xst_castring& strGroupName);
		StaticGeometryPtr		CreateStaticGeometry(xst_castring& strName)
		                        { return CreateStaticGeometry( strName, this->GetName() ); }

		DynamicGeometryPtr		CreateDynamicGeometry(xst_castring& strName, xst_castring& strGroupName);
		DynamicGeometryPtr		CreateDynamicGeometry(xst_castring& strName)
		                        { return CreateDynamicGeometry( strName, this->GetName() ); }

		        void			DestroyStaticGeometry(xst_castring& strName);
		        void			DestroyStaticGeometry(CStaticGeometry* pGeometry);
		        void			DestroyStaticGeometries();

		        void			DestroyDynamicGeometry(xst_castring& strName);
		        void			DestroyDynamicGeometry(CDynamicGeometry* pGeometry);
		        void			DestroyDynamicGeometries();

		xst_fi	void			AddToRenderQueue(RENDER_QUEUE_TYPE eType, const RenderableObjectPtr& pObj)
		                        { AddToRenderQueue( eType, pObj.GetPtr() ); }
		        void			AddToRenderQueue(RENDER_QUEUE_TYPE eType, IRenderableObject* pObj);

				void			AddToPartitionSystem(CObject* pObj);

		        i32				SetTerrainSystem(ITerrainSystem* pSystem);

		        void			DestroyTerrain(xst_castring& strName);

		        ITerrain*		CreateTerrain(xst_castring& strName, STerrainOptions& Options);

		xst_fi
		IScenePartitionSystem*	GetScenePartitionSystem() const
		                        { return m_pScenePartitionSystem; }

		        void			SetScenePartitionSystem(IScenePartitionSystem* pSystem);

		xst_fi	f32				GetSize() const
		                        { return m_fSize; }

		xst_fi	
		const	CSceneDebug*	Dbg() const
		                        { return m_pDbg; }

	protected:

		        void			_AddObject(IRenderableObject* pObj);

		xst_fi	void			_AddObject(const RenderableObjectPtr& pObj)
		                        { _AddObject( pObj.GetPtr() ); }


	protected:

		xst_astring			    m_strName;
		CameraMap			    m_mCameras;
        StaticGeometryMap	    m_mStaticGeometries;
		DynamicGeometryMap	    m_mDynamicGeometries;
		TerrainVec			    m_vTerrains;
		ITerrainSystem*		    m_pTerrainSystem;
		IScenePartitionSystem*	m_pScenePartitionSystem;
		CSceneNode*			    m_pRootNode;
		CCamera*			    m_pViewCamera; //this camera is using for create view/proj matrix
		CCamera*			    m_pComputeCamera; //this camera is using in e.g octrees
		CRenderQueue		    m_RenderQueue;
		CModelManager*		    m_pModelMgr;
		f32					    m_fSize;
		CSceneDebug*		    m_pDbg;
	};

	XST_TEMPLATE_CLASS XST::TCIterator< CSceneManager::CameraMap >;

}//xse

#endif //XSE_CSCENE_MANAGER_H