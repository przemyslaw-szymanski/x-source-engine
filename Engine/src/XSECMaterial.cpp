#include "XSECMaterial.h"
#include "XSECShaderManager.h"
#include "XSECTechnique.h"
#include "XSECPass.h"
#include "XSECTextureManager.h"

namespace XSE
{
	namespace Resources
	{
		CMaterial::CMaterial(CShaderManager* pShaderMgr, XSE_IRESOURCE_DECL_PARAMS) : 
			XSE_IRESOURCE_CTOR, 
			m_pShaderMgr( pShaderMgr ), 
			m_pCurrentTechnique( xst_null )
		{
		}

		CMaterial::CMaterial(CShaderManager* pShaderMgr, xst_castring& strDefaultTechniqueName, XSE_IRESOURCE_DECL_PARAMS) :
			CMaterial( pShaderMgr, pCreator, Handle, strName, iType, iState, pAllocator )
		{
			ITechnique* pTech = CreateTechnique( strDefaultTechniqueName, true );
			xst_assert2( pTech );
		}

		CMaterial::~CMaterial()
		{
			for(TechniqueVector::iterator Itr = m_vTechniques.begin(); Itr != m_vTechniques.end(); ++Itr)
			{
				xst_delete( (*Itr) );
			}

			m_vTechniques.clear();
		}

		void CMaterial::SetAttributes(const SMaterialAttributes& Attribs)
		{
			m_Attribs = Attribs;
			for( auto* pTech : m_vTechniques )
			{
				pTech->SetAttributes( m_Attribs );
			}
		}

		void CMaterial::SetAmbientColor(const Vec4& vecCol)
		{
			m_Attribs.vecAmbientColor = vecCol;
			for( auto* pTech : m_vTechniques )
			{
				pTech->SetAmbientColor( vecCol );
			}
		}

		void CMaterial::SetDiffuseColor(const Vec4& vecCol)
		{

		}

		void CMaterial::SetSpecularColor(const Vec4& vecCol)
		{

		}

