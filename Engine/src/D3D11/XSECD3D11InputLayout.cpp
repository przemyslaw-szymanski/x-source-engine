#include "XSECD3D11InputLayout.h"
#include "XSECD3D11RenderSystem.h"
#include "XSECD3D11VertexShader.h"
#include "XSECD3D11PixelShader.h"

#if defined (XST_WINDOWS)
namespace XSE
{
	namespace D3D11
	{
		CInputLayout::CInputLayout(CRenderSystem* pRS) :m_pRS( pRS ), m_pD3DInputLayout( xst_null ), m_pD3DVertexShaderBlob( xst_null ), m_ulCurrentOffset( 0 ),
														m_ulVertexSize( 0 ), m_pVS( xst_null )
		{
			xst_zero( m_aInputElements, sizeof( m_aInputElements ) );
			//m_Elements.reserve( InputLayoutElements::_ENUM_COUNT );
		}

		CInputLayout::~CInputLayout()
		{
			//xst_release( m_pVS );
			//xst_release( m_pD3DVertexShaderBlob );
		}

        i32 CInputLayout::Create(cul32& uElements, CVertexShader* pShader)
        {
            xst_assert2( pShader );
            xst_assert2( m_pVS.IsValid() );
            m_pVS = pShader;
            return Create( uElements );
        }

		i32 CInputLayout::Create(cul32& ulElements)
		{
			m_ulCurrentOffset = 0;
			SInputLayoutElement Element;
			m_ulVertexSize = 0;

			if( ulElements & InputLayoutElements::POSITION )
			{
				Element.eType = InputLayoutElements::POSITION;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetPositionSize();
				Element.uiId = ILEIds::POSITION;
				m_ulCurrentOffset += Element.ulSize;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::POSITION ] = true;
			}

			if( ulElements & InputLayoutElements::NORMAL )
			{
				Element.eType = InputLayoutElements::NORMAL;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetNormalSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::NORMAL;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::NORMAL ] = true;
			}

