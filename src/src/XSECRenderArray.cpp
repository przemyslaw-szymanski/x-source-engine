#include "XSECRenderArray.h"
#include "XSEIVertexShader.h"
#include "XSECMaterial.h"
#include "XSEIRenderableObject.h"
#include "XSECMesh.h"
#include "XSECModel.h"

namespace XSE
{
	i32 CRenderMaterialArray::AddMaterial(CMaterial* pMat)
	{
		return XST::MapUtils::Insert< ObjectArrayMap, CMaterial*, ChildArray* >( m_mObjects, pMat, xst_null );
	}

	i32 CRenderMaterialArray::AddObject(IRenderableObject* pObj)
	{
		xst_assert( pObj, "" );
		xst_assert( !pObj->GetMaterial().IsNull(), "" );
		ObjectArrayMap::iterator Itr = m_mObjects.find( pObj->GetMaterial().GetPointer() );
		
		if( Itr == m_mObjects.end() )
		{
			if( XST_FAILED( AddMaterial( pObj->GetMaterial().GetPointer() ) ) )
			{
				return XST_FAIL;
			}
		}

		Itr = m_mObjects.find( pObj->GetMaterial().GetPointer() );
		return Itr->second->AddObject( pObj );
	}

	CRenderArray::CRenderArray()
	{
	}

	CRenderArray::~CRenderArray()
	{
	}

	i32 CRenderArray::AddInputLayout(const XSE::IInputLayout *pIL)
	{
		return XST::MapUtils::Insert< ILArrayMap, const IInputLayout*, ChildArray* >( m_mInputLayouts, pIL, xst_null );
	}

	i32 CRenderArray::AddMaterial(CMaterial* pMat)
	{
		VertexShaderPtr pVS = pMat->GetVertexShader();
		if( pVS.IsNull() )
		{
			XST_LOG_ERR( "VertexShader does not exists in material: " << pMat->GetResourceName() );
			return XST_FAIL;
		}
		const IInputLayout* pIL = pVS->GetInputLayout();
		ILArrayMap::iterator Itr = m_mInputLayouts.find( pIL );
		if( Itr == m_mInputLayouts.end() )
		{
			/*if( XST_FAILED( AddInputLayout( pIL ) ) )
			{
				XST_LOG_ERR( "Failed to add the input layout for material: " << pMat->GetResourceName() );
				return XST_FAIL;
			}

			Itr = m_mInputLayouts.find( pIL );*/
			return XST_FAIL;
		}

		ChildArray* pArr = Itr->second;
		if( pArr == xst_null )
		{
			pArr = xst_new ChildArray();
		}

		return pArr->AddMaterial( pMat );
	}

	i32 CRenderArray::AddObject(IRenderableObject* pObj)
	{
		MaterialPtr pMat = pObj->GetMaterial();
		xst_assert( !pMat.IsNull(), "(CRenderArray::AddObject) Material is not set" );
		CRenderMaterialArray* pMatArr = GetMaterialArray( pMat.GetPointer() );
		return pMatArr->AddObject( pObj );
	}

	CRenderMaterialArray* CRenderArray::GetMaterialArray(CMaterial* pMat)
	{
		xst_assert( pMat, "" );
		xst_assert( !pMat->GetVertexShader().IsNull(), "" );
		xst_assert( pMat->GetVertexShader()->GetInputLayout(), "" );

		CRenderMaterialArray* pArr = xst_null;
		ILArrayMap::iterator Itr = m_mInputLayouts.find( pMat->GetVertexShader()->GetInputLayout() );
		if( Itr == m_mInputLayouts.end() )
		{
			/*if( XST_FAILED( this->AddInputLayout( pMat->GetVertexShader()->GetInputLayout() ) ) )
			{
				return XST_FAIL;
			}*/

			if( XST_FAILED( this->AddMaterial( pMat ) ) )
			{
				return xst_null;
			}

			Itr = m_mInputLayouts.find( pMat->GetVertexShader()->GetInputLayout() );
			pArr = Itr->second;
		}
		else
		{
			pArr = Itr->second;
		}

		return pArr;
	}

	///////////////////////////////////////////////////////

	CRenderTree::~CRenderTree()
	{
		//For each input layout
		for(ILVSMap::iterator Itr1 = m_mTree.begin(); Itr1 != m_mTree.end(); ++Itr1)
		{
			for(VSPSMap::iterator Itr2 = Itr1->second->begin(); Itr2 != Itr1->second->end(); ++Itr2)
			{
				for(PSTexMap::iterator Itr3 = Itr2->second->begin(); Itr3 != Itr2->second->end(); ++Itr3)
				{
					for(TexObjMap::iterator Itr4 = Itr3->second->begin(); Itr4 != Itr3->second->end(); ++Itr4)
					{
						//Delete vector
						xst_delete( Itr4->second );
					}

					//Delete map
					xst_delete( Itr3->second );
				}

				//Delete map
				xst_delete( Itr2->second );
			}

			//Delete map
			xst_delete( Itr1->second );
		}

		m_mTree.clear();
	}

