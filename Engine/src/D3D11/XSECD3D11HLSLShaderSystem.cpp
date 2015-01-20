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

		class CCBBuilder
		{
			public:

			enum TYPE
			{
				FLOAT1		= 1,
				FLOAT2		= 2,
				FLOAT3		= 3,
				FLOAT4		= 4,
				MATRIX3		= 3*4,
				MATRIX4		= 4*4,
				_ENUM_COUNT
			};

			struct SCB
			{
				TYPE	eType; // type and size in floats
				u32		uConstant; // constant enum type/name
				xst_astring strName; // name in constant buffer
				SCB* aCBs;
				u32 uCBCount;
				/*void operator=( const SCB& other )
				{
					eType = other.eType;
					uConstant = other.uConstant;
					strName = other.strName;

				}*/
			};

			void Add(TYPE eType, xst_castring& strName, u32 uConstant)
			{
				SCB CB = { eType, uConstant, strName, xst_null, 0 };
				vShaderCB.push_back( CB );
			}

			void Add(TYPE eType, xst_castring& strName, u32 uConstant, SCB* aCBs, u32 uCBCount)
			{
				SCB CB = { eType, uConstant, strName, aCBs, uCBCount };
				vShaderCB.push_back( CB );
			}

			template<class _T_>
			void SetConstant(u32 uConst, const _T_& v)
			{
				cu32 uSize = sizeof(_T_) / 4;
				cu32 uOffset = vOffsets[ uConst ];
				xst_assert2( uOffset + sizeof(_T_)/4 <= vData.size() );
				xst_memcpy( &vData[uOffset], sizeof(_T_), &v, sizeof(_T_) );
			}

			template<class _T_>
			void SetConstant(u32 uConst, const _T_& v, u32 uSize)
			{

			}

			void _AddName( TYPE eType, xst_castring& strName, TYPE ePrevType, bool bSimplePadding, u32* puPadding, u32* puRegOffset, u32* puBoundOffset )
			{
				u32 uPadding = 0;
				u32 uRegOffset = *puRegOffset;
				u32 uStartBounds = *puBoundOffset;
				u32 uNextOffset = 0;
				u32 uNextBoundOffset = 0;
				cu32 uRegisterRange = eType / FLOAT4;
				static const char aBounds[] = { 'x', 'y', 'z', 'w' };
				char strBounds[ 5 ] = {0,0,0,0,0};
				char offBuff[ 32 ];
				
				if( !bSimplePadding )
				{
					if( uRegisterRange == 0 ) // if something smaller than float4
					{
						// Check if in current register are any free bounds
						u32 uRange = uStartBounds + eType;
						if( uRange <= 4 ) // there is a place
						{
							// It must be a float1, float2, float3 ro float4
							u32 j = 0;
							strBounds[ 0 ] = strBounds[ 1 ] = strBounds[ 2 ] = strBounds[ 3 ] = 0;
							for( u32 i = uStartBounds; i < uRange; ++i, ++j )
							{
								strBounds[ j ] = aBounds[ i ];
							}
							uStartBounds += eType;
						}
						else // if not fits to current register create a new one
						{
							uStartBounds = eType;
							uRegOffset += 1;
							for( u32 i = 0; i < eType; ++i )
							{
								strBounds[ i ] = aBounds[ i ];
							}
						}
					}
					else
					{
						uRegOffset += uRegisterRange;
						uStartBounds = 0;
						strBounds[ 0 ] = 'x'; strBounds[ 1 ] = 'y'; strBounds[ 2 ] = 'z'; strBounds[ 3 ] = 'w';
					}

					if( ePrevType == (TYPE)0 )
						uRegOffset = 0;
					xst_sprintf( offBuff, 32, "packoffset(c%d.%s)", uRegOffset, (lpcastr) strBounds );
				}
				else
				{
					uRegOffset += std::max( 1, ePrevType / FLOAT4 );
					if( ePrevType == (TYPE) 0 )
						uRegOffset = 0;
					xst_sprintf( offBuff, 32, "packoffset(c%d)", uRegOffset );
				}

				switch( eType )
				{
					case FLOAT1:
						m_ss << "\tfloat1 " << strName << " : " << offBuff << ";" << xst_endl;
						uPadding = bSimplePadding? 3 : 0;
					break;
					case FLOAT2:
						m_ss << "\tfloat2 " << strName << " :" << offBuff << ";" << xst_endl;
						uPadding = bSimplePadding? 2 : 0;
					break;
					case FLOAT3:
						m_ss << "\tfloat3 " << strName << " : " << offBuff << ";" << xst_endl;
						uPadding = 1;
					break;
					case FLOAT4:
						m_ss << "\tfloat4 " << strName << " : " << offBuff << ";" << xst_endl;
					break;
					case MATRIX3:
						m_ss << "\matrix " << strName << " : " << offBuff << ";" << xst_endl;
					break;
					case MATRIX4:
						m_ss << "\tmatrix " << strName << " : " << offBuff << ";" << xst_endl;
					break;
				}
				
				(*puPadding) = uPadding;
				(*puRegOffset) = uRegOffset;
				(*puBoundOffset) = uStartBounds;
			}

			// TODO: Use packoffset to save memory for smaller than float4 variables
			xst_astring Build(xst_castring& strCBName, int iRegister)
			{
				vOffsets.resize( ShaderConstants::_ENUM_COUNT + 10, 0 );
				uRegister = iRegister;
				std::sort( vShaderCB.begin(), vShaderCB.end(), [ &] ( const SCB& l, const SCB& r ){ return l.eType < r.eType; } );
				m_ss.clear();
				m_ss << "cbuffer " << strCBName << " : register(b" << iRegister << ")" << xst_endl << "{" << xst_endl;
				u32 uOffset = 0;
				for( auto& CB : vShaderCB )
				{
					u32 uPadding = 0;
					if( CB.aCBs && CB.uCBCount )
					{
						m_ss << "struct " << CB.strName << "{" << xst_endl;
						for( u32 i = 0; i < CB.uCBCount; ++i )
						{
							/*_AddName( CB.aCBs[i].eType, CB.aCBs[i].strName, m_eLastType, &uPadding, &m_uLastRegOffset, &m_uLastBoundOffset );
							vOffsets[ CB.aCBs[i].uConstant ] = uOffset;
							uOffset += (u32)CB.aCBs[i].eType + uPadding;	*/
						}
						m_ss << "}" << xst_endl;
					}
					else
					{
						_AddName( CB.eType, CB.strName, m_eLastType, true, &uPadding, &m_uLastRegOffset, &m_uLastBoundOffset );
						vOffsets[ CB.uConstant ] = uOffset;
						uOffset += (u32)CB.eType + uPadding;
						m_eLastType = CB.eType;
					}
				}
				m_ss << "}" << xst_endl;
				u32 uMod = uOffset % 16;
				if( uMod != 0 )
					uOffset += 16 - uMod;
				vData.resize( uOffset );
				uSizeInBytes = vData.size() * sizeof( f32 );
				return m_ss.str();
			}

				xst_vector<u32>	vOffsets;
				xst_vector<SCB>	vShaderCB;
				xst_vector<f32> vData;
				ul32 uSizeInBytes;
				u32 uRegister;
			
			protected:
				xst_stringstream m_ss;
				u32 m_uLastRegOffset = 0; // c0, c1, etc
				u32 m_uLastBoundOffset = 0; // x=1,y=2,z=3,w=4
				TYPE m_eLastType = (TYPE)0;
		};

		CCBBuilder g_aCBBuilders[ ConstantBuffers::_ENUM_COUNT ];
		
		XST_ALIGN(16) struct SVSOncePerFrame
		{
			DirectX::XMMATRIX	mtxView;
			DirectX::XMMATRIX	mtxProj;
			DirectX::XMMATRIX	mtxViewProj;
			Vec3				vecCamPos;
			f32					fTime;
			Vec3				vecCamDir;
			f32					padding0;
			Vec2				vecScreenSize;
		};

		XST_ALIGN(16) struct SPSOncePerFrame
		{
			Vec4		vecLightColor;
			Vec4		vecSceneAmbient;
			Vec4		vecLightSpecularColor;
			Vec3		vecLightPos;
			f32			fTime;
			Vec3		vecCamPos;
			f32			fLightSpecularPower;
			Vec3		vecCamDir;
			f32			fLightPower;
			Vec2		vecScreenSize;
		};

		XST_ALIGN( 16 ) struct PSOncePerMaterial
		{
			Vec4	vecAmbientColor;
			Vec4	vecDiffuseColor;
			Vec4	vecSpecularColor;
			f32		fShininess;
			f32		fAlpha;
		};

		XST_ALIGN(16) struct SVSOncePerObject
		{
			DirectX::XMMATRIX	mtxWorld;
			DirectX::XMMATRIX	mtxWorldViewProj;
			DirectX::XMMATRIX	mtxWorldInvT; // world inverse transpose
		};

		XST_ALIGN(16) struct SPSOncePerObject
		{
			Vec4 vecTmp;
			Vec4 vecTmp2;
		};
		
		SVSOncePerFrame		g_VSOncePerFrame;
		SVSOncePerObject	g_VSOncePerObj;
		SPSOncePerFrame		g_PSOncePerFrame;
		SPSOncePerObject	g_PSOncePerObj;

		SVSOncePerFrame*	g_pOncePerFrameVS = xst_null;
		SVSOncePerObject*	g_pOncePerObjVS = xst_null;
		SPSOncePerFrame*	g_pOncePerFramePS = xst_null;
		SPSOncePerObject*	g_pOncePerObjPS = xst_null;
		PSOncePerMaterial*	g_pOncePerMatPS = xst_null;

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
			static xst_astring g_strPerMaterialPSCBufer = "";
			static xst_astring g_strPerObjectPSCBuffer = "";
			static xst_astring g_strConstantVUniforms = "";
			static xst_astring g_strConstantPUniforms = "";
			static xst_astring g_strShader = "";
			static xst_astring g_astrCBufferCodes[ ConstantBuffers::_ENUM_COUNT ] = { "" };

			//XSE::D3D11::CRenderSystem::SMatrixBuffer g_ConstantMatrices;

			xst_fi void AddConstant(CCBBuilder* pOut, CCBBuilder::TYPE eType, u32 uConstant, xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				pOut->Add( eType, astrConstants[ uConstant ], uConstant );
			}

			xst_castring& CreatePerFrameVSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ], u32 uRegister)
			{
				auto& str = g_astrCBufferCodes[ ConstantBuffers::VS_ONCE_PER_FRAME ];
				if( !str.empty() )
					return str;
				/*
				DirectX::XMMATRIX	mtxView;
			DirectX::XMMATRIX	mtxProj;
			DirectX::XMMATRIX	mtxViewProj;
			Vec3				vecCamPos;
			f32					fTime;
			Vec3				vecCamDir;
			f32					padding0;
			Vec2				vecScreenSize;
				*/
				xst_stringstream ss;
				ss << "cbuffer cbFrame : register(b" << uRegister << ")" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_VIEW ]				<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_PROJECTION ]		<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_VIEW_PROJ ]		<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat "  << astrConstants[ ShaderConstants::TIME ]					<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_DIRECTION ]		<< ";" << xst_endl;
				ss << "\tfloat2 " << astrConstants[ ShaderConstants::SCREEN_SIZE ]			<< ";" << xst_endl;
				ss << "}" << xst_endl;
				str = ss.str();
				return str;
			}

			xst_castring& CreatePerDrawcallVSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ], u32 uRegister)
			{
				auto& str = g_astrCBufferCodes[ ConstantBuffers::VS_ONCE_PER_DRAWCALL ];
				if( !str.empty() )
					return str;
				xst_stringstream ss;
				ss << "cbuffer cbDrawcall : register(b" << uRegister << ")" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD ]					<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION ]	<< ";" << xst_endl;
				ss << "\tmatrix " << astrConstants[ ShaderConstants::MTX_OBJ_WORLD_INVERSE_TRANSPOSE ]	<< ";" << xst_endl;
				ss << "}" << xst_endl;
				str = ss.str();
				return str;
			}

			xst_castring& CreatePerFramePSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ], u32 uRegister)
			{
				auto& str = g_astrCBufferCodes[ ConstantBuffers::PS_ONCE_PER_FRAME ];
				if( !str.empty() )
					return str;
				/*
				Vec4		vecLightColor;
			Vec4		vecSceneAmbient;
			Vec4		vecLightSpecularColor;
			Vec3		vecLightPos;
			f32			fTime;
			Vec3		vecCamPos;
			f32			fLightSpecularPower;
			Vec3		vecCamDir;
			f32			fLightPower;
			Vec2		vecScreenSize;
				*/
				xst_stringstream ss;
				ss << "cbuffer cbFrame : register(b" << uRegister << ")" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::LIGHT_COLOR ]			<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::SCENE_AMBIENT_COLOR ]	<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR_COLOR ]	<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat "  << astrConstants[ ShaderConstants::TIME ]					<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_POSITION ]		<< ";" << xst_endl;
				ss << "\tfloat "  << astrConstants[ ShaderConstants::LIGHT_SPECULAR_POWER ]	<< ";" << xst_endl;
				ss << "\tfloat3 " << astrConstants[ ShaderConstants::CAMERA_DIRECTION ]		<< ";" << xst_endl;
				ss << "\tfloat "  << astrConstants[ ShaderConstants::LIGHT_POWER ]			<< ";" << xst_endl;
				ss << "\tfloat2 " << astrConstants[ ShaderConstants::SCREEN_SIZE ]			<< ";" << xst_endl;
				ss << "}" << xst_endl;
				str = ss.str();
				return str;
			}

			xst_castring& CreatePerDrawcallPSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ], u32 uRegister)
			{
				auto& str = g_astrCBufferCodes[ ConstantBuffers::PS_ONCE_PER_DRAWCALL ];
				if( !str.empty() )
					return str;
				xst_stringstream ss;
				/*ss << "cbuffer cbPerObject : register( b1 )" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tfloat4 " << "tmp"	<< ";" << xst_endl;
				ss << "}" << xst_endl;*/
				str = ss.str();
				return str;
			}

			xst_castring& CreatePerMaterialPSCBuffer(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ], u32 uRegister)
			{
				auto& str = g_astrCBufferCodes[ ConstantBuffers::PS_ONCE_PER_MATERIAL ];
				if( !str.empty() )
					return str;
				/*
				Vec4	vecAmbientColor;
			Vec4	vecDiffuseColor;
			Vec4	vecSpecularColor;
			f32		fShininess;
			f32		fAlpha;
				*/
				xst_stringstream ss;
				ss << "cbuffer cbMaterial : register(b" << uRegister << ")" << xst_endl;
				ss << "{" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::MATERIAL_AMBIENT_COLOR ]	<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::MATERIAL_DIFFUSE_COLOR ]	<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::MATERIAL_SPECULAR_COLOR ]	<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::MATERIAL_SHININESS ]		<< ";" << xst_endl;
				ss << "\tfloat4 " << astrConstants[ ShaderConstants::MATERIAL_ALPHA ]			<< ";" << xst_endl;
				ss << "}" << xst_endl;
				str = ss.str();
				return str;
			}

			xst_astring& CreateVUniforms(xst_castring astrConstants[ ShaderConstants::_ENUM_COUNT ])
			{
				xst_stringstream ss;
				/*ss << ",uniform float " << astrConstants[ ShaderConstants::TIME ] << xst_endl;
				ss << ",uniform float3 " << astrConstants[ ShaderConstants::LIGHT_POSITION ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_DIFFUSE ] << xst_endl;
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR_POWER ] << xst_endl;
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
				ss << ",uniform float4 " << astrConstants[ ShaderConstants::LIGHT_SPECULAR_POWER ] << xst_endl;
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
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_FRAME_VS_CBUFFER ]		= HLSL::CreatePerFrameVSCBuffer( this->CONSTANT_NAMES, 0 );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_FRAME_PS_CBUFFER ]		= HLSL::CreatePerFramePSCBuffer( this->CONSTANT_NAMES, 0 );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_MATERIAL_PS_CBUFFER ]	= HLSL::CreatePerMaterialPSCBuffer( this->CONSTANT_NAMES, 1 );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_DRAWCALL_VS_CBUFFER ]	= HLSL::CreatePerDrawcallVSCBuffer( this->CONSTANT_NAMES, 1 );
			this->m_astrShaderCodes[ IShaderSystem::ShaderCodes::PER_DRAWCALL_PS_CBUFFER ]	= HLSL::CreatePerDrawcallPSCBuffer( this->CONSTANT_NAMES, 2 );
			HLSL::CreateVUniforms( this->CONSTANT_NAMES );
			HLSL::CreatePUniforms( this->CONSTANT_NAMES );
			return XST_OK;
		}

		i32 CHLSLShaderSystem::Init(IRenderSystem* pRS)
		{
			m_pRS = (CRenderSystem*)pRS;
			//Check options
			//Check feature level

			for( i32 i = ConstantBuffers::_ENUM_COUNT; i --> 0; )
				m_apD3DConstantBuffers[ i ] = xst_null;

			HLSL::SetProfiles( m_pRS->m_eFeatureLevel, m_astrProfiles );

			HLSL::g_strShader.reserve( 2048 );
			HLSL::g_strConstantVUniforms.reserve( 512 );
			HLSL::g_strConstantPUniforms.reserve( 512 );

			//Apply default names
			if( XST_FAILED( ApplyShaderConstantNames() ) )
			{
				return XST_FAIL;
			}

			m_vAllConstantValues.resize( ShaderConstants::_ENUM_COUNT );
			u32 uPSCurrReg = 0, uVSCurrReg = 0;
			//Shader codes
			ApplyShaderConstantNames();
	
			HRESULT hr;
			//Create constant buffers
			D3D11_BUFFER_DESC BufferDesc;
			xst_zero( &BufferDesc, sizeof( BufferDesc ) );
			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			u32 uDataSizes[ ConstantBuffers::_SYS_COUNT ] = 
			{
				/*VS_ONCE_PER_FRAME,
			VS_ONCE_PER_DRAWCALL,
			PS_ONCE_PER_FRAME,
			PS_ONCE_PER_DRAWCALL,
			PS_ONCE_PER_MATERIAL,*/
				sizeof( SVSOncePerFrame ),
				sizeof( SVSOncePerObject ),
				sizeof( SPSOncePerFrame ),
				sizeof( SPSOncePerObject ),
				sizeof( PSOncePerMaterial )
			};

			for( u32 i = 0; i < ConstantBuffers::_SYS_COUNT; ++i )
			{
				BufferDesc.ByteWidth = uDataSizes[i];
				hr = m_pRS->m_pDevice->CreateBuffer( &BufferDesc, xst_null, &m_apD3DConstantBuffers[ i ] );
				if( FAILED( hr ) )
				{
					XST_LOG_ERR( "[D3D11]: Could not create constant buffer: " << m_pRS->_ErrorToString( hr ) );
					return XST_FAIL;
				}
			}

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
			if( uConstantOffset + uSize <= vValues.size() )
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
			if( uConstantOffset + uCount <= vValues.size() )
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

		xst_fi void UpdateConstantBuffer(ID3D11DeviceContext* pCtx, ID3D11Buffer* pBuff, D3D11_MAPPED_SUBRESOURCE& Map, const void* pData, u32 uDataSize)
		{
			pCtx->Map( pBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &Map );
			xst_memcpy( Map.pData, uDataSize, pData, uDataSize );
			pCtx->Unmap( pBuff, 0 );
		}

		xst_fi void UpdateVSConstantBuffer(ID3D11DeviceContext* pCtx, ID3D11Buffer** ppBuff, D3D11_MAPPED_SUBRESOURCE& Map, const CCBBuilder& CB)
		{
			UpdateConstantBuffer( pCtx, *ppBuff, Map, &CB.vData[0], CB.uSizeInBytes );
			pCtx->VSSetConstantBuffers( CB.uRegister, 1, ppBuff );
		}

		xst_fi void UpdatePSConstantBuffer(ID3D11DeviceContext* pCtx, ID3D11Buffer** ppBuff, D3D11_MAPPED_SUBRESOURCE& Map, const CCBBuilder& CB)
		{
			UpdateConstantBuffer( pCtx, *ppBuff, Map, &CB.vData[0], CB.uSizeInBytes );
			pCtx->PSSetConstantBuffers( CB.uRegister, 1, ppBuff );
		}

		// TODO: separate updates for vertex and fragment shaders
		void CHLSLShaderSystem::UpdateFrameInputs()
		{	
			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_FRAME ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			g_pOncePerFrameVS = (SVSOncePerFrame*)g_MappedSubresource.pData;
			m_pRS->GetMatrix( MatrixTypes::PROJECTION,	&g_pOncePerFrameVS->mtxProj );
			m_pRS->GetMatrix( MatrixTypes::VIEW,		&g_pOncePerFrameVS->mtxView );
			m_pRS->GetMatrix( MatrixTypes::VIEW_PROJ,	&g_pOncePerFrameVS->mtxViewProj );
			g_pOncePerFrameVS->vecCamPos.Set( m_vAllConstantValues[ ShaderConstants::CAMERA_POSITION ].float3 );
			g_pOncePerFrameVS->vecCamDir.Set( m_vAllConstantValues[ ShaderConstants::CAMERA_DIRECTION ].float3 );
			g_pOncePerFrameVS->vecScreenSize.Set( m_vAllConstantValues[ ShaderConstants::SCREEN_SIZE ].float2 );
			g_pOncePerFrameVS->fTime = m_vAllConstantValues[ ShaderConstants::TIME ].float1[0];
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_FRAME ], 0 );
			m_pRS->m_pDeviceContext->VSSetConstantBuffers( 0, 1, &m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_FRAME ] );

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_FRAME ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			g_pOncePerFramePS = (SPSOncePerFrame*)g_MappedSubresource.pData;
			g_pOncePerFramePS->vecLightColor.Set( m_vAllConstantValues[ ShaderConstants::LIGHT_COLOR ].float4 );
			g_pOncePerFramePS->vecSceneAmbient.Set( m_vAllConstantValues[ ShaderConstants::SCENE_AMBIENT_COLOR ].float4 );
			g_pOncePerFramePS->vecLightPos.Set( m_vAllConstantValues[ ShaderConstants::LIGHT_POSITION ].float3 );
			g_pOncePerFramePS->vecCamPos.Set( m_vAllConstantValues[ ShaderConstants::CAMERA_POSITION ].float3 );
			g_pOncePerFramePS->vecCamDir.Set( m_vAllConstantValues[ ShaderConstants::CAMERA_DIRECTION ].float3 );
			g_pOncePerFramePS->vecScreenSize.Set( m_vAllConstantValues[ ShaderConstants::SCREEN_SIZE ].float2 );
			g_pOncePerFramePS->fLightSpecularPower = ( m_vAllConstantValues[ ShaderConstants::LIGHT_SPECULAR_POWER ].float1[0] );
			g_pOncePerFramePS->fTime = ( m_vAllConstantValues[ ShaderConstants::TIME ].float1[0] );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_FRAME ], 0 );
			m_pRS->m_pDeviceContext->PSSetConstantBuffers( 0, 1, &m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_FRAME ] );
		}

		void CHLSLShaderSystem::UpdateMaterialInputs()
		{
			//auto& CB = g_aCBBuilders[ ConstantBuffers::PS_ONCE_PER_MATERIAL ];
			/*CB.SetConstant( ShaderConstants::MATERIAL_ALPHA, 0.5f );
			CB.SetConstant( ShaderConstants::MATERIAL_AMBIENT_COLOR, Vec4(0.2f) );
			CB.SetConstant( ShaderConstants::MATERIAL_DIFFUSE_COLOR, Vec4(0.7f) );
			CB.SetConstant( ShaderConstants::MATERIAL_SHININESS, 0.1f );
			CB.SetConstant( ShaderConstants::MATERIAL_SPECULAR_COLOR, Vec4(0.3f) );*/

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_MATERIAL ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			g_pOncePerMatPS = (PSOncePerMaterial*)g_MappedSubresource.pData;
			g_pOncePerMatPS->vecAmbientColor.Set( m_vAllConstantValues[ ShaderConstants::MATERIAL_AMBIENT_COLOR ].float4 );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_MATERIAL ], 0 );
			m_pRS->m_pDeviceContext->PSSetConstantBuffers( CB.uRegister, 1, &m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_MATERIAL ] );
		}

		static DirectX::XMMATRIX InverseTranspose( DirectX::CXMMATRIX M )
		{
			DirectX::XMMATRIX A = M;
			A.r[3] = DirectX::XMVectorSet(0.0f,0.0f,0.0f,1.0f);
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
			return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
		}

		//MULTIPLE PER FRAME
		DirectX::XMMATRIX g_mtxView, g_mtxProj;
		void CHLSLShaderSystem::UpdateDrawCallInputs()
		{
			// VERTEX
			/*g_aCBBuilders[ConstantBuffers::VS_ONCE_PER_DRAWCALL].SetConstant( ShaderConstants::MTX_OBJ_WORLD, g_VSOncePerObj.mtxWorld );
			g_aCBBuilders[ConstantBuffers::VS_ONCE_PER_DRAWCALL].SetConstant( ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION, g_VSOncePerObj.mtxWorldViewProj );
			g_aCBBuilders[ConstantBuffers::VS_ONCE_PER_DRAWCALL].SetConstant( ShaderConstants::MTX_OBJ_WORLD_INVERSE_TRANSPOSE, g_VSOncePerObj.mtxWorldInvT );*/
		
			// PIXEL

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_DRAWCALL ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			g_pOncePerObjVS = (SVSOncePerObject*)g_MappedSubresource.pData;
			//Get object input data
			m_pRS->GetMatrix( MatrixTypes::WORLD,		&g_pOncePerObjVS->mtxWorld );
			g_pOncePerObjVS->mtxWorldInvT = InverseTranspose( g_pOncePerObjVS->mtxWorld );
			g_pOncePerObjVS->mtxWorldViewProj = XMMatrixTranspose( XMMatrixMultiply( XMMatrixMultiply( g_pOncePerObjVS->mtxWorld, g_pOncePerFrameVS->mtxView ), g_pOncePerFrameVS->mtxProj ) );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_DRAWCALL ], 0 );
			m_pRS->m_pDeviceContext->VSSetConstantBuffers( 1, 1, &m_apD3DConstantBuffers[ ConstantBuffers::VS_ONCE_PER_DRAWCALL ] );

			m_pRS->m_pDeviceContext->Map( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_DRAWCALL ], 0, D3D11_MAP_WRITE_DISCARD, 0, &g_MappedSubresource );
			xst_memcpy( g_MappedSubresource.pData, sizeof(SPSOncePerObject), &g_PSOncePerObj, sizeof(SPSOncePerObject) );
			m_pRS->m_pDeviceContext->Unmap( m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_DRAWCALL ], 0 );
			m_pRS->m_pDeviceContext->PSSetConstantBuffers( 1, 1, &m_apD3DConstantBuffers[ ConstantBuffers::PS_ONCE_PER_DRAWCALL ] );
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

		i32 _AddConstantBuffers(IShader** ppShader, xst_castring* *const astrCBs, u32 uCount)
		{
			xst_assert2( (*ppShader) );
			IShader* pShader = (*ppShader);
			FilePtr pFile = pShader->GetResourceFile();
			TCData<u8>& Data = pFile->GetData();
			xst_astring strCode;
			strCode.reserve( 10000 );
			for(u32 i = 0; i < uCount; ++i )
				strCode += *(astrCBs[ i ]);
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
					xst_castring* a[] = {	&this->GetShaderCode( ShaderCodes::PER_FRAME_VS_CBUFFER ),
											&this->GetShaderCode( ShaderCodes::PER_DRAWCALL_VS_CBUFFER )};
					_AddConstantBuffers( &pShader, a, 2 );
					CVertexShader* pVShader = (CVertexShader*)pShader;
					return this->CompileVertexShader( pVShader );
				}
				break;

				case ShaderTypes::PIXEL:
				{
					xst_castring* a[] = {	&this->GetShaderCode( ShaderCodes::PER_FRAME_PS_CBUFFER ),
											&this->GetShaderCode( ShaderCodes::PER_MATERIAL_PS_CBUFFER ),
											&this->GetShaderCode( ShaderCodes::PER_DRAWCALL_PS_CBUFFER )};
					_AddConstantBuffers( &pShader, a, 3 );
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
			strCode += GetShaderCode( ShaderCodes::PER_DRAWCALL_VS_CBUFFER ) + xst_endl;

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