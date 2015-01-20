#include "XSECD3D11CGShaderSystem.h"
#include "XSECD3D11RenderSystem.h"
#include "CG/XSECGTypes.h"
#include "../CG/XSECCGVertexShader.h"
#include "../CG/XSECCGPixelShader.h"

namespace XSE
{
	namespace D3D11
	{		
		//CG D3D11
		typedef HRESULT (CGD3D11ENTRY * pfncgD3D11SetDevice)(CGcontext Context, ID3D11Device *pDevice);

		//CG D3D11
		pfncgD3D11SetDevice	cgD3D11SetDevice = xst_null;

		XSE_CG_TDF( CGprogram, cgD3D11LoadProgram )(CGprogram, UINT);
		XSE_CG_TDF( HRESULT, cgD3D11BindProgram)(CGprogram);

		XSE_CG_DF( cgD3D11LoadProgram );
		XSE_CG_DF( cgD3D11BindProgram );

		namespace CG
		{
			static xst_astring g_strConstantBuffers = "";
			static xst_astring g_strConstantVUniforms = "";
			static xst_astring g_strConstantPUniforms = "";
			static xst_astring g_strShader = "";

			xst_astring& CreateVUniforms(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				std::stringstream ss;
				ss << ",uniform float4x4 " << astrConstants[ ShaderConstants::MTX_VIEW ] << std::endl;
				ss << ",uniform float4x4 " << astrConstants[ ShaderConstants::MTX_PROJECTION ] << std::endl;
				ss << ",uniform float " << astrConstants[ ShaderConstants::TIME ] << std::endl;
				ss << ",uniform float3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_COLOR ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR_POWER ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT_COLOR ];

				g_strConstantVUniforms = ss.str();
				return g_strConstantVUniforms;
			}

			xst_astring& CreatePUniforms(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				std::stringstream ss;
				ss << ",uniform float " << astrConstants[ ShaderConstants::TIME ] << std::endl;
				ss << ",uniform float3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_COLOR ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR_POWER ] << std::endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT_COLOR ];

				g_strConstantPUniforms = ss.str();
				return g_strConstantPUniforms;
			}

			xst_astring& CreateVShader(xst_castring& strShader, xst_castring& strEntryPoint)
			{
				g_strShader.clear();
				//Set constant buffers
				g_strShader = g_strConstantBuffers + "\n\n";
				//Find entry point
				ul32 ulPos = strShader.find_first_of( strEntryPoint );
				ulPos = strShader.find_first_of( ")", ulPos );

				if( ulPos > 0 )
				{
					//Set uniforms
					g_strShader += strShader.substr( 0, ulPos );
					g_strShader += g_strConstantVUniforms;
					g_strShader += strShader.substr( ulPos, strShader.length() - ulPos );
				}

				return g_strShader;
			}

			xst_astring& CreatePShader(xst_castring& strShader, xst_castring& strEntryPoint)
			{
				g_strShader.clear();
				//Set constant buffers
				g_strShader = g_strConstantBuffers + "\n\n";
				//Find entry point
				ul32 ulPos = strShader.find_first_of( strEntryPoint );
				ulPos = strShader.find_first_of( ")", ulPos );

				if( ulPos > 0 )
				{
					//Set uniforms
					g_strShader += strShader.substr( 0, ulPos );
					g_strShader += g_strConstantPUniforms;
					g_strShader += strShader.substr( ulPos, strShader.length() - ulPos );
				}

				return g_strShader;
			}

			void SetProfiles(D3D_FEATURE_LEVEL eLevel, CGprofile aeProfiles[ ShaderProfiles::_ENUM_COUNT ]);
		}//CG


		CCGShaderSystem::CCGShaderSystem(XSE::D3D11::CRenderSystem *pRS) : m_pRenderSystem( pRS )
		{
			
		}

		CCGShaderSystem::~CCGShaderSystem()
		{
			UnloadLibraries();
		}

		void CCGShaderSystem::Destroy()
		{
			cgD3D11SetDevice( m_CGContext, xst_null );
			XSE::CCGShaderSystem::Destroy();
		}