	i32 CRenderTree::Add(XSE::CRenderTree::ObjPtr pObj)
	{
		//MaterialPtr pMat = pObj->GetMaterial();
		//xst_assert( !pMat.IsNull(), "(CRenderTree::Add)" );
		//VertexShaderPtr pVS = pMat->GetVertexShader();
		//PixelShaderPtr pPS = pMat->GetPixelShader();
		//xst_assert( !pVS.IsNull(), "(CRenderTree::Add)" );
		//xst_assert( !pPS.IsNull(), "(CRenderTree::Add)" );
		//const IInputLayout* pIL = pVS->GetInputLayout();
		//xst_assert( pIL, "(CRenderTree::Add)" );

		////Get or create input layout map
		//ObjVec* pVec = GetOrCreateObjVec( pVS.GetPointer(), pPS.GetPointer(), xst_null );
		//xst_assert( pVec, "(CRenderTree::Add)" );
		//pVec->push_back( pObj );

		m_vObjs.push_back( pObj );

		return XST_OK;
	}

	i32 CRenderTree::Add(CModel* pModel)
	{
		xst_assert( pModel != xst_null, "(CRenderTree::Add) Null pointer" );
		//MeshPtr pMesh = pModel->GetMesh();
		//if( pMesh.IsNull() )
		//{
		//	XST_LOG_ERR( "Model has no mesh" );
		//	return XST_FAIL;
		//}

		////Add main mesh
		//if( XST_FAILED( Add( pMesh.GetPointer() ) ) )
		//{
		//	return XST_FAIL;
		//}

		////Add submeshes
		//CMesh::MeshIterator Itr = pMesh->GetSubMeshIterator();
		//while( Itr.HasMoreElements() )
		//{
		//	if( XST_FAILED( Add( Itr.GetValue().GetPointer() ) ) )
		//	{
		//		return XST_FAIL;
		//	}
		//	Itr.MoveNext();
		//}

		CModel::MeshVecItr Itr;
		xst_stl_foreach( Itr, pModel->GetMeshes() )
		{
			if( XST_FAILED( Add( (*Itr).GetPointer() ) ) )
			{
				return XST_FAIL;
			}
		}

		return XST_OK;
	}