			if( ulElements & InputLayoutElements::TEXCOORD0 )
			{
				Element.eType = InputLayoutElements::TEXCOORD0;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD0;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD0 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::BINORMAL )
			{
				Element.eType = InputLayoutElements::BINORMAL;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetBinormalSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::BINORMAL;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::BINORMAL ] = true;
			}

			if( ulElements & InputLayoutElements::TANGENT )
			{
				Element.eType = InputLayoutElements::TANGENT;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTangentSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TANGENT;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TANGENT ] = true;
			}

			if( ulElements & InputLayoutElements::COLOR )
			{
				Element.eType = InputLayoutElements::COLOR;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetColorSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::COLOR;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::COLOR ] = true;
			}

			if( ulElements & InputLayoutElements::TEXCOORD1 )
			{
				Element.eType = InputLayoutElements::TEXCOORD1;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD1;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD1 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD2 )
			{
				Element.eType = InputLayoutElements::TEXCOORD2;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD2;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD2 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD3 )
			{
				Element.eType = InputLayoutElements::TEXCOORD3;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD3;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD3 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD4 )
			{
				Element.eType = InputLayoutElements::TEXCOORD4;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD4;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD4 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD5 )
			{
				Element.eType = InputLayoutElements::TEXCOORD5;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD5;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD5 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD6 )
			{
				Element.eType = InputLayoutElements::TEXCOORD6;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD6;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD6 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::TEXCOORD7 )
			{
				Element.eType = InputLayoutElements::TEXCOORD7;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetTexCoordSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::TEXCOORD7;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::TEXCOORD7 ] = true;
				m_uTexCoordCount++;
			}

			if( ulElements & InputLayoutElements::SPECULAR )
			{
				Element.eType = InputLayoutElements::SPECULAR;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetSpecularSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::SPECULAR;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::SPECULAR ] = true;
			}

			if( ulElements & InputLayoutElements::DIFFUSE )
			{
				Element.eType = InputLayoutElements::DIFFUSE;
				Element.ulOffset = m_ulCurrentOffset;
				Element.ulSize = IInputLayout::GetDiffuseSize();
				m_ulCurrentOffset += Element.ulSize;
				Element.uiId = ILEIds::DIFFUSE;
				m_Elements.push_back( Element );
				m_ulVertexSize += Element.ulSize;
				this->m_aAvailableElements[ ILEIds::DIFFUSE ] = true;
			}

			if( m_Elements.size() == 0 )
			{
				XST_LOG_ERR( "No elements specified for this input layout" );
				return XST_FAIL;
			}

			return _Build();
		}

		i32 CInputLayout::_FindElement(INPUT_LAYOUT_ELEMENT eElement)
		{
			for(u32 i = 0; i < m_Elements.size(); ++i)
			{
				if( m_Elements[ i ].eType == eElement )
				{
					return i;
				}
			}

			return -1;
		}

		ul32 _GetElementSize(INPUT_LAYOUT_ELEMENT eElement)
		{
			switch( eElement )
			{
				case InputLayoutElements::POSITION:
				{
					return ILESizes::POSITION;
				}
				break;

				case InputLayoutElements::NORMAL:
				{
					return ILESizes::NORMAL;
				}
				break;

				case InputLayoutElements::BINORMAL:
				{
					return ILESizes::BINORMAL;
				}
				break;

				case InputLayoutElements::TANGENT:
				{
					return ILESizes::TANGENT;
				}
				break;

				case InputLayoutElements::COLOR:
				{
					return ILESizes::COLOR;
				}
				break;

				case InputLayoutElements::TEXCOORD0:
				case InputLayoutElements::TEXCOORD1:
				case InputLayoutElements::TEXCOORD2:
				case InputLayoutElements::TEXCOORD3:
				case InputLayoutElements::TEXCOORD4:
				case InputLayoutElements::TEXCOORD5:
				case InputLayoutElements::TEXCOORD6:
				case InputLayoutElements::TEXCOORD7:
				{
					return ILESizes::TEXCOORD;
				}
				break;

				case InputLayoutElements::SPECULAR:
				{
					return ILESizes::POSITION;
				}
				break;

				case InputLayoutElements::DIFFUSE:
				{
					return ILESizes::POSITION;
				}
				break;
			}

			return 0;
		}

		D3D11_INPUT_ELEMENT_DESC CreateInputElement(lpcastr lpszSemanticName, u32 uiSemanticIndex, DXGI_FORMAT Format, 
													u32 uiInputSlot, u32 uiAligmentByteOffset, 
													D3D11_INPUT_CLASSIFICATION InputSlotClass, u32 uiInstanceDataStepRate)
		{
			D3D11_INPUT_ELEMENT_DESC Desc;// = {	lpszSemanticName, uiSemanticIndex, Format, uiInputSlot, uiAligmentByteOffset,
										//		InputSlotClass, uiInstanceDataStepRate };
			Desc.Format = Format;
			Desc.SemanticIndex = uiSemanticIndex;
			Desc.SemanticName = lpszSemanticName;
			Desc.AlignedByteOffset = uiAligmentByteOffset;
			Desc.InputSlot = uiInputSlot;
			Desc.InstanceDataStepRate = uiInstanceDataStepRate;
			Desc.AlignedByteOffset = uiAligmentByteOffset;
			Desc.InputSlotClass = InputSlotClass;
			return Desc;
		}

		void AddShaderInput(xst_astring& str, lpcastr lpszSemantic, lpcastr lpszType, bool bIsNextParameter)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s%s\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).c_str(), lpszSemantic, ( bIsNextParameter )? "," : "" );
			str += aBuff;
		}

		void AddShaderInput(xst_astring& str, lpcastr lpszSemantic, lpcastr lpszType)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s;\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).c_str(), lpszSemantic );
			str += aBuff;
		}

		void AddShaderOutput(xst_astring& strVS, lpcastr lpszSemantic, lpcastr lpszType, bool bIsNextParameter)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s%s\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).c_str(), lpszSemantic, ( bIsNextParameter )? "," : "" );
			strVS += aBuff;
		}

		void AddShaderOutput(xst_astring& strVS, lpcastr lpszSemantic, lpcastr lpszType)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s;\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).c_str(), lpszSemantic );
			strVS += aBuff;
		}

		void AddShaderInOut(xst_astring& strVS, xst_astring& strPS, lpcastr lpszSemantic, lpcastr lpszType, bool bIsNextParameter)
		{
			AddShaderInput( strVS, lpszSemantic, lpszType, bIsNextParameter );
			AddShaderOutput( strPS, lpszSemantic, lpszType, bIsNextParameter );
		}

		void AddShaderInOut(xst_astring& strVS, xst_astring& strPS, lpcastr lpszSemantic, lpcastr lpszType)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s;\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).c_str(), lpszSemantic );
			strVS += aBuff;
			strPS += aBuff;
		}

		void AddShaderInOut(xst_astring& strVS, xst_astring& strPS, lpcastr lpszVSSemantic, lpcastr lpszPSSemantic, lpcastr lpszType)
		{
			ch8 aBuff[ 256 ];
			xst_sprintf( aBuff, 256, "%s %s : %s;\n", lpszType, XST::StringUtil::ToLower( lpszVSSemantic ).c_str(), lpszVSSemantic );
			strVS += aBuff;

			xst_zero( aBuff, sizeof( aBuff ) );
			xst_sprintf( aBuff, 256, "%s %s : %s;\n", lpszType, XST::StringUtil::ToLower( lpszVSSemantic ).c_str(), lpszPSSemantic );
			strPS += aBuff;
		}

		xst_astring g_strVS_OUT;
		xst_astring g_strVS_IN;

		void FindAndReplace(xst_astring* pOut, xst_castring& strFind, xst_castring& strNew)
		{
			std::size_t uPos = 0;
			while( ( uPos = pOut->find( strFind, uPos ) ) != xst_astring::npos )
			{
				pOut->replace( uPos, strFind.length(), strNew );
				uPos += strNew.length();
			}
		}
 
		i32 CInputLayout::_Build()
		{
			g_strVS_OUT.clear();
			g_strVS_OUT.reserve( 1024 );
			g_strVS_IN.clear();
			g_strVS_IN.reserve( 1024 );

			//Build shader
			//xst_astring strVS;
			m_strVSCode.clear();
			m_strVSCode.reserve( 1024 );
			m_strVSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_FRAME_VS_CBUFFER ) + "\n";
			m_strVSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_DRAWCALL_VS_CBUFFER ) + "\n";
			
			g_strVS_IN += "\nstruct VS_IN {\n";
			g_strVS_OUT += "\nstruct VS_OUT {\n";
			
			m_strVSCode += "[VS_IN]\n[VS_OUT]\nVS_OUT VS(VS_IN IN) {\nVS_OUT OUT;";

			m_strPSCode.clear();
			m_strPSCode.reserve( 1024 );
			m_strPSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_FRAME_PS_CBUFFER ) + "\n";
			m_strPSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_DRAWCALL_PS_CBUFFER ) + "\n";
			m_strPSCode += "\n[VS_OUT]\nfloat4 PS(VS_OUT IN) : COLOR {\nfloat4 C = float4(1,0.02,0.5,0);";

			xst_astring strName = "";
			//AddShaderInput( m_strVSCode, "POSITION", "float4", m_Elements.size() > 0 );

			//Add default POSITION
			//m_aInputElements[ 0 ] = CreateInputElement( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
			ul32 ulOffset = 0;
			u32 uiTexUnit = m_uTexCoordCount;
			ch8 strTmp[ 128 ];

			for(u32 i = 0; i < m_Elements.size(); ++i)
			{
				bool bNext = i + 1 < m_Elements.size();
				m_ulHandle |= m_Elements[ i ].eType;

				switch( m_Elements[ i ].eType )
				{
					case InputLayoutElements::POSITION:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetPositionSize();
						//AddShaderInOut( g_strVS_IN, g_strVS_OUT, "POSITION0", "SV_POSITION", "float3" );
						AddShaderInput(g_strVS_IN, "POSITION", "float3");
						AddShaderOutput(g_strVS_OUT, "SV_POSITION", "float4");
						g_strVS_OUT += "float3 pos : TEXCOORD";
						xst_sprintf( strTmp, 128, "%d;\n", uiTexUnit++ );
						g_strVS_OUT += strTmp;
						m_strVSCode += "\nOUT.sv_position = mul(float4(IN.position.xyz,1),[WVP]);";
						m_strVSCode += "\nOUT.pos = mul(IN.position.xyz,[W]);";
						strName += "Position";
					}
					break;

					case InputLayoutElements::COLOR:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetColorSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "COLOR", "float4" );
						m_strVSCode += "\nOUT.color = IN.color;";
						strName += "Color";
					}
					break;

					case InputLayoutElements::NORMAL:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetNormalSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "NORMAL", "float3" );
						m_strVSCode += "\nOUT.normal=IN.normal;";
						strName += "Normal";
					}
					break;

					case InputLayoutElements::TEXCOORD0:
					{
						//m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						D3D11_INPUT_ELEMENT_DESC d = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
						m_aInputElements[ i + 0 ] = d;
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD0", "float2" );
						strName += "Texcoord0";
						m_strVSCode += "\nOUT.texcoord0=IN.texcoord0;";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD1:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD1", "float2" );
						strName += "Texcoord1";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD2:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD2", "float2" );
						strName += "Texcoord2";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD3:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD3", "float2" );
						strName += "Texcoord3";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD4:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD4", "float2" );
						strName += "Texcoord4";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD5:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 5, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD5", "float2" );
						strName += "Texcoord5";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD6:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 6, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD6", "float2" );
						strName += "Texcoord6";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::TEXCOORD7:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", 7, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInOut( g_strVS_IN, g_strVS_OUT, "TEXCOORD7", "float2" );
						strName += "Texcoord7";
						uiTexUnit++;
					}
					break;

					case InputLayoutElements::BINORMAL:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetBinormalSize();
						AddShaderInput( m_strVSCode, "BINORMAL", "float3", i + 1 < m_Elements.size() );
						strName += "Binormal";
					}
					break;

					case InputLayoutElements::TANGENT:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTangentSize();
						AddShaderInput( m_strVSCode, "TANGENT", "float3", i + 1 < m_Elements.size() );
						strName += "Tangent";
					}
					break;

					case InputLayoutElements::SPECULAR:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "SPECULAR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetSpecularSize();
						AddShaderInput( m_strVSCode, "SPECULAR", "float3", i + 1 < m_Elements.size() );
						strName += "Specular";
					}
					break;

					case InputLayoutElements::DIFFUSE:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "DIFFUSE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetDiffuseSize();
						AddShaderInput( m_strVSCode, "DIFFUSE", "float3", i + 1 < m_Elements.size() );
						strName += "Diffuse";
					}
					break;

				}//switch
			}//for

			g_strVS_IN += "};\n";
			g_strVS_OUT += "};\n";
			//End shader build
			m_strVSCode += "\nreturn OUT;\n}";
			FindAndReplace( &m_strVSCode, "[VS_IN]", g_strVS_IN );
			FindAndReplace( &m_strVSCode, "[VS_OUT]", g_strVS_OUT );
			FindAndReplace( &m_strVSCode, "[WVP]", IShaderSystem::GetConstantName( ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION ) );
			FindAndReplace( &m_strVSCode, "[W]", IShaderSystem::GetConstantName( ShaderConstants::MTX_OBJ_WORLD ) );
			FindAndReplace( &m_strVSCode, "[WINVT]", IShaderSystem::GetConstantName( ShaderConstants::MTX_OBJ_WORLD_INVERSE_TRANSPOSE ) );

			if( this->m_aAvailableElements[ InputLayoutElementIds::COLOR ] )
				m_strPSCode += "\nC=saturate(IN.color);";
			
			if( this->m_aAvailableElements[ InputLayoutElementIds::NORMAL ] )
				m_strPSCode += "\nfloat3 P=IN.pos;\nfloat3 N=normalize(IN.normal);\nfloat3 L=normalize(float3(0,1000,0)-P);\nfloat DL=max(dot(N,L),0);\nfloat3 V=normalize(float3(-389,450,230)-P);\nfloat3 H=normalize(L+V);C=C*DL;";
				//m_strPSCode += "C.xyz = IN.normal; C.w=1;";
			if( this->m_aAvailableElements[ InputLayoutElementIds::TEXCOORD0 ] )
				m_strPSCode += "\nC *= float4(IN.texcoord0.x, IN.texcoord0.y, 1, 1);";

			m_strPSCode += "\nreturn C;\n}";
			FindAndReplace( &m_strPSCode, "[VS_OUT]", g_strVS_OUT );
			
			lpcastr lpszText = m_strVSCode.data();
			ul32 ulTextLen = m_strVSCode.length();

			XST::CDebug::PrintDebugLN("VS--------------------");
			XST::CDebug::PrintDebugLN(m_strVSCode);
			XST::CDebug::PrintDebugLN("PS--------------------");
			XST::CDebug::PrintDebugLN(m_strPSCode);
		
			IShaderSystem* pSS = m_pRS->GetShaderSystem();

            CVertexShader* pVS = (CVertexShader*)m_pVS.GetPtr();
            if( !pVS )
            {
                pVS = ( CVertexShader* )pSS->CreateVertexShader( this, xst_null, 0, strName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, xst_null );
                if( pVS == xst_null )
                {
                    return XST_FAIL;
                }

                pVS->m_strShaderEntryPoint = "VS";
                pVS->m_eProfile = ShaderProfiles::VS_BEST;
                pSS->DoNotValidateNextVertexShaderInput(); // Disable ComnpileVertexShader to validate its input due to it is not created yet
                if( XST_FAILED( pSS->CompileVertexShader( pVS, m_strVSCode.data(), m_strVSCode.length(), pVS->m_strShaderEntryPoint.data(), pVS->m_eProfile ) ) )
                {
                    xst_assert(0, "(InputLayout) COMPILE VERTEX SHADER ERROR");
                    return XST_FAIL;
                }
            }

			// Pixel Shader
			CPixelShader* pPS = (CPixelShader*)pSS->CreatePixelShader( xst_null, 0, strName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, xst_null );
			if( pPS == xst_null )
			{
				return XST_FAIL;
			}

			pPS->m_strShaderEntryPoint = "PS";
			pPS->m_eProfile = ShaderProfiles::PS_BEST;
		
			if( XST_FAILED( pSS->CompilePixelShader( pPS, m_strPSCode.data(), m_strPSCode.length(), pPS->m_strShaderEntryPoint.data(), pPS->m_eProfile ) ) )
			{
                xst_assert(0, "(InputLayout) COMPILE PIXEL SHADER ERROR");
				return XST_FAIL;
			}

			ID3D11ClassLinkage* pLinker;

			ID3DBlob* pPSBlob = pPS->m_pBlob;

			//Create d3d input layout
			m_pD3DVertexShaderBlob = pVS->m_pBlob; // Need for D3D11 CreateInputLayout, after that it must be released
			if( XST_FAILED( m_pRS->_CreateInputLayout( this ) ) )
			{
				if( m_pVS.IsNull() ) 
                    xst_release( pVS->m_pBlob );
				xst_release( pPS->m_pBlob );
				return XST_FAIL;
			}

            if( m_pVS.IsNull() )
            {
                xst_release( pVS->m_pBlob );
                m_pVS = VertexShaderPtr( pVS );
            }
			xst_release( pPS->m_pBlob );
			m_pD3DVertexShaderBlob = xst_null;

			m_pPS = PixelShaderPtr( pPS );
			return XST_OK;
		}

	}//d3d11
}//xse
#endif //XST_WINDOWS