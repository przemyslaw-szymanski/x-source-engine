#include "XSECMaterialManager.h"
#include "XSECLuaScriptManager.h"
#include "XSECLuaMaterialReader.h"
#include "XSECBinaryMaterialReader.h"
#include "XSECLuaMaterialParser.h"
#include "XSECShaderManager.h"
#include "XSEITechnique.h"
#include "XSEIPass.h"
#include "XSECMesh.h"
#include "XSECmaterial.h"
#include "XSEIRenderSystem.h"

namespace XSE
{
	lpcastr XSE_LUA_MAT_API	=
	"function Vector2(fX, fY) return { x = fX, y = fY } end " 
	"function Vector3(fX, fY, fZ) return { x = fX, y = fY, z = fZ } end " 
	"function Vector4(fX, fY, fZ, fW) return { x = fX, y = fY, z = fZ, w = fW } end "
	"function Color3(fR, fG, fB) return { r = fR, g = fG, b = fB } end " 
	"function Color4(fR, fG, fB, fA) return { r = fR, g = fG, b = fB, a = fA } end " 
	"materials = {}";

	xst_castring CMaterialManager::DEFAULT_MAT_COLOR( "xse_default_color" );

	using namespace Resources;

	CMaterialManager::CMaterialManager(CShaderManager* pShaderMgr)
	{
		m_LuaApi.OpenLibs();
		if( m_LuaApi.DoString( XSE_LUA_MAT_API ) != RESULT::OK )
		{
			XST_LOG_ERR( m_LuaApi.GetLastError() );
		}

		m_pShaderMgr = pShaderMgr;
	}

	CMaterialManager::~CMaterialManager()
	{
        int a = m_pDefaultMat->GetRefCount();
	}

	i32 CMaterialManager::Init()
	{
		//Create default material
		Resources::CMaterial* pMat = xst_new Resources::CMaterial( m_pShaderMgr, this, 0, "xse_default_material", XST::ResourceType::MATERIAL, XST::ResourceStates::CREATED, xst_null );
		if( pMat == xst_null )
		{
			return XST_FAIL;
		}

		IPass* pPass = pMat->GetCurrentTechnique()->GetPass( 0 );
		pPass->SetVertexShader( m_pShaderMgr->GetDefaultVertexShader( ) );
		pPass->SetPixelShader( m_pShaderMgr->GetDefaultPixelShader() );

		m_pDefaultMat = MaterialPtr( pMat );

		//Create default simple use materials
		xst_astring strShaderCode;
		strShaderCode.reserve( 2048 );

		{
			MaterialPtr pMat = CreateMaterial( DEFAULT_MAT_COLOR, DEFAULT_GROUP );
            // TODO: implement case if pMat is null if( !pMat ) this resource should be destroyed
			IPass* pPass = pMat->GetCurrentTechnique()->GetPass( 0 );
			strShaderCode = m_pShaderMgr->CreateShaderCode( ILEs::POSITION | ILEs::COLOR, ILEs::POSITION | ILEs::COLOR );	
			VertexShaderPtr pVS = m_pShaderMgr->CompileVertexShader( "xse_default_color_vs", "vs_main", strShaderCode );
			PixelShaderPtr pPS = m_pShaderMgr->CompilePixelShader( "xse_default_color_ps", "ps_main", strShaderCode );

			pPass->SetVertexShader( pVS );
			pPass->SetPixelShader( pPS );
		}

		return XST_OK;
	}

