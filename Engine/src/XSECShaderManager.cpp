#include "XSECShaderManager.h"
#include "XSEIRenderSystem.h"
#include "XSEIShaderSystem.h"
#include "XSECResourceFileManager.h"
#include "XSEIInputLayout.h"
#include "XSECMesh.h"

namespace XSE
{


	static lpcastr g_alpszShaderTypes[] =
	{
		"cg", "hlsl", "glsl", "fx", "cgfx"
	};

	CShaderManager::CShaderManager() : m_pRenderSystem( xst_null ), m_pAllocator( xst_null )
	{

	}

	CShaderManager::~CShaderManager()
	{
	}
	//
	i32 CShaderManager::Init()
	{
		//Create default shaders
		IInputLayout* pIL = m_pRenderSystem->GetInputLayout( ILE::POSITION );
		xst_assert2( pIL != xst_null );
		xst_assert2( pIL->GetVertexShader() != xst_null );
		
		/*IVertexShader* pVS = m_pRenderSystem->CreateDefaultVertexShader( pIL, this, 0, strVSName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, xst_null );
		if( pVS == xst_null )
		{
			return XST_FAIL;
		}*/

		xst_castring strPSName( "xse_default_ps" );
		Resources::IPixelShader* pPS = m_pRenderSystem->CreateDefaultPixelShader( xst_null, this, 0, strPSName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, xst_null );
		if( pPS == xst_null )
		{
			return XST_FAIL;
		}

		m_pDefaultPS = PixelShaderPtr( pPS );
		m_pDefaultVS = VertexShaderPtr( pIL->GetVertexShader() );

		return XST_OK;
	}

	/*i32 CShaderManager::_CreateMemoryPool(cul32 &ulObjCount, XST::IAllocator *pAllocator)
	{
		return XST_OK;
	}*/

	void CShaderManager::SetShaderConstantName(SHADER_CONSTANT eConstant, xst_castring& strName)
	{
		xst_assert( m_pRenderSystem, "(CShaderManager::SetShaderConstantNames" );
		m_pRenderSystem->SetShaderConstantName( eConstant, strName );
	}

	i32 CShaderManager::ApplyShaderConstantNames()
	{
		xst_assert( m_pRenderSystem, "(CShaderManager::SetShaderConstantNames" );
		return m_pRenderSystem->ApplyShaderConstantNames();
	}

	/*i32 CShaderManager::_CreateVSMemoryPool(cul32 &ulObjCount, XST::IAllocator *pAllocator)
	{
		return XST_OK;
	}

	i32 CShaderManager::_CreatePSMemoryPool(cul32 &ulObjCount, XST::IAllocator *pAllocator)
	{
		return XST_OK;
	}*/

    ShaderWeakPtr CShaderManager::_CompileShader(SHADER_TYPE eType, SHADER_PROFILE eProfile, xst_castring& strName, xst_castring& strEntryPoint, const IInputLayout* pIL, xst_castring& strCode, xst_castring& strGroupName)
    {
        ShaderPtr pShader = _CreateShader( eType, strName, strEntryPoint, strGroupName );
        auto pSh = pShader.GetPtr();
        if( pSh )
        {
            if( eType == ShaderTypes::VERTEX )
            {
                Resources::IVertexShader* pVSh = reinterpret_cast< Resources::IVertexShader* >( pSh );
                pVSh->SetInputLayout( pIL );
            }

            if( XST_SUCCEEDED( pSh->Compile( strEntryPoint, eProfile, strCode ) ) )
            {
                pSh->_SetResourceState( Resources::ResourceStates::PREPARED );
                return pShader;
            }
        }
        return ShaderWeakPtr();
    }

