#include "XSECOBJLoader.h"
#include "XSECModel.h"
#include "XSECMeshManager.h"

namespace XSE
{
	COBJLoader::COBJLoader()
	{
	}

	COBJLoader::~COBJLoader()
	{
	}

	i32 COBJLoader::LoadModel(Resources::CModel** ppOut, cu8* pData, cul32 uDataSize)
	{
		char* strData = (char*)pData;
		
		xst_vector< Vec3 > vVertices, vNormals;
		xst_vector< Vec2 > vUVs;
		xst_vector< u32 > vVertexIds, vNormalIds, vUVIds;

		cul32 uReserve = 3000;
		vVertices.reserve( uReserve );
		vNormals.reserve( uReserve );
		vUVs.reserve( uReserve );
		vVertexIds.reserve( uReserve );
		vNormalIds.reserve( uReserve );
		vUVIds.reserve( uReserve );

		char* strLine = strtok( strData, "\n" );
		while( strLine )
		{
			printf( "%s\n", strLine );
			if( strLine[ 0 ] == 'v' && strLine[1] == ' ' )
			{
				Vec3 vecData;
				sscanf( strLine, "v %f %f %f", &vecData.x, &vecData.y, &vecData.z );
				vVertices.push_back( vecData );
			}
			else if( strLine[ 0 ] == 'v' && strLine[ 1 ] == 'n' )
			{
				Vec3 vecData;
				sscanf( strLine, "vn %f %f %f", &vecData.x, &vecData.y, &vecData.z );
				vNormals.push_back( vecData );
			}
			else if( strLine[ 0 ] == 'f' && strLine[ 1 ] == ' ' )
			{
				u32 auVIds[3]; // vertex indices
				u32 auTIds[3]; // texcoord indices
				u32 auNIds[3]; // normal indices
				sscanf( strLine, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
						&auVIds[0], &auTIds[0], &auNIds[0],
						&auVIds[1], &auTIds[1], &auNIds[1],
						&auVIds[2],&auTIds[2], &auNIds[2]);
				vVertexIds.push_back( auVIds[0]-1 );
				vVertexIds.push_back( auVIds[1]-1 );
				vVertexIds.push_back( auVIds[2]-1 );
				vNormalIds.push_back( auNIds[0]-1 );
				vNormalIds.push_back( auNIds[1]-1 );
				vNormalIds.push_back( auNIds[2]-1 );
				vUVIds.push_back( auTIds[0]-1 );
				vUVIds.push_back( auTIds[0]-1 );
				vUVIds.push_back( auTIds[0]-1 );
			}
			strLine = strtok( NULL, "\n" );
		}
		//http://www.braynzarsoft.net/index.php?p=D3D11OBJMODEL
		Resources::CModel* pModel = *ppOut;
		CMeshManager* pMeshMgr = CMeshManager::GetSingletonPtr();
		u32 uIL = ILEs::POSITION;
		if( !vNormals.empty() )
			uIL |= ILEs::NORMAL;
		if( !vUVs.empty() )
			uIL |= ILEs::TEXCOORD0;
		MeshWeakPtr pMesh = pMeshMgr->CreateMesh( pModel->GetResourceName() + "/mesh", pModel->GetResourceGroupHandle() );
		if( pMesh.IsNull() )
			return XST_FAIL;
		pMesh->SetInputLayout( uIL );

		IVertexBuffer* pVB = pMesh->CreateVertexBuffer().GetPtr();
		IIndexBuffer* pIB = pMesh->CreateIndexBuffer().GetPtr();

		pVB->SetTopologyType( TopologyTypes::TRIANGLE_LIST );
		pVB->SetUsage( BufferUsages::STATIC );
		pVB->SetVertexCount( vVertices.size() );

		if( XST_FAILED( pVB->Lock() ) )
		{
			return XST_FAIL;
		}

		CVertexData& VData = pVB->GetVertexData();
		for( ul32 v = 0; v < vVertices.size(); ++v )
		{
			VData.SetPosition( v, vVertices[ v ] );
		}

		if( XST_FAILED( pVB->Unlock() ) )
		{
			return XST_FAIL;
		}

		pIB->SetIndexCount( vVertexIds.size() );
		pIB->SetUsage( BufferUsages::STATIC );
		XST_RET_FAIL( pIB->Lock() );
		CIndexData& IData = pIB->GetIndexData();
		for( ul32 i = 0; i < vVertexIds.size(); ++i )
		{
			IData.SetIndex( i, vVertexIds[ i ] );
		}
		XST_RET_FAIL( pIB->Unlock() );

		pModel->AddMesh( pMesh, true );

		return XST_OK;
	}
} // XSE