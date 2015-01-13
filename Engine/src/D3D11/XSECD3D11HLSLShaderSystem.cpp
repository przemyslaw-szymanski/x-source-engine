#include "XSECD3D11HLSLShaderSystem.h"
#include "XSECD3D11VertexShader.h"
#include "XSECD3D11PixelShader.h"
#include "XSECD3D11RenderSystem.h"
#include "XSED3D11DefaultShaders.h"

namespace XSE
{
	namespace D3D11
	{
		static const u32 MATRIX4_SIZE = sizeof(DirectX::XMMATRIX);
		static const u32 FLOAT_SIZE = sizeof(f32);
		static const u32 FLOAT2_SIZE = sizeof(DirectX::XMFLOAT2);
		static const u32 FLOAT3_SIZE = sizeof(DirectX::XMFLOAT3);
		static const u32 FLOAT4_SIZE = sizeof(DirectX::XMFLOAT4);
		static const u32 INT_SIZE = sizeof(i32);
		static const u32 INT2_SIZE = sizeof(i32);

		// Offsets counted in floats
		struct ConstantOffsets
		{
			static const u32 MTX4 = 16;
			static const u32 FLOAT1 = 1;
			static const u32 FLOAT2 = 2;
			static const u32 FLOAT3 = 3;
			static const u32 FLOAT4 = 4;
			struct PerFrameVS
			{
				enum
				{
					MTX_VIEW = 0,
					MTX_PROJECTION = MTX_VIEW + MTX4,
					MTX_VIEW_PROJ = MTX_PROJECTION + MTX4,
					TIME = MTX_VIEW_PROJ + MTX4,
					LIGHT_POSITION = TIME + FLOAT1,
					LIGHT_SPECULAR = LIGHT_POSITION + FLOAT3,
					LIGHT_COLOR = LIGHT_SPECULAR + FLOAT1,
					SCENE_AMBIENT_COLOR = LIGHT_COLOR + FLOAT4,
					SCREEN_SIZE = SCENE_AMBIENT_COLOR + FLOAT4,
					CAMERA_POSITION = SCREEN_SIZE + FLOAT2,
					CAMERA_DIRECTION = CAMERA_POSITION + FLOAT3,
					_TOTAL_COUNT = CAMERA_DIRECTION + FLOAT3
				};
			};
			struct PerObjVS
			{
				enum
				{
					MTX_OBJ_WORLD = 0,
					MTX_OBJ_WORLD_VIEW_PROJECTION = MTX_OBJ_WORLD + MTX4,
					MTX_OBJ_WORLD_INVERSE_TRANSPOSE = MTX_OBJ_WORLD_VIEW_PROJECTION + MTX4,
					_TOTAL_COUNT = MTX_OBJ_WORLD_INVERSE_TRANSPOSE + FLOAT3
				};
			};
			struct PerFramePS
			{
				enum
				{
					TIME = 0,
					LIGHT_POSITION = TIME + FLOAT1,
					LIGHT_SPECULAR = LIGHT_POSITION + FLOAT3,
					LIGHT_COLOR = LIGHT_SPECULAR + FLOAT1,
					SCENE_AMBIENT_COLOR = LIGHT_COLOR + FLOAT4,
					SCREEN_SIZE = SCENE_AMBIENT_COLOR + FLOAT4,
					CAMERA_POSITION = SCREEN_SIZE + FLOAT2,
					CAMERA_DIRECTION = CAMERA_POSITION + FLOAT3,
					_TOTAL_COUNT = CAMERA_DIRECTION + FLOAT3
				};
			};
			struct PerObjPS
			{
				enum
				{
					_TOTAL_COUNT
				};
			};
		};

		CHLSLShaderSystem::SVSOncePerFrame	g_VSOncePerFrame;
		CHLSLShaderSystem::SVSOncePerObject	g_VSOncePerObj;
		CHLSLShaderSystem::SPSOncePerFrame	g_PSOncePerFrame;
		CHLSLShaderSystem::SPSOncePerObject	g_PSOncePerObj;

		D3D11_MAPPED_SUBRESOURCE				g_MappedSubresource;

		void SetProfiles(XST::xst_astr8* astrProfiles, ul32 ulSize, lpcastr lpszValue)
		{
			for(ul32 i = ulSize; i --> 0;)
			{
				astrProfiles[ i ] = lpszValue;
			}
		}


		namespace HLSL
		{
			static xst_astring g_strPerFrameVSCBuffer = "";
			static xst_astring g_strPerObjectVSCBuffer = "";
			static xst_astring g_strPerFramePSCBuffer = "";
			static xst_astring g_strPerObjectPSCBuffer = "";
			static xst_astring g_strConstantVUniforms = "";
			static xst_astring g_strConstantPUniforms = "";
			static xst_astring g_strShader = "";

			//XSE::D3D11::CRenderSystem::SMatrixBuffer g_ConstantMatrices;