	VertexShaderPtr CShaderManager::CompileVertexShader(xst_castring& strName, xst_castring& strEntryPoint, u32 uInputLayoutElementIds, xst_castring& strCode, xst_castring& strGroupName)
	{
		/*ShaderPtr pShader = _CreateShader( ShaderTypes::VERTEX, strName, strEntryPoint, strGroupName );
		if( pShader == xst_null )
		{
			return pShader;
		}

		VertexShaderPtr pVS( pShader );
		
        pVS->SetInputLayout( m_pRenderSystem->GetInputLayout( uInputLayoutElementIds ) );
		if( XST_FAILED( pVS->Compile( strEntryPoint, ShaderProfiles::VS_BEST, strCode ) ) )
		{
			this->DestroyResource( pVS );
			return VertexShaderPtr();
		}
		pVS->m_iResourceState = Resources::ResourceStates::PREPARED;
		return pVS;*/
        return _CompileShader( ShaderTypes::VERTEX, ShaderProfiles::VS_BEST, strName, strEntryPoint, 
                               m_pRenderSystem->GetInputLayout( uInputLayoutElementIds ), strCode, strGroupName );
	}

	PixelShaderPtr CShaderManager::CompilePixelShader(xst_castring& strName, xst_castring& strEntryPoint, xst_castring& strCode, xst_castring& strGroupName)
	{
		ShaderPtr pShader = _CreateShader( ShaderTypes::PIXEL, strName, strEntryPoint, strGroupName );
		if( pShader == xst_null )
		{
			return pShader;
		}

		PixelShaderPtr pPS( pShader );
		
		if( XST_FAILED( pPS->Compile( strEntryPoint, ShaderProfiles::PS_BEST, strCode ) ) )
		{
			this->DestroyResource( pPS );
			return PixelShaderPtr();
		}
		pPS->m_iResourceState = Resources::ResourceStates::PREPARED;
		return pPS;
	}

	ShaderPtr CShaderManager::_CreateShader(SHADER_TYPE eType, xst_castring& strName, xst_castring& strEntryPoint, xst_castring& strGroupName)
	{
		//ul32 ulHandle = XST::CHash::GetCRC( strShaderName );
		GroupPtr pGr = this->GetOrCreateGroup( strGroupName );
		this->m_strEntryPoint = strEntryPoint;
		this->m_eShaderType = eType;
	
		//Try to get this resource
		bool bCreated = false;
		ResourcePtr pRes = this->GetOrCreateResource( strName, pGr, &bCreated );
		ShaderPtr pShader( pRes );
		if( !bCreated ) //if resource already exists
		{
			return pShader;
		}

		return pShader;
	}