	i32 CMaterialManager::_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator)
	{
		xst_delete( this->m_pMemoryMgr );

		if( pAllocator )
		{	
			this->m_pMemoryMgr = pAllocator;
			this->m_pMemoryMgr->AllocatePool( sizeof( Resources::CMaterial ), ulObjCount );
		}
		else
		{
			this->m_pMemoryMgr = xst_new XST::TCFreeListMemoryManager< Resources::CMaterial >( ulObjCount );
		}

		if( this->m_pMemoryMgr == xst_null )
		{
			XST_LOG_ERR( "Create memory pool failed in material manager." );
			return RESULT::FAILED;
		}

		return RESULT::OK;
	}

	MaterialPtr CMaterialManager::CreateMaterial(xst_castring &strName, xst_castring &strGroupName)
	{
		MaterialPtr pMat = this->CreateResource( strName, strGroupName );
		if( pMat.IsNull() )
		{
			return MaterialPtr();
		}

		return pMat;
	}

	Resources::IResource* CMaterialManager::_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup)
	{
		Resources::CMaterial* pMat = xst_new Resources::CMaterial( m_pShaderMgr, this, ulHandle, strName, XST::ResourceType::MATERIAL, XST::ResourceStates::CREATED, this->m_pMemoryMgr );
		if( pMat == xst_null )
		{
			XST_LOG_ERR( "Create material: " << strName << " failed. Not enough memory." );
			return xst_null;
		}

		return pMat;
	}

	MaterialPtr CMaterialManager::GetDefaultMaterial(const Resources::CMesh *pMesh)
	{
		IInputLayout* pIL = pMesh->GetInputLayout();
		xst_assert( pIL != xst_null, "(CMaterialManager::GetDefaultMaterial) Input layout must be set" );
		//Get or create default material for this input layout
		DefaultMatMap::iterator Itr;
		//If resource not found
		if( XST::MapUtils::FindPlace( m_mDefaultMaterials, pIL, &Itr ) )
		{
			//Create new material	
			CMaterial* pMat = xst_new CMaterial( m_pShaderMgr, this, 0, "Default", XST::ResourceType::MATERIAL, XST::ResourceStates::CREATED, xst_null );
			if( pMat == xst_null )
			{
				XST_LOG_ERR( "Default material creation failed. Memory error." );
				return MaterialPtr();
			}

			//Set defaults
			IPass* pPass = pMat->GetCurrentTechnique()->GetPass( 0 );
			pPass->SetVertexShader( m_pShaderMgr->GetDefaultVertexShader( pMesh ) );
			pPass->SetPixelShader( m_pShaderMgr->GetDefaultPixelShader() );

			//Add material to the buffer
			MaterialPtr pMaterial( pMat );
			XST::MapUtils::InsertOnPlace( m_mDefaultMaterials, pIL, pMaterial, Itr );
			return pMaterial;
		}

		//If resource found returns it
		return Itr->second;
	}

	i32	CMaterialManager::PrepareResource(ResourcePtr pRes)
	{
		MaterialPtr pMat = pRes;
		//Compile material

		return 0;
	}

	i32 CMaterialManager::PrepareMaterials(xst_castring& strGroup)
	{
		XST::CLuaTable MaterialsTable( "materials" );

		if( MaterialsTable.Create( &m_LuaApi, "materials" ) != RESULT::OK )
		{
			return RESULT::FAILED;
		}

		CLuaMaterialParser Parser;

		XST::CLuaTable::ChildTableItr TableItr = MaterialsTable.GetChildTableMap().begin();
		for(; TableItr != MaterialsTable.GetChildTableMap().end(); ++TableItr)
		{
			XST::CLuaTable* pTmpTable = TableItr->second;
			if( pTmpTable == xst_null || this->GetResource( TableItr->first ) ) continue;

			MaterialPtr pMat = CreateMaterial( TableItr->first, strGroup );
			if( Parser.Parse( pMat.GetPointer(), pTmpTable ) != RESULT::OK )
			{
				continue;
			}
		}

		return RESULT::OK;
	}

	MaterialPtr CMaterialManager::LoadMaterial(xst_castring& strName, xst_castring& strGroupName)
	{
		LuaScriptPtr pScript = CLuaScriptManager::GetSingletonPtr()->LoadScript( strName, strGroupName, &m_LuaApi );
		if( pScript.IsNull() )
		{
			return MaterialPtr();
		}

        xst_assert( 0, "(CMaterialManager::LoadMaterial) Not implemented" );
		return MaterialPtr();
	}

	i32 CMaterialManager::SaveMaterial(xst_castring &strFileName, XSE::MaterialPtr pMat)
	{
		if( pMat.IsNull() )
		{
			XST_LOG_ERR( "Material is null" );
			return RESULT::FAILED;
		}

		XST::CBinaryWriter Writer;
		if( Writer.Open( strFileName ) != RESULT::OK )
		{
			XST_LOG_ERR( "Could not create file: " << strFileName );
			return RESULT::FAILED;
		}

		//Write header
		Writer.Write( XSE_BIN_MAT_VER_HEADER_1_0, 3 );
		//Write version
		Writer.Write( (u16)XSE_BIN_MAT_VER_1_0 );
		//Write name
		Writer.Write( pMat->GetResourceName() );
		//Write ambient
		//Writer.Write( pMat->GetAmbient().x );
		//Writer.Write( pMat->GetAmbient().y );
		//Writer.Write( pMat->GetAmbient().z );
		//Writer.Write( 0.0f );
		////Write diffuse
		//Writer.Write( pMat->GetDiffuse().x );
		//Writer.Write( pMat->GetDiffuse().y );
		//Writer.Write( pMat->GetDiffuse().z );
		//Writer.Write( 0.0f );
		////Write specular
		//Writer.Write( pMat->GetSpecular().x );
		//Writer.Write( pMat->GetSpecular().y );
		//Writer.Write( pMat->GetSpecular().z );
		//Writer.Write( 0.0f );

		return RESULT::OK;
	}

}//xse