		void CMaterial::SetShininess(f32 fValue)
		{

		}
						

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName, bool bSetAsCurrent)
		{
			//Find this technique
			ITechnique* pTech = GetTechniqueByName( strName );
			if( pTech == xst_null )
			{
				pTech = xst_new CTechnique( strName );
				if( pTech == xst_null )
				{
					XST_LOG_ERR( "Failed to create technique: " << strName << " for material: " << this->GetResourceName() << ". Memory error." );
					return xst_null;
				}
				pTech->m_lTechniqueID = m_vTechniques.size();
				pTech->m_ulTechniqueNameHash = XST::CHash::GetCRC( strName );
				IPass* pPass = pTech->CreatePass( "pass0" );
				//Set default vertex and pixel shaders
				pPass->SetVertexShader( m_pShaderMgr->GetDefaultVertexShader() );
				pPass->SetPixelShader( m_pShaderMgr->GetDefaultPixelShader() );

				m_vTechniques.push_back( pTech );
			}
			if( bSetAsCurrent )
				m_pCurrentTechnique = pTech;
			return pTech;
		}

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName, const VertexShaderPtr& pVS, const PixelShaderPtr& pPS, bool bSetAsCurrent)
		{
			ITechnique* pTech = CreateTechnique( strName, bSetAsCurrent );
			pTech->GetPass( 0 )->SetVertexShader( pVS );
			pTech->GetPass( 0 )->SetPixelShader( pPS );
			return pTech;
		}

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName, xst_castring& strEffectName, bool bSetAsCurrent)
		{
			return xst_null;
		}

		CMaterial::ConstTechniqueIterator	CMaterial::GetTechniqueIterator() const
		{
			return ConstTechniqueIterator( &m_vTechniques );
		}

		CMaterial::TechniqueIterator	CMaterial::GetTechniqueIterator()
		{
			return TechniqueIterator( &m_vTechniques );
		}

		i32 CMaterial::UseTechnique(xst_castring& strName)
		{
			//If selected technique is the current technique
			ul32 ulHash = XST::CHash::GetCRC( strName );
			if( m_pCurrentTechnique->m_ulTechniqueNameHash == ulHash )
			{
				return XST_OK;
			}

			ITechnique* pTech = GetTechniqueByNameHash( ulHash );
			if( pTech == xst_null )
			{
				XST_LOG_ERR( "Technique: " << strName << " does not exists in material: " << this->GetResourceName() );
				return XST_FAIL;
			}

			m_pCurrentTechnique = pTech;

			return XST_OK;
		}

		VertexShaderPtr CMaterial::GetVertexShader()
		{
			xst_assert( m_pCurrentTechnique != xst_null, "(CMaterial::GetVertexShader)" );
			xst_assert( m_pCurrentTechnique->GetPass( 0 ) != xst_null, "(CMaterial::GetVertexShader)" );
			xst_assert( m_pCurrentTechnique->GetPass( 0 )->GetVertexShader() != xst_null, "(CMaterial::GetVertexShader)" );
			return m_pCurrentTechnique->GetPass( 0 )->GetVertexShader();
		}

		PixelShaderPtr CMaterial::GetPixelShader()
		{
			xst_assert( m_pCurrentTechnique != xst_null, "(CMaterial::GetVertexShader)" );
			xst_assert( m_pCurrentTechnique->GetPass( 0 ) != xst_null, "(CMaterial::GetVertexShader)" );
			xst_assert( m_pCurrentTechnique->GetPass( 0 )->GetPixelShader() != xst_null, "(CMaterial::GetVertexShader)" );
			return m_pCurrentTechnique->GetPass( 0 )->GetPixelShader();
		}

		ITechnique*	CMaterial::GetTechniqueByName(xst_castring& strName)
		{
			ul32 ulHash = XST::CHash::GetCRC( strName );
			return GetTechniqueByNameHash( ulHash );
		}

		ITechnique*	CMaterial::GetTechniqueByHandle(const ITechnique::ID& pulId)
		{
			xst_assert( pulId != 0, "(CMaterial::GetTechnique) The technique id should not be 0" );
			return m_vTechniques[ *pulId ];
		}

		ITechnique* CMaterial::GetTechnique(cu32& uiTechId)
		{
			return GetTechniqueByHandle( (ITechnique::ID)&uiTechId );
		}

		ITechnique*	CMaterial::GetTechniqueByNameHash(cul32& ulHash)
		{
			if( m_pCurrentTechnique && m_pCurrentTechnique->m_ulTechniqueNameHash == ulHash )
			{
				return m_pCurrentTechnique;
			}

			for(u32 i = 0; i < m_vTechniques.size(); ++i)
			{
				if( m_vTechniques[ i ]->m_ulTechniqueNameHash == ulHash )
				{
					return m_vTechniques[ i ];
				}
			}

			return xst_null;
		}

		const ITechnique*	CMaterial::GetTechniqueByName(xst_castring& strName) const
		{
			ul32 ulHash = XST::CHash::GetCRC( strName );
			return GetTechniqueByNameHash( ulHash );
		}

		const ITechnique*	CMaterial::GetTechniqueByHandle(const ITechnique::ID& pulId) const 
		{
			xst_assert( pulId != 0, "(CMaterial::GetTechnique) The technique id should not be 0" );
			return m_vTechniques[ *pulId ];
		}

		const ITechnique* CMaterial::GetTechnique(cu32& uiTechId) const 
		{
			return GetTechniqueByHandle( (ITechnique::ID)&uiTechId );
		}

		const ITechnique*	CMaterial::GetTechniqueByNameHash(cul32& ulHash) const 
		{
			if( m_pCurrentTechnique->m_ulTechniqueNameHash == ulHash )
			{
				return m_pCurrentTechnique;
			}

			for(u32 i = 0; i < m_vTechniques.size(); ++i)
			{
				if( m_vTechniques[ i ]->m_ulTechniqueNameHash == ulHash )
				{
					return m_vTechniques[ i ];
				}
			}

			return xst_null;
		}

		i32 CMaterial::Compare(const CMaterial* pMat) const
		{
			bool bAttribEquals = IPass::AttribEquals( m_Attribs, pMat->m_Attribs );
			if( !bAttribEquals )
				return 1;

			i32 iTechDiff = m_vTechniques.size() - pMat->m_vTechniques.size();
			if( iTechDiff == 0 )
			{
				for( u32 i = 0; i < m_vTechniques.size(); ++i )
				{
					i32 iResult = m_vTechniques[ i ]->Compare( pMat->m_vTechniques[ i ] );
					if( iResult != 0 )
						return iResult;
				}
			}
			else
			{
				return iTechDiff;
			}
			return 0;
		}

		i32 CMaterial::Compare(const MaterialPtr& pMat) const
		{
			return Compare( pMat.GetPtr() );
		}

		i32	CMaterial::SetTexture(MATERIAL_TEXTURE_TYPE eType, xst_castring& strName, xst_castring& strGroup)
		{
			if( !XST_FAILED( IPass::SetTexture( strName, strGroup, &m_apTextures[ eType ] ) ) )
			{
				i32 iResult = 0;
				for( auto& pTech : m_vTechniques )
				{
					iResult += pTech->SetTexture( eType, m_apTextures[ eType ] );
				}
				return iResult;
			}
			return XST_FAIL;
		}
			
		i32	CMaterial::SetTexture(MATERIAL_TEXTURE_TYPE eType, TextureWeakPtr pTex)
		{
			m_apTextures[ eType ] = pTex;
			i32 iResult = 0;
			for( auto& pTech : m_vTechniques )
			{
				iResult += pTech->SetTexture( eType, pTex );
			}
			return iResult;
		}

		void CMaterial::SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const RSHandleRef Handle)
		{
			m_aSamplers[ eType ] = Handle;
			for( auto& pTech : m_vTechniques )
			{
				pTech->SetTextureSamplingMode( eType, Handle );
			}
		}

		void CMaterial::SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const STextureSamplingMode& Mode)
		{
			const RSHandleRef Handle = IPass::GetSamplerHandle( Mode );
			SetTextureSamplingMode(eType, Handle );
		}

	}//resources
}//xse