	Resources::IResource*	CShaderManager::_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup)
	{
		xst_assert( m_pRenderSystem, "(CShaderManager::_CreateResource) RenderSystem not set" );
		Resources::IResource* pRes = xst_null;
		Resources::IShader* pShader = xst_null;
		XST::xst_astr16 strShaderType;
		//Get file extension

		switch( m_eShaderType )
		{
			case ShaderTypes::VERTEX:
			{
				pShader = (Resources::IShader*)m_pRenderSystem->CreateVertexShader( xst_null, this, ulHandle, strName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, m_pAllocator );
			}
			break;

			case ShaderTypes::PIXEL:
				pShader = (Resources::IShader*)m_pRenderSystem->CreatePixelShader( this, ulHandle, strName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, m_pAllocator );
			break;
		}

		if( pShader == xst_null )
		{
			return xst_null;
		}

		pShader->m_iResourceState = XST::ResourceStates::CREATED;
		pShader->m_strShaderEntryPoint = this->m_strEntryPoint;

		return pShader;
	}

	ShaderPtr CShaderManager::_LoadShader(SHADER_TYPE eType, xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName)
	{
		this->m_eShaderType = eType;
		this->m_strEntryPoint = strEntryPoint;
		this->m_eShaderProfile = eProfile;

		ResourcePtr pRes = this->LoadResource( strShaderName, strGroupName, strFileName, strGroupName );
		if( pRes.IsNull() )
		{
			return ShaderPtr();
		}

		//ShaderPtr pShader( pRes );
		//pShader->m_eShaderType = eType;
		
		/*if( XST_FAILED( this->AddResource( strShaderName, pShader, strGroupName ) ) )
		{
			this->DestroyResource( pShader );
			return ShaderPtr();
		}*/

		this->m_eShaderType = ( SHADER_TYPE )0;
		this->m_strEntryPoint.clear();
		this->m_eShaderProfile = ( SHADER_PROFILE )0;
		return pRes;
	}

	VertexShaderPtr	CShaderManager::LoadVertexShader(xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName)
	{
		/*this->m_eShaderType = ShaderTypes::VERTEX;
		this->m_strEntryPoint = strEntryPoint;

		ResourcePtr pRes = this->LoadResource( strName, _CreateName( strName, strEntryPoint ), strGroupName );
		if( pRes.IsNull() )
		{
			return VertexShaderPtr();
		}

		VertexShaderPtr pVS( pRes );
		pVS->m_eShaderType = ShaderTypes::VERTEX;
	
		this->m_eShaderType = ( SHADER_TYPE )0;

		return pVS;*/
		return _LoadShader( ShaderTypes::VERTEX, strFileName, strShaderName, strEntryPoint, eProfile, strGroupName );
	}

	PixelShaderPtr	CShaderManager::LoadPixelShader(xst_castring& strFileName, xst_castring& strShaderName, xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strGroupName)
	{
		/*this->m_eShaderType = ShaderTypes::PIXEL;
		this->m_strEntryPoint = strEntryPoint;

		ResourcePtr pRes = this->LoadResource( strName, _CreateName( strName, strEntryPoint ), strGroupName );

		if( pRes.IsNull() )
		{
			return PixelShaderPtr();
		}

		PixelShaderPtr pPS( pRes );
		pPS->m_eShaderType = ShaderTypes::PIXEL;
		this->m_eShaderType = ( SHADER_TYPE )0;
		return pPS;*/
		return _LoadShader( ShaderTypes::PIXEL, strFileName, strShaderName, strEntryPoint, eProfile, strGroupName );
	}

	xst_astring CShaderManager::CreateShaderCode(u32 uiVSInput, u32 uiPSInput)
	{
		return m_pRenderSystem->GetShaderSystem()->CreateShaderCode( uiVSInput, uiPSInput );
	}

	VertexShaderPtr CShaderManager::GetDefaultVertexShader(const IInputLayout* pIL)
	{
		xst_assert( pIL->GetVertexShader() != xst_null, "(CShaderManager::GetdefaultVertexShader) Vertex shader for input layout must be created!" );
		//Try to get this shader
		VShaderMap::iterator Itr;
		if( XST::MapUtils::FindPlace( m_mDefaultVShaders, pIL, &Itr ) )
		{
			XST::MapUtils::InsertOnPlace( m_mDefaultVShaders, pIL, VertexShaderPtr( pIL->GetVertexShader() ), Itr );
		}

		//If found return it
		return Itr->second;
	}

	VertexShaderPtr CShaderManager::GetDefaultVertexShader(const Resources::CMesh* pMesh)
	{
		return GetDefaultVertexShader( pMesh->GetInputLayout() );
	}

	i32	CShaderManager::PrepareResource(ResourceWeakPtr *const ppRes)
	{
		xst_assert2( ppRes && (*ppRes).IsValid() );
		auto pRes = *ppRes;
		Resources::IShader* pShader = (Resources::IShader*)pRes.GetPtr();
		pShader->m_eProfile = this->m_eShaderProfile;
		pShader->m_eShaderLanguage = this->m_eShaderLang;
		pShader->m_eShaderType = this->m_eShaderType;
		if( XST_FAILED( m_pRenderSystem->GetShaderSystem()->PrepareResource( pRes.GetPtr() ) ) )
		{
			return XST_FAIL;
		}
		//pRes->m_iResourceState = ResourceStates::PREPARED;
		pShader->m_iResourceState = Resources::ResourceStates::PREPARED;

		return XST_OK;
	}

}//xse