		i32 CCGShaderSystem::LoadLibraries()
		{
			m_hD3D11Dll = XST::Platform::LoadLibrary( "cgD3D11.dll" );
			if( m_hD3D11Dll == xst_null )
			{
				XST_LOG_ERR( "Failed to load cgD3D11.dll" );
				return XST_FAIL;
			}

			//CG D3D11
			xst_unknown hDll = m_hD3D11Dll;
			XSE_LOAD_FUNC( cgD3D11SetDevice, pfncgD3D11SetDevice, m_hD3D11Dll, XST_TOSTRING( cgD3D11SetDevice ) );
			XSE_LOAD_FUNC3( cgD3D11LoadProgram, m_hD3D11Dll );
			XSE_LOAD_FUNC3( cgD3D11BindProgram, m_hD3D11Dll );

			return XST_OK;
		}

		void CCGShaderSystem::UnloadLibraries()
		{
			XST::Platform::CloseLibrary( m_hD3D11Dll );
		}

		i32	CCGShaderSystem::ApplyShaderConstantNames()
		{
			//CG::CreateConstantBuffers( this->CONSTANT_NAMES );
			CG::CreateVUniforms( this->CONSTANT_NAMES );
			CG::CreatePUniforms( this->CONSTANT_NAMES );
			return XST_OK;
		}

		i32 CCGShaderSystem::Init(IRenderSystem* pRS)
		{
			//Init base cg system
			if( XST_FAILED( XSE::CCGShaderSystem::Init( pRS ) ) )
			{
				return XST_FAIL;
			}

			UnloadLibraries();
			if( XST_FAILED( LoadLibraries() ) )
			{
				return XST_FAIL;
			}

			m_pRenderSystem = (CRenderSystem*)pRS;

			CG::SetProfiles( m_pRenderSystem->m_eFeatureLevel, m_aeProfiles );

			HRESULT hr = cgD3D11SetDevice( this->m_CGContext, m_pRenderSystem->m_pDevice );
			CHECK_CG_ERR( "setting Direct3D11 device", "" );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "Setting Direct3D11 device failed: " << hr );
				return XST_FAIL;
			}

			if( XST_FAILED( ApplyShaderConstantNames() ) )
			{
				return XST_OK;
			}