	i32 CRenderTree::Remove(XSE::CRenderTree::ObjPtr pObj)
	{
		MaterialPtr pMat = pObj->GetMaterial();
		xst_assert( !pMat.IsNull(), "" );
		VertexShaderPtr pVS = pMat->GetVertexShader();
		PixelShaderPtr pPS = pMat->GetPixelShader();
		xst_assert( !pVS.IsNull(), "" );
		xst_assert( !pPS.IsNull(), "" );

		const IInputLayout* pIL = pVS->GetInputLayout();
		xst_assert( pIL, "" );

		ObjVec* pVec = GetObjVec( pVS.GetPointer(), pPS.GetPointer(), pMat->GetDiffuseTexture().GetPointer() );
		xst_assert( pVec, "" );
		ObjVec::iterator Itr = std::remove( pVec->begin(), pVec->end(), pObj );
		if( Itr == pVec->end() )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	CRenderTree::VSPSMap*	CRenderTree::GetOrCreateVSPSMap(ILPtr pIL)
	{
		ILVSMap::iterator Itr;
		XST::MapUtils::FindPlace( m_mTree, pIL, &Itr );
		if( XST::MapUtils::IsKeyExists( m_mTree, pIL, Itr ) )
		{
			return Itr->second;
		}
		else
		{
			VSPSMap* pMap = xst_new VSPSMap();
			XST::MapUtils::InsertOnPlace( m_mTree, pIL, pMap, Itr );
			return pMap;
		}

		return xst_null;
	}

	CRenderTree::PSTexMap*	CRenderTree::GetOrCreatePSTexMap(ILPtr pIL, VSPtr pVS)
	{
		VSPSMap* pVSPSMap = GetOrCreateVSPSMap( pIL );
		xst_assert( pVSPSMap, "" );

		VSPSMap& VPMap = *pVSPSMap;
		VSPSMap::iterator Itr;
		XST::MapUtils::FindPlace( VPMap, pVS, &Itr );
		if( XST::MapUtils::IsKeyExists( VPMap, pVS, Itr ) )
		{
			return Itr->second;
		}
		else
		{
			PSTexMap* pMap = xst_new PSTexMap();
			XST::MapUtils::InsertOnPlace( VPMap, pVS, pMap, Itr );
			return pMap;
		}

		return xst_null;
	}

	CRenderTree::TexObjMap*	CRenderTree::GetOrCreateTexObjMap(VSPtr pVS, PSPtr pPS)
	{
		xst_assert( pVS->GetInputLayout(), "" );
		PSTexMap* pPSTexMap = GetOrCreatePSTexMap( (IInputLayout*)pVS->GetInputLayout(), pVS );
		xst_assert( pPSTexMap, "" );

		PSTexMap& PTMap = *pPSTexMap;
		PSTexMap::iterator Itr;
		XST::MapUtils::FindPlace( PTMap, pPS, &Itr );
		if( XST::MapUtils::IsKeyExists( PTMap, pPS, Itr ) )
		{
			return Itr->second;
		}
		else
		{
			TexObjMap* pMap = xst_new TexObjMap();
			XST::MapUtils::InsertOnPlace( PTMap, pPS, pMap, Itr );
			return pMap;
		}

		return xst_null;
	}

	CRenderTree::ObjVec* CRenderTree::GetOrCreateObjVec(VSPtr pVS, PSPtr pPS, TexPtr pTex)
	{
		TexObjMap* pTexObjMap = GetOrCreateTexObjMap( pVS, pPS );
		xst_assert( pTexObjMap, "" );

		TexObjMap& TOMap = *pTexObjMap;
		TexObjMap::iterator Itr;
		XST::MapUtils::FindPlace( TOMap, pTex, &Itr );
		if( XST::MapUtils::IsKeyExists( TOMap, pTex, Itr ) )
		{
			return Itr->second;
		}
		else
		{
			ObjVec* pMap = xst_new ObjVec();
			XST::MapUtils::InsertOnPlace( TOMap, pTex, pMap, Itr );
			return pMap;
		}
		return xst_null;
	}

	CRenderTree::ILVSMap*	CRenderTree::GetILVSMap()
	{
		return &m_mTree;
	}

	CRenderTree::VSPSMap*	CRenderTree::GetVSPSMap(ILPtr pIL)
	{
		ILVSMap::iterator Itr;
		XST::MapUtils::FindPlace( m_mTree, pIL, &Itr );
		if( XST::MapUtils::IsKeyExists( m_mTree, pIL, Itr ) )
		{
			return Itr->second;
		}

		return xst_null;
	}

	CRenderTree::PSTexMap*	CRenderTree::GetPSTexMap(ILPtr pIL, VSPtr pVS)
	{
		VSPSMap* pVSPSMap = GetOrCreateVSPSMap( pIL );
		xst_assert( pVSPSMap, "" );

		VSPSMap& VPMap = *pVSPSMap;
		VSPSMap::iterator Itr;
		XST::MapUtils::FindPlace( VPMap, pVS, &Itr );
		if( XST::MapUtils::IsKeyExists( VPMap, pVS, Itr ) )
		{
			return Itr->second;
		}
		
		return xst_null;
	}

	CRenderTree::TexObjMap*	CRenderTree::GetTexObjMap(VSPtr pVS, PSPtr pPS)
	{
		xst_assert( pVS->GetInputLayout(), "" );
		PSTexMap* pPSTexMap = GetOrCreatePSTexMap( (IInputLayout*)pVS->GetInputLayout(), pVS );
		xst_assert( pPSTexMap, "" );

		PSTexMap& PTMap = *pPSTexMap;
		PSTexMap::iterator Itr;
		XST::MapUtils::FindPlace( PTMap, pPS, &Itr );
		if( XST::MapUtils::IsKeyExists( PTMap, pPS, Itr ) )
		{
			return Itr->second;
		}
		
		return xst_null;
	}

	CRenderTree::ObjVec*		CRenderTree::GetObjVec(VSPtr pVS, PSPtr pPS, TexPtr pTex)
	{
		TexObjMap* pTexObjMap = GetOrCreateTexObjMap( pVS, pPS );
		xst_assert( pTexObjMap, "" );

		TexObjMap& TOMap = *pTexObjMap;
		TexObjMap::iterator Itr;
		XST::MapUtils::FindPlace( TOMap, pTex, &Itr );
		if( XST::MapUtils::IsKeyExists( TOMap, pTex, Itr ) )
		{
			return Itr->second;
		}

		return xst_null;
	}

}//xse