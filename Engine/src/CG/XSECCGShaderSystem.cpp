#include "CG/XSECCGShaderSystem.h"
#include "CG/XSECGTypes.h"
#include "XSECCGPixelShader.h"
#include "XSECCGVertexShader.h"

namespace XSE
{
	using namespace Resources;

	CCGShaderSystem::CCGShaderSystem() : IShaderSystem( ShaderLanguages::CG )
	{
		m_CGContext = xst_null;
	}

	CCGShaderSystem::~CCGShaderSystem()
	{
		UnloadLibraries();
	}

	i32 CCGShaderSystem::LoadLibraries()
	{
		m_hDll = XST::Platform::LoadLibrary( "cg.dll" );
		if( m_hDll == xst_null )
		{
			XST_LOG_ERR( "Failed to load CG.dll" );
			return XST_FAIL;
		}

		//CG
		XSE_LOAD_FUNC( cgCreateContext, pfncgCreateContext, m_hDll, XST_TOSTRING( cgCreateContext ) );
		XSE_LOAD_FUNC( cgGetLastErrorString, pfncgGetLastErrorString, m_hDll, XST_TOSTRING( cgGetLastErrorString ) );
		XSE_LOAD_FUNC( cgGetLastListing, pfncgGetLastListing, m_hDll, XST_TOSTRING( cgGetLastListing ) );
		XSE_LOAD_FUNC3( cgDestroyContext, m_hDll );
		XSE_LOAD_FUNC3( cgCreateProgram, m_hDll );
		XSE_LOAD_FUNC3( cgSetMatrixParameterfr, m_hDll );
		
		return XST_OK;
	}

	void CCGShaderSystem::UnloadLibraries()
	{
		XST::Platform::CloseLibrary( m_hDll );
	}

	void CCGShaderSystem::Destroy()
	{
		cgDestroyContext( m_CGContext );
		m_CGContext = xst_null;
	}

	i32 CCGShaderSystem::Init(IRenderSystem* pRS)
	{
		UnloadLibraries();
		if( XST_FAILED( LoadLibraries() ) )
		{
			return XST_FAIL;
		}

		m_pRS = pRS;

		m_CGContext = cgCreateContext();
		CHECK_CG_ERR( "creating context", "" );

		return XST_OK;
	}

	CGprogram CCGShaderSystem::CreateProgram(lpcastr lpszShaderCode, CGprofile Profile, lpcastr lpszEntryPoint, lpcastr lpszShaderName)
	{
		XST_LOG_ERR( lpszShaderCode );
		CGprogram Program = cgCreateProgram( m_CGContext, CG_SOURCE, lpszShaderCode, Profile, lpszEntryPoint, xst_null );
		if( XST_FAILED( this->CheckForErrors( "Program creation", lpszShaderName ) ) )
		{
			if( Program ) 
			{
				cgDestroyProgram( Program );
			}
			return xst_null;
		}

		return Program;
	}

	Resources::IVertexShader*	CCGShaderSystem::CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
	{
		//CVertexShader* pVS = xst_new CVertexShader( pRS, 
		return 0;
	}

	Resources::IPixelShader*	CCGShaderSystem::CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
	{
		return 0;
	}

	i32	CCGShaderSystem::CompileVertexShader(Resources::IVertexShader* pVS)
	{
		return CompileVertexShader( (lpcastr)pVS->GetResourceFile().GetPointer()->GetData().GetData(), 0, pVS );
	}
	
	i32	CCGShaderSystem::CompilePixelShader(Resources::IPixelShader* pPS)
	{
		return CompilePixelShader( (lpcastr)pPS->GetResourceFile().GetPointer()->GetData().GetData(), 0, pPS );
	}

	i32	CCGShaderSystem::CompileVertexShader(lpcastr lpszShaderCode, ul32 ulCodeSize, Resources::IVertexShader* pVS)
	{
		xst_assert( pVS, "(CCGShaderSystem::CompileVertexShader)" );

		CCGVertexShader* pShader = (CCGVertexShader*)pVS;
		if( pShader->m_CGShader == xst_null )
		{
			//Create program if not created
			pShader->m_CGShader = CreateProgram( lpszShaderCode, pShader->m_CGProfile, pShader->GetEntryPoint(), pShader->GetResourceName().data() );
			if( pShader->m_CGShader == xst_null )
			{
				return XST_FAIL;
			}

			pShader->m_bCompiled = false;
		}

		return XST_OK;
	}