			return XST_OK;
		}

		xst_astring	CCGShaderSystem::CreateShaderCode(u32 uiVSInput, u32 uiPSInput)
		{
			xst_astring strCode;
			strCode.reserve( 1024 );

			//strCode += GetShaderCode( ShaderCodes::PER_FRAME_CBUFFER ) + xst_endl;
			//strCode += GetShaderCode( ShaderCodes::PER_OBJECT_CBUFFER ) + xst_endl;

			//Vertex shader
			xst_stringstream ssVS;
			ssVS<< "struct VS_INPUT{" << xst_endl
				<< "	float4 pos : POSITION;" << xst_endl
				<< GetShaderCode( ShaderCodes::PER_FRAME_VS_CBUFFER ) << xst_endl
				<< GetShaderCode( ShaderCodes::PER_DRAWCALL_VS_CBUFFER ) << xst_endl;

			if( uiVSInput & ILEs::COLOR )
			{
				ssVS << "	float4 color : COLOR0;" << xst_endl;
			}

			ssVS<< "};" << xst_endl
				<< "struct PS_INPUT{" << xst_endl
				<< "	float4 pos : SV_POSITION" << xst_endl;

			if( uiPSInput & ILEs::COLOR )
			{
				ssVS << "	float4 color : COLOR0;" << xst_endl;
			}

			ssVS<< "};" << xst_endl
				<< "PS_INPUT vs_main(VS_INPUT In)" << xst_endl
				<< "{" << xst_endl
				<< "	PS_INPUT Out = (PS_INPUT)0;" << xst_endl
				<< "	Out.pos = mul( In.pos, " << GetConstantName( ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION ) << " );" <<xst_endl
				<< "	Out.color = In.color;" << xst_endl
				<< "	return Out;" << xst_endl
				<< "}" << xst_endl;

			strCode += ssVS.str() + xst_endl;

			xst_stringstream ssPS;
			ssPS<< "float4 ps_main(PS_INPUT In) : COLOR" << xst_endl
				<< "{" << xst_endl
				<< "	return In.color;" << xst_endl
				<< "}";

			strCode += ssPS.str();

			return strCode;
		}

		IVertexShader*	CCGShaderSystem::CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CCGVertexShader* pShader = xst_new CCGVertexShader( m_pRenderSystem, this, pIL, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			if( pShader == xst_null )
			{
				return xst_null;
			}

			return pShader;
		}

		void CCGShaderSystem::UpdateDrawCallInputs()
		{
		}

		void CCGShaderSystem::UpdateFrameInputs()
		{
		}

		void CCGShaderSystem::UpdateMaterialInputs()
		{

		}

		IPixelShader*	CCGShaderSystem::CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CCGPixelShader* pShader = xst_new CCGPixelShader( m_pRenderSystem, this, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			if( pShader == xst_null )
			{
				return xst_null;
			}

			return pShader;
		}

		IVertexShader*	CCGShaderSystem::CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CCGVertexShader* pShader = xst_new CCGVertexShader( m_pRenderSystem, this, xst_null, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			if( pShader == xst_null )
			{
				return xst_null;
			}

			return pShader;
		}

		IPixelShader*	CCGShaderSystem::CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CCGPixelShader* pShader = xst_new CCGPixelShader( m_pRenderSystem, this, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			if( pShader == xst_null )
			{
				return xst_null;
			}

			return pShader;
		}

		i32	CCGShaderSystem::CompileVertexShader(Resources::IVertexShader* pVS)
		{
			xst_assert( pVS, "(CCGShaderSystem::CompileVertexShader)" );
			CCGVertexShader* pShader = (CCGVertexShader*)pVS;
			if( pShader->GetCGProfile() == CG_PROFILE_UNKNOWN )
			{
				pShader->SetCGProfile( this->m_aeProfiles[ pShader->GetProfile() ] );
			}

			xst_astring strData = (lpcastr)pShader->GetResourceFile().GetPtr()->GetData().GetPointer();
			CG::CreateVShader( strData, pShader->GetEntryPoint() );

			//if( XST_FAILED( XSE::CCGShaderSystem::CompileVertexShader( pVS ) ) )
			if( XST_FAILED( XSE::CCGShaderSystem::CompileVertexShader( CG::g_strShader.data(), CG::g_strShader.length(), pVS ) ) )
			{
				return XST_FAIL;
			}
			
			CGprogram CGShader = pShader->GetCGShader();
			if( CGShader == xst_null )
			{
				XST_LOG_ERR( "CG vertex shader: " << pShader->GetResourceName() << " not created" );
				return XST_FAIL;
			}
			cgD3D11LoadProgram( CGShader, pShader->GetShaderFlags() );
			CHECK_CG_ERR( "Loading d3d11 program", pVS->GetResourceName().data() );
			return XST_OK;
		}
		
		i32	CCGShaderSystem::CompilePixelShader(Resources::IPixelShader* pPS)
		{
			xst_assert( pPS, "(CCGShaderSystem::CompilePixelShader)" );
			//If profile is not created, create it
			CCGPixelShader* pShader = (CCGPixelShader*)pPS;
			if( pShader->GetCGProfile() == CG_PROFILE_UNKNOWN )
			{
				pShader->SetCGProfile( this->m_aeProfiles[ pShader->GetProfile() ] );
			}

			/*if( XST_FAILED( XSE::CCGShaderSystem::CompilePixelShader( pPS ) ) )
			{
				return XST_FAIL;
			}*/

			xst_astring strData = (lpcastr)pShader->GetResourceFile().GetPtr()->GetData().GetPointer();
			CG::CreatePShader( strData, pShader->GetEntryPoint() );

			//if( XST_FAILED( XSE::CCGShaderSystem::CompileVertexShader( pVS ) ) )
			if( XST_FAILED( XSE::CCGShaderSystem::CompilePixelShader( CG::g_strShader.data(), CG::g_strShader.length(), pPS ) ) )
			{
				return XST_FAIL;
			}

			
			CGprogram CGShader = pShader->GetCGShader();
			if( CGShader == xst_null )
			{
				XST_LOG_ERR( "CG pixel shader: " << pShader->GetResourceName() << " not created" );
				return XST_FAIL;
			}
			cgD3D11LoadProgram( CGShader, pShader->GetShaderFlags() );
			CHECK_CG_ERR( "Loading d3d11 program", pPS->GetResourceName().data() );
			return XST_OK;
		}

		i32	CCGShaderSystem::CompileVertexShader(Resources::IVertexShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile)
		{
			xst_assert( pVS, "(CCGShaderSystem::CompileVertexShader)" );
			CCGVertexShader* pShader = (CCGVertexShader*)pVS;
		
			if( pShader->GetCGProfile() == CG_PROFILE_UNKNOWN )
			{
				pShader->SetCGProfile( this->m_aeProfiles[ eProfile ] );
			}

			if( XST_FAILED( XSE::CCGShaderSystem::CompileVertexShader( pVS ) ) )
			{
				return XST_FAIL;
			}
			
			CGprogram CGShader = pShader->GetCGShader();
			if( CGShader == xst_null )
			{
				XST_LOG_ERR( "CG vertex shader: " << pShader->GetResourceName() << " not created" );
				return XST_FAIL;
			}
			cgD3D11LoadProgram( CGShader, pShader->GetShaderFlags() );
			CHECK_CG_ERR( "Loading d3d11 program", pVS->GetResourceName().data() );
			return XST_OK;
		}

		i32	CCGShaderSystem::CompilePixelShader(Resources::IPixelShader* pPS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile)
		{
			xst_assert( pPS, "(CCGShaderSystem::CompilePixelShader)" );
			//If profile is not created, create it
			CCGPixelShader* pShader = (CCGPixelShader*)pPS;
			if( pShader->GetCGProfile() == CG_PROFILE_UNKNOWN )
			{
				pShader->SetCGProfile( this->m_aeProfiles[ eProfile ] );
			}

			if( XST_FAILED( XSE::CCGShaderSystem::CompilePixelShader( pPS ) ) )
			{
				return XST_FAIL;
			}

			
			CGprogram CGShader = pShader->GetCGShader();
			if( CGShader == xst_null )
			{
				XST_LOG_ERR( "CG pixel shader: " << pShader->GetResourceName() << " not created" );
				return XST_FAIL;
			}
			cgD3D11LoadProgram( CGShader, pShader->GetShaderFlags() );
			CHECK_CG_ERR( "Loading d3d11 program", pPS->GetResourceName().data() );
			return XST_OK;
		}

		i32	CCGShaderSystem::SetVertexShader(const Resources::IVertexShader* pVS)
		{
			xst_assert( pVS, "(D3D11::CCGShaderSystem::SetVertexShader)" );
			const CCGVertexShader* pShader = (CCGVertexShader*)pVS;
			cgD3D11BindProgram( pShader->GetCGShader() );
			return XST_OK;
		}

		i32 CCGShaderSystem::SetPixelShader(const Resources::IPixelShader* pPS)
		{
			return 0;
		}


		namespace CG
		{
			void SetProfiles(D3D_FEATURE_LEVEL eLevel, CGprofile aeProfiles[ ShaderProfiles::_ENUM_COUNT ])
			{
				memset( aeProfiles, CGprofile::CG_PROFILE_UNKNOWN, sizeof( i32 ) * ShaderProfiles::_ENUM_COUNT );

				//SM 1.0
				aeProfiles[ ShaderProfiles::VS_1_1 ] = CGprofile::CG_PROFILE_VS_1_1;
				

				switch( eLevel )
				{
					case D3D_FEATURE_LEVEL_9_1:
					case D3D_FEATURE_LEVEL_9_2:
					{
						//SM 2.0
						aeProfiles[ ShaderProfiles::VS_2_0 ] = CGprofile::CG_PROFILE_VS_2_0;
						aeProfiles[ ShaderProfiles::VS_2_X ] = CGprofile::CG_PROFILE_VS_2_X;
						aeProfiles[ ShaderProfiles::PS_2_0 ] = CGprofile::CG_PROFILE_PS_2_0;
						aeProfiles[ ShaderProfiles::PS_2_X ] = CGprofile::CG_PROFILE_PS_2_X;
						//SM 3.0
						aeProfiles[ ShaderProfiles::VS_3_0 ] = aeProfiles[ ShaderProfiles::VS_2_0 ];
						aeProfiles[ ShaderProfiles::PS_3_0 ] = aeProfiles[ ShaderProfiles::PS_2_0 ];
						//SM 4.0
						aeProfiles[ ShaderProfiles::VS_4_0 ] = aeProfiles[ ShaderProfiles::VS_2_0 ];
						aeProfiles[ ShaderProfiles::PS_4_0 ] = aeProfiles[ ShaderProfiles::PS_2_0 ];
						aeProfiles[ ShaderProfiles::GS_4_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::VS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::PS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						//SM 5.0
						aeProfiles[ ShaderProfiles::VS_5_0 ] = aeProfiles[ ShaderProfiles::VS_2_0 ];
						aeProfiles[ ShaderProfiles::PS_5_0 ] = aeProfiles[ ShaderProfiles::PS_2_0 ];
						aeProfiles[ ShaderProfiles::CS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::DS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::HS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;

						//Best
						aeProfiles[ ShaderProfiles::VS_BEST ] = aeProfiles[ ShaderProfiles::VS_2_0 ];
						aeProfiles[ ShaderProfiles::PS_BEST ] = aeProfiles[ ShaderProfiles::PS_2_0 ];
						aeProfiles[ ShaderProfiles::GS_BEST ] = aeProfiles[ ShaderProfiles::GS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::CS_BEST ] = aeProfiles[ ShaderProfiles::CS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::HS_BEST ] = aeProfiles[ ShaderProfiles::HS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::DS_BEST ] = aeProfiles[ ShaderProfiles::DS_UNKNOWN ];
					}
					break;

					case D3D_FEATURE_LEVEL_9_3:
					{
						//SM 2.0
						aeProfiles[ ShaderProfiles::VS_2_0 ] = CGprofile::CG_PROFILE_VS_2_0;
						aeProfiles[ ShaderProfiles::VS_2_X ] = CGprofile::CG_PROFILE_VS_2_X;
						aeProfiles[ ShaderProfiles::PS_2_0 ] = CGprofile::CG_PROFILE_PS_2_0;
						aeProfiles[ ShaderProfiles::PS_2_X ] = CGprofile::CG_PROFILE_PS_2_X;
						//SM 3.0
						aeProfiles[ ShaderProfiles::VS_3_0 ] = CGprofile::CG_PROFILE_VS_3_0;
						aeProfiles[ ShaderProfiles::PS_3_0 ] = CGprofile::CG_PROFILE_PS_3_0;
						//SM 4.0
						aeProfiles[ ShaderProfiles::VS_4_0 ] = aeProfiles[ ShaderProfiles::VS_3_0 ];
						aeProfiles[ ShaderProfiles::PS_4_0 ] = aeProfiles[ ShaderProfiles::PS_3_0 ];
						aeProfiles[ ShaderProfiles::GS_4_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::VS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::PS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						//SM 5.0
						aeProfiles[ ShaderProfiles::VS_5_0 ] = aeProfiles[ ShaderProfiles::VS_3_0 ];
						aeProfiles[ ShaderProfiles::PS_5_0 ] = aeProfiles[ ShaderProfiles::PS_3_0 ];
						aeProfiles[ ShaderProfiles::CS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::DS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::HS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;

						//Best
						aeProfiles[ ShaderProfiles::VS_BEST ] = aeProfiles[ ShaderProfiles::VS_3_0 ];
						aeProfiles[ ShaderProfiles::PS_BEST ] = aeProfiles[ ShaderProfiles::PS_3_0 ];
						aeProfiles[ ShaderProfiles::GS_BEST ] = aeProfiles[ ShaderProfiles::GS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::CS_BEST ] = aeProfiles[ ShaderProfiles::CS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::HS_BEST ] = aeProfiles[ ShaderProfiles::HS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::DS_BEST ] = aeProfiles[ ShaderProfiles::DS_UNKNOWN ];
					}
					break;

					case D3D_FEATURE_LEVEL_10_0:
					{
						//SM 2.0
						aeProfiles[ ShaderProfiles::VS_2_0 ] = CGprofile::CG_PROFILE_VS_2_0;
						aeProfiles[ ShaderProfiles::VS_2_X ] = CGprofile::CG_PROFILE_VS_2_X;
						aeProfiles[ ShaderProfiles::PS_2_0 ] = CGprofile::CG_PROFILE_PS_2_0;
						aeProfiles[ ShaderProfiles::PS_2_X ] = CGprofile::CG_PROFILE_PS_2_X;
						//SM 3.0
						aeProfiles[ ShaderProfiles::VS_3_0 ] = CG_PROFILE_VS_3_0;
						aeProfiles[ ShaderProfiles::PS_3_0 ] = CG_PROFILE_PS_3_0;
						//SM 4.0
						aeProfiles[ ShaderProfiles::VS_4_0 ] = CG_PROFILE_VS_4_0;
						aeProfiles[ ShaderProfiles::PS_4_0 ] = CG_PROFILE_PS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_0 ] = CG_PROFILE_GS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::VS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::PS_4_1 ] = CGprofile::CG_PROFILE_UNKNOWN;
						//SM 5.0
						aeProfiles[ ShaderProfiles::VS_5_0 ] = aeProfiles[ ShaderProfiles::VS_4_0 ];
						aeProfiles[ ShaderProfiles::PS_5_0 ] = aeProfiles[ ShaderProfiles::PS_4_0 ];
						aeProfiles[ ShaderProfiles::CS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_5_0 ] = aeProfiles[ ShaderProfiles::GS_4_0 ];
						aeProfiles[ ShaderProfiles::DS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::HS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;

						//Best
						aeProfiles[ ShaderProfiles::VS_BEST ] = aeProfiles[ ShaderProfiles::VS_4_0 ];
						aeProfiles[ ShaderProfiles::PS_BEST ] = aeProfiles[ ShaderProfiles::PS_4_0 ];
						aeProfiles[ ShaderProfiles::GS_BEST ] = aeProfiles[ ShaderProfiles::GS_4_0 ];
						aeProfiles[ ShaderProfiles::CS_BEST ] = aeProfiles[ ShaderProfiles::CS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::HS_BEST ] = aeProfiles[ ShaderProfiles::HS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::DS_BEST ] = aeProfiles[ ShaderProfiles::DS_UNKNOWN ];
					}
					break;

					case D3D_FEATURE_LEVEL_10_1:
					{
						//SM 2.0
						aeProfiles[ ShaderProfiles::VS_2_0 ] = CGprofile::CG_PROFILE_VS_2_0;
						aeProfiles[ ShaderProfiles::VS_2_X ] = CGprofile::CG_PROFILE_VS_2_X;
						aeProfiles[ ShaderProfiles::PS_2_0 ] = CGprofile::CG_PROFILE_PS_2_0;
						aeProfiles[ ShaderProfiles::PS_2_X ] = CGprofile::CG_PROFILE_PS_2_X;
						//SM 3.0
						aeProfiles[ ShaderProfiles::VS_3_0 ] = CG_PROFILE_VS_3_0;
						aeProfiles[ ShaderProfiles::PS_3_0 ] = CG_PROFILE_PS_3_0;
						//SM 4.0
						aeProfiles[ ShaderProfiles::VS_4_0 ] = CG_PROFILE_VS_4_0;
						aeProfiles[ ShaderProfiles::PS_4_0 ] = CG_PROFILE_PS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_0 ] = CG_PROFILE_GS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_1 ] = CG_PROFILE_GS_4_0;
						aeProfiles[ ShaderProfiles::VS_4_1 ] = CG_PROFILE_VS_4_0;
						aeProfiles[ ShaderProfiles::PS_4_1 ] = CG_PROFILE_PS_4_0;
						//SM 5.0
						aeProfiles[ ShaderProfiles::VS_5_0 ] = aeProfiles[ ShaderProfiles::VS_4_1 ];
						aeProfiles[ ShaderProfiles::PS_5_0 ] = aeProfiles[ ShaderProfiles::PS_4_1 ];
						aeProfiles[ ShaderProfiles::CS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_5_0 ] = aeProfiles[ ShaderProfiles::GS_4_1 ];
						aeProfiles[ ShaderProfiles::DS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::HS_5_0 ] = CGprofile::CG_PROFILE_UNKNOWN;

						//Best
						aeProfiles[ ShaderProfiles::VS_BEST ] = aeProfiles[ ShaderProfiles::VS_4_1 ];
						aeProfiles[ ShaderProfiles::PS_BEST ] = aeProfiles[ ShaderProfiles::PS_4_1 ];
						aeProfiles[ ShaderProfiles::GS_BEST ] = aeProfiles[ ShaderProfiles::GS_4_1 ];
						aeProfiles[ ShaderProfiles::CS_BEST ] = aeProfiles[ ShaderProfiles::CS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::HS_BEST ] = aeProfiles[ ShaderProfiles::HS_UNKNOWN ];
						aeProfiles[ ShaderProfiles::DS_BEST ] = aeProfiles[ ShaderProfiles::DS_UNKNOWN ];
					}
					break;

					case D3D_FEATURE_LEVEL_11_0:
					{
						//SM 2.0
						aeProfiles[ ShaderProfiles::VS_2_0 ] = CGprofile::CG_PROFILE_VS_2_0;
						aeProfiles[ ShaderProfiles::VS_2_X ] = CGprofile::CG_PROFILE_VS_2_X;
						aeProfiles[ ShaderProfiles::PS_2_0 ] = CGprofile::CG_PROFILE_PS_2_0;
						aeProfiles[ ShaderProfiles::PS_2_X ] = CGprofile::CG_PROFILE_PS_2_X;
						//SM 3.0
						aeProfiles[ ShaderProfiles::VS_3_0 ] = CG_PROFILE_VS_3_0;
						aeProfiles[ ShaderProfiles::PS_3_0 ] = CG_PROFILE_PS_3_0;
						//SM 4.0
						aeProfiles[ ShaderProfiles::VS_4_0 ] = CG_PROFILE_VS_4_0;
						aeProfiles[ ShaderProfiles::PS_4_0 ] = CG_PROFILE_PS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_0 ] = CG_PROFILE_GS_4_0;
						aeProfiles[ ShaderProfiles::GS_4_1 ] = CG_PROFILE_GS_4_0;
						aeProfiles[ ShaderProfiles::VS_4_1 ] = CG_PROFILE_VS_4_0;
						aeProfiles[ ShaderProfiles::PS_4_1 ] = CG_PROFILE_PS_4_0;
						//SM 5.0
						aeProfiles[ ShaderProfiles::VS_5_0 ] = CG_PROFILE_VS_5_0;
						aeProfiles[ ShaderProfiles::PS_5_0 ] = CG_PROFILE_PS_5_0;
						aeProfiles[ ShaderProfiles::CS_5_0 ] = CG_PROFILE_UNKNOWN;
						aeProfiles[ ShaderProfiles::GS_5_0 ] = CG_PROFILE_GS_5_0;
						aeProfiles[ ShaderProfiles::DS_5_0 ] = CG_PROFILE_DS_5_0;
						aeProfiles[ ShaderProfiles::HS_5_0 ] = CG_PROFILE_HS_5_0;

						//Best
						aeProfiles[ ShaderProfiles::VS_BEST ] = aeProfiles[ ShaderProfiles::VS_5_0 ];
						aeProfiles[ ShaderProfiles::PS_BEST ] = aeProfiles[ ShaderProfiles::PS_5_0 ];
						aeProfiles[ ShaderProfiles::GS_BEST ] = aeProfiles[ ShaderProfiles::GS_5_0 ];
						aeProfiles[ ShaderProfiles::CS_BEST ] = aeProfiles[ ShaderProfiles::CS_5_0 ];
						aeProfiles[ ShaderProfiles::HS_BEST ] = aeProfiles[ ShaderProfiles::HS_5_0 ];
						aeProfiles[ ShaderProfiles::DS_BEST ] = aeProfiles[ ShaderProfiles::DS_5_0 ];
					}
					break;
				};
			}
		}//cg

	}//d3d11
}//xse