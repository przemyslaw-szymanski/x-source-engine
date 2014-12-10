#include "XSECD3D11InputLayout.h"
#include "XSECD3D11RenderSystem.h"
#include "XSECD3D11VertexShader.h"

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
			xst_release( m_pD3DVertexShaderBlob );
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

		void AddShaderInput(xst_astring& strVS, lpcastr lpszSemantic, lpcastr lpszType, bool bIsNextParameter)
		{
			ch8 aBuff[ 256 ];
			#if defined (XST_VISUAL_STUDIO)
				sprintf_s( aBuff, 256, "%s %s : %s%s\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).data(), lpszSemantic, ( bIsNextParameter )? "," : "" );
			#else
				sprintf( aBuff, "%s %s : %s%s\n", lpszType, XST::StringUtil::ToLower( lpszSemantic ).data(), lpszSemantic, ( bIsNextParameter )? "," : "" );
			#endif
			strVS += aBuff;
		}

		void AddShaderInput( xst_astring& strVS, lpcastr lpszSemantic, lpcastr lpszType, bool bIsNextParameter )
		{

		}
 
		i32 CInputLayout::_Build()
		{
			//Build shader
			//xst_astring strVS;
			m_strVSCode.clear();
			m_strVSCode.reserve( 1024 );
			m_strVSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_FRAME_CBUFFER ) + "\n";
			m_strVSCode += m_pRS->GetShaderSystem()->GetShaderCode( IShaderSystem::ShaderCodes::PER_OBJECT_CBUFFER ) + "\n";
			m_strVSCode += "\nfloat4 VS(";
			xst_astring strName = "";
			//AddShaderInput( m_strVSCode, "POSITION", "float4", m_Elements.size() > 0 );

			//Add default POSITION
			//m_aInputElements[ 0 ] = CreateInputElement( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
			ul32 ulOffset = 0;
			u32 uiTexUnit = 0;

			for(u32 i = 0; i < m_Elements.size(); ++i)
			{
				switch( m_Elements[ i ].eType )
				{
					case InputLayoutElements::POSITION:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetPositionSize();
						AddShaderInput( m_strVSCode, "POSITION", "float4", i + 1 < m_Elements.size() );
						strName += "Position";
					}
					break;

					case InputLayoutElements::NORMAL:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetNormalSize();
						AddShaderInput( m_strVSCode, "NORMAL", "float3", i + 1 < m_Elements.size() );
						strName += "Normal";
					}
					break;

					case InputLayoutElements::TEXCOORD0:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD0", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord0";
					}
					break;

					case InputLayoutElements::TEXCOORD1:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD1", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord1";
					}
					break;

					case InputLayoutElements::TEXCOORD2:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD2", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord2";
					}
					break;

					case InputLayoutElements::TEXCOORD3:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD3", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord3";
					}
					break;

					case InputLayoutElements::TEXCOORD4:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD4", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord4";
					}
					break;

					case InputLayoutElements::TEXCOORD5:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD5", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord5";
					}
					break;

					case InputLayoutElements::TEXCOORD6:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD6", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord6";
					}
					break;

					case InputLayoutElements::TEXCOORD7:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "TEXCOORD", uiTexUnit++, DXGI_FORMAT_R32G32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetTexCoordSize();
						AddShaderInput( m_strVSCode, "TEXCOORD7", "float2", i + 1 < m_Elements.size() );
						strName += "Texcoord7";
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

					case InputLayoutElements::COLOR:
					{
						m_aInputElements[ i + 0 ] = CreateInputElement( "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, ulOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 );
						ulOffset += GetColorSize();
						AddShaderInput( m_strVSCode, "COLOR", "float4", i + 1 < m_Elements.size() );
						strName += "Color";
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

			//End shader build
			m_strVSCode += ") : SV_POSITION { return ";
			m_strVSCode += "mul( position, " + IShaderSystem::GetConstantName( ShaderConstants::MTX_OBJ_WORLD_VIEW_PROJECTION );
			m_strVSCode += " ); }";
			
			lpcastr lpszText = m_strVSCode.data();
			ul32 ulTextLen = m_strVSCode.length();
		
			IShaderSystem* pSS = m_pRS->GetShaderSystem();
			CVertexShader* pVS = (CVertexShader*)pSS->CreateVertexShader( this, xst_null, 0, strName, XST::ResourceType::SHADER, XST::ResourceStates::CREATED, xst_null );
			if( pVS == xst_null )
			{
				return XST_FAIL;
			}

			pVS->m_strShaderEntryPoint = "VS";
			pVS->m_eProfile = ShaderProfiles::VS_1_1;
		
			if( XST_FAILED( pSS->CompileVertexShader( pVS, m_strVSCode.data(), m_strVSCode.length(), pVS->m_strShaderEntryPoint.data(), pVS->m_eProfile ) ) )
			{
				return XST_FAIL;
			}

			m_pD3DVertexShaderBlob = pVS->m_pBlob;

			//Create d3d input layout
			if( XST_FAILED( m_pRS->_CreateInputLayout( this ) ) )
			{
				xst_release( m_pD3DVertexShaderBlob );
				return XST_FAIL;
			}

			xst_release( m_pD3DVertexShaderBlob );
			m_pVS = VertexShaderPtr( pVS );
			return XST_OK;
		}

	}//d3d11
}//xse
#endif //XST_WINDOWS