			xst_astring& CreatePerFrameVSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				if( g_strPerFrameVSCBuffer.length() > 0 )
				{
					return g_strPerFrameVSCBuffer;
				}
				xst_stringstream ss;
				ss << "cbuffer cbPerFrame : register( b0 )" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_VIEW ]				<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_PROJECTION ]		<< ";" << xst_endl;
				ss << "\tfloat2 " << astrConstants[ ShaderConstants::SCREEN_SIZE ]			<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_DIRECTION ]		<< ";" << xst_endl;
				ss << "}" << xst_endl;
				g_strPerFrameVSCBuffer = ss.str();
				return g_strPerFrameVSCBuffer;
			}

			xst_astring& CreatePerObjectVSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				if( g_strPerObjectVSCBuffer.length() > 0 )
				{
					return g_strPerObjectVSCBuffer;
				}
				xst_stringstream ss;
				ss << "cbuffer cbPerObject : register( b1 )" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD ]					<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION ]	<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_INVERSE_TRANSPOSE ]	<< ";" << xst_endl;
				ss << "}" << xst_endl;
				g_strPerFrameVSCBuffer = ss.str();
				return g_strPerFrameVSCBuffer;
			}

			xst_astring& CreatePerFramePSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				if( g_strPerFramePSCBuffer.length() > 0 )
				{
					return g_strPerFramePSCBuffer;
				}
				xst_stringstream ss;
				ss << "cbuffer cbPerFrame : register( b0 )" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::LIGHT_COLOR ]		<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR ]		<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT_COLOR ]		<< ";" << xst_endl;
				ss << "\tfloat2 " << astrConstants[ ShaderConstants::SCREEN_SIZE ]			<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_DIRECTION ]		<< ";" << xst_endl;
				ss << "}" << xst_endl;
				g_strPerFramePSCBuffer = ss.str();
				return g_strPerFramePSCBuffer;
			}

			xst_astring& CreatePerObjectPSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				if( g_strPerObjectPSCBuffer.length() > 0 )
				{
					return g_strPerObjectPSCBuffer;
				}
				xst_stringstream ss;
				ss << "cbuffer cbPerObject : register( b1 )" << xst_endl;
				ss << "{" << xst_endl;
				//ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD ]					<< ";" << xst_endl;
				//ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION ]	<< ";" << xst_endl;
				//ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_INVERSE_TRANSPOSE ]	<< ";" << xst_endl;
				ss << "}" << xst_endl;
				g_strPerFramePSCBuffer = ss.str();
				return g_strPerFramePSCBuffer;
			}

			xst_astring& CreateVUniforms(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				xst_stringstream ss;
				/*ss << ",uniform float " << astrConstants[ ShaderConstants::TIME ] << xst_endl;
				ss << ",uniform float3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_DIFFUSE ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT ];*/

				g_strConstantVUniforms = ss.str();
				return g_strConstantVUniforms;
			}

			xst_astring& CreatePUniforms(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				xst_stringstream ss;
				/*ss << ",uniform float " << astrConstants[ ShaderConstants::TIME ] << xst_endl;
				ss << ",uniform float3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_DIFFUSE ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT ];*/

				g_strConstantPUniforms = ss.str();
				return g_strConstantPUniforms;
			}

			xst_astring& CreateVShader(xst_castring& strShader, xst_castring& strEntryPoint)
			{
				g_strShader.clear();
				//Set constant buffers
				g_strShader = g_strPerFrameVSCBuffer + "\n\n";
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
				g_strShader = g_strPerFramePSCBuffer + "\n\n";
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

			void SetProfiles(D3D_FEATURE_LEVEL eLevel, xst_astr16 astrProfiles[ ShaderProfiles::_ENUM_COUNT ]);

		}//HLSL

		CHLSLShaderSystem::CHLSLShaderSystem() : IShaderSystem( ShaderLanguages::HLSL ), m_pRS( xst_null )
		{
			
		}

		CHLSLShaderSystem::~CHLSLShaderSystem()
		{
		}

		void CHLSLShaderSystem::Destroy()
		{
			for(u32 i = ConstantBuffers::_ENUM_COUNT; i --> 0;)
			{
				xst_release( m_apD3DConstantBuffers[ i ] );
			}
		}

		i32 CHLSLShaderSystem::ApplyShaderConstantNames()
		{
			HLSL::CreatePerFrameVSCBuffer( this->CONSTANT_NAMES );
			HLSL::CreatePerFramePSCBuffer( this->CONSTANT_NAMES );
			HLSL::CreateVUniforms( this->CONSTANT_NAMES );
			HLSL::CreatePUniforms( this->CONSTANT_NAMES );
			return XST_OK;
		}

		i32 CHLSLShaderSystem::Init(IRenderSystem* pRS)
		{
			m_pRS = (CRenderSystem*)pRS;
			//Check options
			//Check feature level

			HLSL::SetProfiles( m_pRS->m_eFeatureLevel, m_astrProfiles );

			HLSL::g_strShader.reserve( 2048 );
			HLSL::g_strConstantVUniforms.reserve( 512 );
			HLSL::g_strConstantPUniforms.reserve( 512 );

			//Apply default names
			if( XST_FAILED( ApplyShaderConstantNames() ) )
			{
				return XST_FAIL;
			}

			// Create constant values each buffer MUST be multiple of 16bytes
			ul32 uBufferSize = 0;
			uBufferSize = ConstantOffsets::PerFrameVS::_TOTAL_COUNT + (16 - (ConstantOffsets::PerFrameVS::_TOTAL_COUNT % 16));
			m_vConstantValues[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ].resize( uBufferSize, 0 );
			uBufferSize = ConstantOffsets::PerObjVS::_TOTAL_COUNT + (16 - (ConstantOffsets::PerObjVS::_TOTAL_COUNT % 16));;
			m_vConstantValues[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ].resize( uBufferSize, 0 );
			uBufferSize = ConstantOffsets::PerFramePS::_TOTAL_COUNT + (16 - (ConstantOffsets::PerFramePS::_TOTAL_COUNT % 16));;
			m_vConstantValues[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ].resize( uBufferSize, 0 );
			uBufferSize = ConstantOffsets::PerObjPS::_TOTAL_COUNT + (16 - (ConstantOffsets::PerObjPS::_TOTAL_COUNT % 16));;
			m_vConstantValues[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ].resize( uBufferSize, 0 );

			m_vAllConstantValues.resize( ShaderConstants::_ENUM_COUNT );

			HRESULT hr;
			//Create constant buffers
			D3D11_BUFFER_DESC BufferDesc;
			xst_zero( &BufferDesc, sizeof( BufferDesc ) );
			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			
			//Per frame
			//BufferDesc.ByteWidth = sizeof( SVSOncePerFrame );
			BufferDesc.ByteWidth = m_vConstantValues[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ].size() * sizeof( f32 );
			hr = m_pRS->m_pDevice->CreateBuffer( &BufferDesc, xst_null, &m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Could not create per frame constant buffer: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			BufferDesc.ByteWidth = sizeof( SPSOncePerFrame );
			hr = m_pRS->m_pDevice->CreateBuffer( &BufferDesc, xst_null, &m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Could not create per object constant buffer: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			//Per object
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			BufferDesc.ByteWidth = sizeof( SVSOncePerObject );
			hr = m_pRS->m_pDevice->CreateBuffer( &BufferDesc, xst_null, &m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Could not create per object constant buffer: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			BufferDesc.ByteWidth = sizeof( SPSOncePerObject );
			hr = m_pRS->m_pDevice->CreateBuffer( &BufferDesc, xst_null, &m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ] );
			if( FAILED( hr ) )
			{
				XST_LOG_ERR( "[D3D11]: Could not create per object constant buffer: " << m_pRS->_ErrorToString( hr ) );
				return XST_FAIL;
			}

			//Shader codes
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_FRAME_VS_CBUFFER ] = HLSL::CreatePerFrameVSCBuffer( this->CONSTANT_NAMES );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_OBJECT_VS_CBUFFER ] = HLSL::CreatePerObjectVSCBuffer( this->CONSTANT_NAMES );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_FRAME_PS_CBUFFER ] = HLSL::CreatePerFramePSCBuffer( this->CONSTANT_NAMES );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_OBJECT_PS_CBUFFER ] = HLSL::CreatePerObjectPSCBuffer( this->CONSTANT_NAMES );

			return XST_OK;
		}

		void CHLSLShaderSystem::SetSceneAmbient(const XST::CColor& Color)
		{
			g_PSOncePerFrame.vecSceneAmbient = Color.ToVector4();
		}

		template<class _T_>
		xst_fi i32 UpdateConstant(u32 uConstantOffset, const _T_& tValue, CHLSLShaderSystem::FloatVec& vValues)
		{
			// If it is a standard constant defined by the engine
			cul32 uSize = sizeof( _T_ ) / 4;
			if( uConstantOffset + uSize < vValues.size() )
			{
				// Calc offset
				ul32 uOffset = uConstantOffset;
				//u32 uFloats = sizeof( _T_ ) / sizeof( f32 );
				xst_memcpy( &vValues[ uConstantOffset ], sizeof( _T_ ), &tValue, sizeof( _T_ ) );
				return XST_OK;
			}

			return XST_FAIL;
		}

		xst_fi i32 UpdateConstant(u32 uConstantOffset, const f32* pArray, u32 uCount, CHLSLShaderSystem::FloatVec& vValues)
		{
			// If it is a standard constant defined by the engine
			if( uConstantOffset + uCount < vValues.size() )
			{
				// Calc offset
				ul32 uOffset = uConstantOffset;
				cu32 uFloats = sizeof( f32 ) * uCount;
				for( u32 i = 0; i < uCount; ++i )
				{
					vValues[uConstantOffset + i] = pArray[ i ];
				}
				return XST_OK;
			}

			return XST_FAIL;
		}

		template<class _T_>
		xst_fi i32 UpdateConstant(u32 uConstantId, const _T_& tValue, CHLSLShaderSystem::ConstantValueVec& vValues, CHLSLShaderSystem::ConstantValueVec& vUserValues)
		{
			// If it is a standard constant defined by the engine
			if( uConstantId < vValues.size() )
			{
				xst_memcpy( &vValues[ uConstantId ], sizeof( _T_ ), &tValue, sizeof( _T_ ) );
				return XST_OK;
			}

			return XST_FAIL;
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const Mtx3& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const Mtx4& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const f32& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const Vec2& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const Vec3& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const Vec4& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const i32& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		i32	CHLSLShaderSystem::SetConstantValue(u32 uConstant, const bool& value)
		{
			return UpdateConstant( uConstant, value, m_vAllConstantValues, m_vUserConstantValues );
		}

		void CHLSLShaderSystem::UpdateFrameInputs()
		{
			m_pRS->GetMatrix( MatrixTypes::TRANSPOSED_PROJ,				&g_VSOncePerFrame.mtxProj );
			m_pRS->GetMatrix( MatrixTypes::TRANSPOSED_VIEW,				&g_VSOncePerFrame.mtxView );
			m_pRS->GetMatrix( MatrixTypes::TRANSPOSED_WORLD,			&g_VSOncePerFrame.mtxWorld );
			m_pRS->GetMatrix( MatrixTypes::TRANSPOSED_VIEW_PROJ,		&g_VSOncePerFrame.mtxViewProj );
			g_PSOncePerFrame.vecSceneAmbient = Vec4( 0.3f, 0.3f, 0.3f, 1.0f );
			g_VSOncePerFrame.vecScreenSize.x = (f32)m_pRS->GetOptions().uiResolutionWidth;
			g_VSOncePerFrame.vecScreenSize.y = (f32)m_pRS->GetOptions().uiResolutionHeight;
			
			FloatVec& vTmpVS = m_vConstantValues[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ];
			FloatVec& vTmpPS = m_vConstantValues[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ];
			cul32 uVSSize = vTmpVS.size() * sizeof( f32 );
			cul32 uPSSize = vTmpPS.size() * sizeof( f32 );

			// VERTEX
			UpdateConstant( ConstantOffsets::PerFrameVS::MTX_VIEW, g_VSOncePerFrame.mtxView, vTmpVS );
			UpdateConstant( ConstantOffsets::PerFrameVS::MTX_PROJECTION, g_VSOncePerFrame.mtxProj, vTmpVS );
			UpdateConstant( ConstantOffsets::PerFrameVS::MTX_VIEW_PROJ, g_VSOncePerFrame.mtxViewProj, vTmpVS );
			UpdateConstant( ConstantOffsets::PerFrameVS::SCREEN_SIZE, g_VSOncePerFrame.vecScreenSize, vTmpVS );

			// PIXEL
			UpdateConstant( ConstantOffsets::PerFramePS::LIGHT_POSITION, Vec3(1,0,0), vTmpPS );
			UpdateConstant( ConstantOffsets::PerFramePS::SCREEN_SIZE, g_VSOncePerFrame.vecScreenSize, vTmpPS );
			UpdateConstant( ConstantOffsets::PerFramePS::SCENE_AMBIENT_COLOR, m_vAllConstantValues[ ShaderConstants::SCENE_AMBIENT_COLOR ].float4, 4, vTmpPS );
			
			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			xst_memcpy( g_MappedSubresource.pData, uVSSize, &vTmpVS[0], uVSSize );
			//xst_memcpy( g_MappedSubresource.pData, sizeof( SVSOncePerFrame ), &g_VSOncePerFrame, sizeof( SVSOncePerFrame ) );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ], 0 );
			
			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			xst_memcpy( g_MappedSubresource.pData, uPSSize, &vTmpPS[0], uPSSize );
			//xst_memcpy( g_MappedSubresource.pData, sizeof( SPSOncePerFrame ), &g_VSOncePerFrame, sizeof( SPSOncePerFrame ) );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ], 0 );

			m_pRS->m_pDeviceContext->VSSetConstantBuffers( ConstantBuffers::CB_VS_ONCE_PER_FRAME, 1, &m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_FRAME ] );
			m_pRS->m_pDeviceContext->PSSetConstantBuffers( ConstantBuffers::CB_PS_ONCE_PER_FRAME, 1, &m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_FRAME ] );
		}

		static DirectX::XMMATRIX InverseTranspose( DirectX::CXMMATRIX M )
		{
			DirectX::XMMATRIX A = M;
			A.r[3] = DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f);
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
			return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
		}

		//MULTIPLE PER FRAME
		void CHLSLShaderSystem::UpdateObjectInputs()
		{
			FloatVec& vTmpVS = m_vConstantValues[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ];
			FloatVec& vTmpPS = m_vConstantValues[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ];
			cul32 uVSSize = vTmpVS.size() * sizeof( f32 );
			cul32 uPSSize = vTmpPS.size() * sizeof( f32 );

			//Get object input data
			DirectX::XMMATRIX mtxView, mtxProj;
			m_pRS->GetMatrix( MatrixTypes::WORLD,	&g_VSOncePerObj.mtxWorld );
			m_pRS->GetMatrix( MatrixTypes::PROJECTION,	&mtxProj );
			m_pRS->GetMatrix( MatrixTypes::VIEW,	&mtxView );
			g_VSOncePerObj.mtxWorldInvT = InverseTranspose( g_VSOncePerObj.mtxWorld );
			g_VSOncePerObj.mtxWorldViewProj = XMMatrixTranspose( XMMatrixMultiply( XMMatrixMultiply( g_VSOncePerObj.mtxWorld, mtxView ), mtxProj ) );

			// VERTEX
			UpdateConstant( ConstantOffsets::PerObjVS::MTX_OBJ_WORLD, g_VSOncePerObj.mtxWorld, vTmpVS );
			UpdateConstant( ConstantOffsets::PerObjVS::MTX_OBJ_WORLD_VIEW_PROJECTION, g_VSOncePerObj.mtxWorldViewProj, vTmpVS );
			UpdateConstant( ConstantOffsets::PerObjVS::MTX_OBJ_WORLD_INVERSE_TRANSPOSE, g_VSOncePerObj.mtxWorldInvT, vTmpVS );

			// PIXEL

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			//xst_memcpy( g_MappedSubresource.pData, sizeof( SVSOncePerObject ), &g_VSOncePerObj, sizeof( SVSOncePerObject ) );
			xst_memcpy( g_MappedSubresource.pData, uVSSize, &vTmpVS[0], uVSSize );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ], 0 );

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			//xst_memcpy( g_MappedSubresource.pData, uPSSize, &vTmpPS, uPSSize );
			xst_memcpy( g_MappedSubresource.pData, sizeof( SPSOncePerObject ), &g_PSOncePerObj, sizeof( SPSOncePerObject ) );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ], 0 );

			m_pRS->m_pDeviceContext->VSSetConstantBuffers( ConstantBuffers::CB_VS_ONCE_PER_OBJECT, 1, &m_apD3DConstantBuffers[ ConstantBuffers::CB_VS_ONCE_PER_OBJECT ] );
			m_pRS->m_pDeviceContext->PSSetConstantBuffers( ConstantBuffers::CB_PS_ONCE_PER_OBJECT, 1, &m_apD3DConstantBuffers[ ConstantBuffers::CB_PS_ONCE_PER_OBJECT ] );
		}

		IVertexShader*	CHLSLShaderSystem::CreateVertexShader(IInputLayout* pIL, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CVertexShader* pShader = xst_new CVertexShader( m_pRS, this, pIL, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			pShader->m_eShaderLanguage = ShaderLanguages::HLSL;
			return pShader;
		}

		IPixelShader*	CHLSLShaderSystem::CreatePixelShader(XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CPixelShader* pShader = xst_new CPixelShader( m_pRS, this, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			pShader->m_eShaderLanguage = ShaderLanguages::HLSL;
			return pShader;
		}

		IVertexShader*	CHLSLShaderSystem::CreateDefaultVertexShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			xst_assert( pOptions != xst_null, "(CRendersystem::CreateDefaultVertexShader) pOptions must be an non null created input layout" );
			IInputLayout* pIL = (IInputLayout*)pOptions;

			CVertexShader* pShader = xst_new CVertexShader( m_pRS, this, xst_null, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			pShader->m_eShaderLanguage = ShaderLanguages::HLSL;
			pShader->m_strShaderEntryPoint = "VS";
			pShader->m_eProfile = ShaderProfiles::VS_1_1;
			pShader->m_ulFlags = D3D10_SHADER_ENABLE_STRICTNESS;
			ul32 ulShaderLen = strlen( g_lpszDefault_MVP );

			if( XST_FAILED( m_pRS->_CompileShaderFromMemory( g_lpszDefault_MVP, ulShaderLen, m_astrProfiles[ pShader->m_eProfile ].data(), pShader ) ) )
			{
				DestroyVertexShader( pShader );
				return xst_null;
			}
			if( XST_FAILED( m_pRS->_CreateVertexShader( pShader ) ) )
			{
				DestroyVertexShader( pShader );
				return xst_null;
			}
			return pShader;
		}

		IPixelShader*	CHLSLShaderSystem::CreateDefaultPixelShader(xst_unknown pOptions, XSE::IResourceManager* pResourceMgr, cul32& ulHandle, xst_castring& strName, ci32& iType, ci32& iState, XST::IAllocator* pAllocator)
		{
			CPixelShader* pShader = xst_new CPixelShader( m_pRS, this, pResourceMgr, ulHandle, strName, iType, iState, pAllocator );
			pShader->m_eShaderLanguage = ShaderLanguages::HLSL;
			pShader->m_strShaderEntryPoint = "PS";
			pShader->m_eProfile = ShaderProfiles::PS_2_0;
			pShader->m_ulFlags = D3D10_SHADER_ENABLE_STRICTNESS;
			cul32 ulShaderLen = strlen( g_lpszDefault_MVP );

			if( XST_FAILED( m_pRS->_CompileShaderFromMemory( g_lpszDefault_MVP, ulShaderLen, m_astrProfiles[ pShader->m_eProfile ].data(), pShader ) ) )
			{
				DestroyPixelShader( pShader );
				xst_delete( pShader );
				return xst_null;
			}
			if( XST_FAILED( m_pRS->_CreatePixelShader( pShader ) ) )
			{
				DestroyPixelShader( pShader );
				xst_delete( pShader );
				return xst_null;
			}
			return pShader;
		}

		i32	CHLSLShaderSystem::CompileVertexShader(Resources::IVertexShader* pVS)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::CompileVertexShader)" );
			CVertexShader* pShader = (CVertexShader*)pVS;
			pShader->m_ulFlags = m_pRS->_GetShaderFlags();
			//Add default constants
			//xst_castring& strData = (lpcastr)pShader->GetResourceFile().GetPtr()->GetData().GetData();
			//xst_castring& strShader = HLSL::CreateVShader( strData, pVS->GetEntryPoint() );
			//XST_LOG_ERR( strShader );

			if( XST_FAILED( m_pRS->_CompileShader( pShader, m_astrProfiles[ pShader->GetProfile() ].data() ) ) )
			//if( XST_FAILED( m_pRS->_CompileShaderFromMemory( HLSL::g_strShader.data(), HLSL::g_strShader.length(), m_astrProfiles[ pShader->GetProfile() ].data(), pShader ) ) )
			{
				return XST_FAIL;
			}
			return m_pRS->_CreateVertexShader( pShader );
		}
		
		i32	CHLSLShaderSystem::CompilePixelShader(Resources::IPixelShader* pPS)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::CompilePixelShader)" );
			CPixelShader* pShader = (CPixelShader*)pPS;
			pShader->m_ulFlags = m_pRS->_GetShaderFlags();
			//Add default constants
			//xst_castring& strData = (lpcastr)pShader->GetResourceFile().GetPtr()->GetData().GetData();
			//xst_castring& strShader = HLSL::CreatePShader( strData, pPS->GetEntryPoint() );

			if( XST_FAILED( m_pRS->_CompileShader( pShader, m_astrProfiles[ pShader->GetProfile() ].data() ) ) )
			//if( XST_FAILED( m_pRS->_CompileShaderFromMemory( HLSL::g_strShader.data(), HLSL::g_strShader.length(), m_astrProfiles[ pShader->GetProfile() ].data(), pShader ) ) )
			{
				return XST_FAIL;
			}
			return m_pRS->_CreatePixelShader( pShader );
		}

		i32	CHLSLShaderSystem::CompileVertexShader(Resources::IVertexShader* pVS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::CompileVertexShader)" );
			CVertexShader* pShader = (CVertexShader*)pVS;
			pShader->m_ulFlags = m_pRS->_GetShaderFlags();
			if( XST_FAILED( m_pRS->_CompileShaderFromMemory(	lpszShader, ulShaderSize, pVS->GetResourceName().data(), 
																xst_null, xst_null, lpszEntryPoint, m_astrProfiles[ eProfile ].data(),
																pShader->m_ulFlags, 0, xst_null, &pShader->m_pBlob, xst_null ) ) )
			{
				return XST_FAIL;
			}
			return m_pRS->_CreateVertexShader( pShader );
		}

		i32	CHLSLShaderSystem::CompilePixelShader(Resources::IPixelShader* pPS, lpcastr lpszShader, ul32 ulShaderSize, lpcastr lpszEntryPoint, SHADER_PROFILE eProfile)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::CompilePixelShader)" );
			CPixelShader* pShader = (CPixelShader*)pPS;
			pShader->m_ulFlags = m_pRS->_GetShaderFlags();
			if( XST_FAILED( m_pRS->_CompileShaderFromMemory(	lpszShader, ulShaderSize, pPS->GetResourceName().data(), 
																xst_null, xst_null, lpszEntryPoint, m_astrProfiles[ eProfile ].data(),
																pShader->m_ulFlags, 0, xst_null, &pShader->m_pBlob, xst_null ) ) )
			{
				return XST_FAIL;
			}
			return m_pRS->_CreatePixelShader( pShader );
		}


		void CHLSLShaderSystem::DestroyVertexShader(Resources::IVertexShader* pVS)
		{
			CVertexShader* pShader = (CVertexShader*)pVS;
			xst_release( pShader->m_pD3DVertexShader );
			xst_release( pShader->m_pBlob );
		}

		void CHLSLShaderSystem::DestroyPixelShader(Resources::IPixelShader* pPS)
		{
			CPixelShader* pShader = (CPixelShader*)pPS;
			xst_release( pShader->m_pD3DPixelShader );
			xst_release( pShader->m_pBlob );
		}

		i32	CHLSLShaderSystem::SetVertexShader(const Resources::IVertexShader* pVS)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::SetVertexShader)" );
			//return m_pRS->SetVertexShader( pVS );
			const CVertexShader* pShader = (CVertexShader*)pVS;
			if( pShader->m_pD3DVertexShader )
			{
				m_pRS->m_pDeviceContext->VSSetShader( pShader->m_pD3DVertexShader, xst_null, 0 );
			}
			else
			{
				return XST_FAIL;
			}
			return XST_OK;
		}

		i32 CHLSLShaderSystem::SetPixelShader(const Resources::IPixelShader* pPS)
		{
			xst_assert( m_pRS, "(CHLSLShaderSystem::SetPixelShader)" );
			xst_assert( pPS, "(CHLSLShaderSystem::SetPixelShader)" );

			//return m_pRS->SetPixelShader( pPS );
			const CPixelShader* pShader = (CPixelShader*)pPS;
			if( pShader->m_pD3DPixelShader )
			{
				m_pRS->m_pDeviceContext->PSSetShader( pShader->m_pD3DPixelShader, xst_null, 0 );
			}
			else
			{
				return XST_FAIL;
			}
			return XST_OK;
		}

		i32 CHLSLShaderSystem::_AddConstantBuffers(IShader** ppShader, xst_castring& strPerFrame, xst_castring& strPerObject)
		{
			xst_assert2( (*ppShader) );
			IShader* pShader = (*ppShader);
			FilePtr pFile = pShader->GetResourceFile();
			TCData<u8>& Data = pFile->GetData();
			xst_astring strCode;
			strCode.reserve( 10000 );
			strCode += strPerFrame;
			strCode += strPerObject;
			strCode.append( (lpcastr)Data.GetPointer(), Data.GetSize() );
			Data.Copy( (u8*)strCode.c_str(), strCode.length(), true );
			//strCode = (lpcastr)Data.GetData();
			return XST_OK;
		}

		i32	CHLSLShaderSystem::PrepareResource(Resources::IResource* pRes)
		{
			IShader* pShader = (IShader*)pRes;

			switch( pShader->GetShaderType() )
			{
				case ShaderTypes::VERTEX:
				{
					_AddConstantBuffers( &pShader, 
										 this->GetShaderCode( ShaderCodes::PER_FRAME_VS_CBUFFER ),
										 this->GetShaderCode( ShaderCodes::PER_OBJECT_VS_CBUFFER ));
					CVertexShader* pVShader = (CVertexShader*)pShader;
					return this->CompileVertexShader( pVShader );
				}
				break;

				case ShaderTypes::PIXEL:
				{
					_AddConstantBuffers( &pShader, 
										 this->GetShaderCode( ShaderCodes::PER_FRAME_PS_CBUFFER ),
										 this->GetShaderCode( ShaderCodes::PER_OBJECT_PS_CBUFFER ));
					CPixelShader* pPShader = (CPixelShader*)pShader;
					return this->CompilePixelShader( pPShader );
				}
				break;
			}

			XST_LOG_ERR( "Invalid shader type" );
			return XST_FAIL;
		}

		xst_astring	CHLSLShaderSystem::CreateShaderCode(u32 uiVSInput, u32 uiPSInput)
		{
			xst_astring strCode;
			strCode.reserve( 1024 );

			strCode += GetShaderCode( ShaderCodes::PER_FRAME_VS_CBUFFER ) + xst_endl;
			strCode += GetShaderCode( ShaderCodes::PER_OBJECT_VS_CBUFFER ) + xst_endl;

			//Vertex shader
			xst_stringstream ssVS;
			ssVS<< "struct VS_INPUT{" << xst_endl
				<< "	float4 pos : POSITION;" << xst_endl;

			if( uiVSInput & ILEs::COLOR )
			{
				ssVS << "	float4 color : COLOR0;" << xst_endl;
			}

			ssVS<< "};" << xst_endl
				<< "struct PS_INPUT{" << xst_endl
				<< "	float4 pos : SV_POSITION;" << xst_endl;

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
			ssPS<< "float4 ps_main(PS_INPUT In) : SV_TARGET" << xst_endl
				<< "{" << xst_endl
				<< "	return In.color;" << xst_endl
				<< "}";

			strCode += ssPS.str();

			return strCode;
		}

		//void CHLSLShaderSystem::SetConstantBuffers()
		//{
		//	xst_assert( m_pRS, "(CHLSLShaderSystem::SetConstantBuffers)" );
		//	//Set constant buffer
		//	m_pRS->GetTransposedWorldMatrix( &HLSL::g_ConstantMatrices.mtxWorld );
		//	m_pRS->GetTransposedViewMatrix( &HLSL::g_ConstantMatrices.mtxView );
		//	m_pRS->GetTransposedProjectionMatrix( &HLSL::g_ConstantMatrices.mtxProj );
		//	m_pRS->GetTransposedViewProjectionMatrix( &HLSL::g_ConstantMatrices.mtxViewProj );
		//	m_pRS->GetTransposedWorldViewProjectionMatrix( &HLSL::g_ConstantMatrices.mtxWorldViewProj );

		//	m_pRS->m_pDeviceContext->UpdateSubresource( m_pRS->m_pD3DMtxConstantBuffer, 0, xst_null, &HLSL::g_ConstantMatrices, 0, 0 );
		//	m_pRS->m_pDeviceContext->VSSetConstantBuffers( 0, 1, &m_pRS->m_pD3DMtxConstantBuffer );
		//}

		namespace HLSL
		{
			void SetProfiles(D3D_FEATURE_LEVEL eLevel, xst_astr16 astrProfiles[ ShaderProfiles::_ENUM_COUNT ])
			{
				//SetProfiles( astrProfiles, ShaderProfiles::_ENUM_COUNT, "" );
				//memset( astrProfiles, 0, sizeof( xst_astr16 ) * ShaderProfiles::_ENUM_COUNT );
				xst_zero( astrProfiles, sizeof( xst_astr16 ) * ShaderProfiles::_ENUM_COUNT );
				//Do not support earlier version than DirectX9
				//SM 1.0
				astrProfiles[ ShaderProfiles::VS_1_1 ] = "vs_4_0_level_9_1";
			
				if( eLevel == D3D_FEATURE_LEVEL_9_1 || eLevel == D3D_FEATURE_LEVEL_9_2 )
				{
					//SM 2.0
					astrProfiles[ ShaderProfiles::VS_2_0 ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_0 ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_2_X ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_2_X ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					astrProfiles[ ShaderProfiles::VS_3_0 ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_3_0 ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					astrProfiles[ ShaderProfiles::VS_4_0 ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_4_0 ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					astrProfiles[ ShaderProfiles::GS_4_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_4_1 ] = "";
					astrProfiles[ ShaderProfiles::VS_4_1 ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_4_1 ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					//SM 5.0
					astrProfiles[ ShaderProfiles::VS_5_0 ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_5_0 ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					astrProfiles[ ShaderProfiles::CS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::DS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::HS_5_0 ] = "";

					//Best
					astrProfiles[ ShaderProfiles::VS_BEST ] = astrProfiles[ ShaderProfiles::VS_2_0 ];
					astrProfiles[ ShaderProfiles::PS_BEST ] = astrProfiles[ ShaderProfiles::PS_2_0 ];
					astrProfiles[ ShaderProfiles::GS_BEST ] = astrProfiles[ ShaderProfiles::GS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::HS_BEST ] = astrProfiles[ ShaderProfiles::HS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::DS_BEST ] = astrProfiles[ ShaderProfiles::DS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::CS_BEST ] = astrProfiles[ ShaderProfiles::CS_UNKNOWN ];
				}
				else
				if( eLevel == D3D_FEATURE_LEVEL_9_3 )
				{
					//SM 3.0
					astrProfiles[ ShaderProfiles::VS_2_0 ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_0 ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_2_X ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_X ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_3_0 ] = "vs_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::PS_3_0 ] = "ps_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::VS_4_0 ] = astrProfiles[ ShaderProfiles::VS_3_0 ];
					astrProfiles[ ShaderProfiles::PS_4_0 ] = astrProfiles[ ShaderProfiles::PS_3_0 ];
					astrProfiles[ ShaderProfiles::GS_4_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_4_1 ] = "";
					astrProfiles[ ShaderProfiles::VS_4_1 ] = astrProfiles[ ShaderProfiles::VS_3_0 ];
					astrProfiles[ ShaderProfiles::PS_4_1 ] = astrProfiles[ ShaderProfiles::PS_3_0 ];
					//SM 5.0
					astrProfiles[ ShaderProfiles::VS_5_0 ] = astrProfiles[ ShaderProfiles::VS_3_0 ];
					astrProfiles[ ShaderProfiles::PS_5_0 ] = astrProfiles[ ShaderProfiles::PS_3_0 ];
					astrProfiles[ ShaderProfiles::CS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::DS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::HS_5_0 ] = "";

					//Best
					astrProfiles[ ShaderProfiles::VS_BEST ] = astrProfiles[ ShaderProfiles::VS_3_0 ];
					astrProfiles[ ShaderProfiles::PS_BEST ] = astrProfiles[ ShaderProfiles::PS_3_0 ];
					astrProfiles[ ShaderProfiles::GS_BEST ] = astrProfiles[ ShaderProfiles::GS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::HS_BEST ] = astrProfiles[ ShaderProfiles::HS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::DS_BEST ] = astrProfiles[ ShaderProfiles::DS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::CS_BEST ] = astrProfiles[ ShaderProfiles::CS_UNKNOWN ];
				}
				else
				if( eLevel == D3D_FEATURE_LEVEL_10_0 )
				{
					//SM 4.0
					astrProfiles[ ShaderProfiles::VS_2_0 ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_0 ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_2_X ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_X ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_3_0 ] = "vs_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::PS_3_0 ] = "ps_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::VS_4_0 ] = "vs_4_0";
					astrProfiles[ ShaderProfiles::PS_4_0 ] = "ps_4_0";
					astrProfiles[ ShaderProfiles::GS_4_0 ] = "gs_4_0";
					astrProfiles[ ShaderProfiles::GS_4_1 ] = astrProfiles[ ShaderProfiles::GS_4_0 ];
					astrProfiles[ ShaderProfiles::VS_4_1 ] = astrProfiles[ ShaderProfiles::VS_4_0 ];
					astrProfiles[ ShaderProfiles::PS_4_1 ] = astrProfiles[ ShaderProfiles::PS_4_0 ];
					//SM 5.0
					astrProfiles[ ShaderProfiles::VS_5_0 ] = astrProfiles[ ShaderProfiles::VS_4_0 ];
					astrProfiles[ ShaderProfiles::PS_5_0 ] = astrProfiles[ ShaderProfiles::PS_4_0 ];
					astrProfiles[ ShaderProfiles::CS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_5_0 ] = astrProfiles[ ShaderProfiles::GS_4_0 ];
					astrProfiles[ ShaderProfiles::DS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::HS_5_0 ] = "";

					//Best
					astrProfiles[ ShaderProfiles::VS_BEST ] = astrProfiles[ ShaderProfiles::VS_4_0 ];
					astrProfiles[ ShaderProfiles::PS_BEST ] = astrProfiles[ ShaderProfiles::PS_4_0 ];
					astrProfiles[ ShaderProfiles::GS_BEST ] = astrProfiles[ ShaderProfiles::GS_4_0 ];
					astrProfiles[ ShaderProfiles::HS_BEST ] = astrProfiles[ ShaderProfiles::HS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::DS_BEST ] = astrProfiles[ ShaderProfiles::DS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::CS_BEST ] = astrProfiles[ ShaderProfiles::CS_UNKNOWN ];
					
				}
				else
				if( eLevel == D3D_FEATURE_LEVEL_10_1 )
				{
					//SM 4.0 extended
					astrProfiles[ ShaderProfiles::VS_2_0 ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_0 ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_2_X ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_X ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_3_0 ] = "vs_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::PS_3_0 ] = "ps_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::VS_4_0 ] = "vs_4_0";
					astrProfiles[ ShaderProfiles::PS_4_0 ] = "ps_4_0";
					astrProfiles[ ShaderProfiles::GS_4_0 ] = "gs_4_0";
					astrProfiles[ ShaderProfiles::GS_4_1 ] = "gs_4_1";
					astrProfiles[ ShaderProfiles::VS_4_1 ] = "vs_4_1";
					astrProfiles[ ShaderProfiles::PS_4_1 ] = "ps_4_1";
					//SM 5.0
					astrProfiles[ ShaderProfiles::VS_5_0 ] = astrProfiles[ ShaderProfiles::VS_4_1 ];
					astrProfiles[ ShaderProfiles::PS_5_0 ] = astrProfiles[ ShaderProfiles::PS_4_1 ];
					astrProfiles[ ShaderProfiles::CS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::GS_5_0 ] = astrProfiles[ ShaderProfiles::GS_4_1 ];
					astrProfiles[ ShaderProfiles::DS_5_0 ] = "";
					astrProfiles[ ShaderProfiles::HS_5_0 ] = "";

					//Best
					astrProfiles[ ShaderProfiles::VS_BEST ] = astrProfiles[ ShaderProfiles::VS_4_1 ];
					astrProfiles[ ShaderProfiles::PS_BEST ] = astrProfiles[ ShaderProfiles::PS_4_1 ];
					astrProfiles[ ShaderProfiles::GS_BEST ] = astrProfiles[ ShaderProfiles::GS_4_1 ];
					astrProfiles[ ShaderProfiles::HS_BEST ] = astrProfiles[ ShaderProfiles::HS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::DS_BEST ] = astrProfiles[ ShaderProfiles::DS_UNKNOWN ];
					astrProfiles[ ShaderProfiles::CS_BEST ] = astrProfiles[ ShaderProfiles::CS_UNKNOWN ];
				}
				else
				if( eLevel == D3D_FEATURE_LEVEL_11_0 )
				{
					astrProfiles[ ShaderProfiles::VS_2_0 ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_0 ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_2_X ] = "vs_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::PS_2_X ] = "ps_4_0_level_9_1";
					astrProfiles[ ShaderProfiles::VS_3_0 ] = "vs_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::PS_3_0 ] = "ps_4_0_level_9_3";
					astrProfiles[ ShaderProfiles::VS_4_0 ] = "vs_4_0";
					astrProfiles[ ShaderProfiles::PS_4_0 ] = "ps_4_0";
					astrProfiles[ ShaderProfiles::GS_4_0 ] = "gs_4_0";
					astrProfiles[ ShaderProfiles::GS_4_1 ] = "gs_4_1";
					astrProfiles[ ShaderProfiles::VS_4_1 ] = "vs_4_1";
					astrProfiles[ ShaderProfiles::PS_4_1 ] = "ps_4_1";
					//SM 5.0
					astrProfiles[ ShaderProfiles::VS_5_0 ] = "vs_5_0";
					astrProfiles[ ShaderProfiles::PS_5_0 ] = "ps_5_0";
					astrProfiles[ ShaderProfiles::CS_5_0 ] = "cs_5_0";
					astrProfiles[ ShaderProfiles::GS_5_0 ] = "gs_5_0";
					astrProfiles[ ShaderProfiles::DS_5_0 ] = "ds_5_0";
					astrProfiles[ ShaderProfiles::HS_5_0 ] = "hs_5_0";

					//Best
					astrProfiles[ ShaderProfiles::VS_BEST ] = astrProfiles[ ShaderProfiles::VS_5_0 ];
					astrProfiles[ ShaderProfiles::PS_BEST ] = astrProfiles[ ShaderProfiles::PS_5_0 ];
					astrProfiles[ ShaderProfiles::GS_BEST ] = astrProfiles[ ShaderProfiles::GS_5_0 ];
					astrProfiles[ ShaderProfiles::HS_BEST ] = astrProfiles[ ShaderProfiles::HS_5_0 ];
					astrProfiles[ ShaderProfiles::DS_BEST ] = astrProfiles[ ShaderProfiles::DS_5_0 ];
					astrProfiles[ ShaderProfiles::CS_BEST ] = astrProfiles[ ShaderProfiles::CS_5_0 ];
				}
			}

			

		}//hlsl

	}//d3d11
}//xse