	i32	CCGShaderSystem::CompilePixelShader(lpcastr lpszShaderCode, ul32 ulCodeSize, Resources::IPixelShader* pPS)
	{
		xst_assert( pPS, "(CCGShaderSystem::CompilePixelShader)" );

		CCGPixelShader* pShader = (CCGPixelShader*)pPS;
		if( pShader->m_CGShader == xst_null )
		{
			//Create program if not created
			pShader->m_CGShader = CreateProgram( lpszShaderCode, pShader->m_CGProfile, pShader->GetEntryPoint(), pShader->GetResourceName().data() );
			if( pShader->m_CGShader == xst_null )
			{
				return XST_FAIL;
			}

			pShader->m_bCompiled = false;
		}

		return XST_OK;
	}

	void CCGShaderSystem::DestroyVertexShader(Resources::IVertexShader* pVS)
	{
		xst_assert( pVS, "(CCGShaderSystem::DestroyVertexShader)" );
		CCGVertexShader* pShader = (CCGVertexShader*)pVS;
		if( pShader->m_CGShader != xst_null )
		{
			cgDestroyProgram( pShader->m_CGShader );
		}
	}

	void CCGShaderSystem::DestroyPixelShader(Resources::IPixelShader* pPS)
	{
		xst_assert( pPS, "(CCGShaderSystem::DestroyPixelShader)" );
		CCGPixelShader* pShader = (CCGPixelShader*)pPS;
		if( pShader->m_CGShader != xst_null )
		{
			cgDestroyProgram( pShader->m_CGShader );
		}

	}

	i32	CCGShaderSystem::SetVertexShader(Resources::IVertexShader* pVS)
	{
		return 0;
	}

	i32 CCGShaderSystem::SetPixelShader(Resources::IPixelShader* pPS)
	{
		return 0;
	}

	void CCGShaderSystem::SetConstantBuffers()
	{
		cgSetMatrixParameterfr( m_CGMtxWorldParam, m_mtxWorld.mm );
		cgSetMatrixParameterfr( m_CGMtxViewParam, m_mtxView.mm );
		cgSetMatrixParameterfr( m_CGMtxProjParam, m_mtxProj.mm );
		cgSetMatrixParameterfr( m_CGMtxViewProjParam, m_mtxViewProj.mm );
		cgSetMatrixParameterfr( m_CGMtxWVPParam, m_mtxWVP.mm );
	}

	i32 CCGShaderSystem::PrepareResource(Resources::IResource* pRes)
	{
		IShader* pShader = (IShader*)pRes;

		switch( pShader->GetShaderType() )
		{
			case ShaderTypes::VERTEX:
			{
				CCGVertexShader* pVShader = (CCGVertexShader*)pShader;
				return this->CompileVertexShader( pVShader );
			}
			break;

			case ShaderTypes::PIXEL:
			{
				CCGPixelShader* pPShader = (CCGPixelShader*)pShader;
				return this->CompilePixelShader( pPShader );
			}
			break;
		}

		XST_LOG_ERR( "Invalid shader type" );
		return XST_FAIL;
	}

	i32 CCGShaderSystem::CheckForErrors(lpcastr situation, lpcastr lpszShaderName)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if( error != CG_NO_ERROR ) 
		{
			if( error == CG_COMPILER_ERROR ) 
			{
				/*fprintf(stderr,
					"Program: %s\n"
					"Situation: %s\n"
					"Error: %s\n\n"
					"Cg compiler output...\n%s",
					lpszShaderName, situation, string,
					cgGetLastListing( m_CGContext ));
				XST_LOG_ERR( stderr );*/
				XST_LOG_ERR(	"Program: " << lpszShaderName << 
								"\nSituation: " << situation << 
								"\nError: " << string <<
								"\nCg compiler output: " << cgGetLastListing( m_CGContext ) );
			} 
			else 
			{
				/*fprintf(stderr,
					"Program: %s\n"
					"Situation: %s\n"
					"Error: %s",
					lpszShaderName, situation, string);
				XST_LOG_ERR( stderr );*/
				XST_LOG_ERR(	"Program: " << lpszShaderName << 
								"\nSituation: " << situation << 
								"\nError: " << string );
			}

			return XST_FAIL;
		}

		return XST_OK;
	}

}//xse