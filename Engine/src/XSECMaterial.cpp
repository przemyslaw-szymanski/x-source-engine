#include "../include/XSECMaterial.h"
#include "XSECShaderManager.h"
#include "XSECTechnique.h"
#include "XSECPass.h"

namespace XSE
{
	namespace Resources
	{
		CMaterial::CMaterial(CShaderManager* pShaderMgr, XSE_IRESOURCE_DECL_PARAMS) : 
			XSE_IRESOURCE_CTOR, 
			m_pShaderMgr( pShaderMgr ), 
			m_pCurrentTechnique( xst_null )
		{
			//Create default technique
			CTechnique* pTech = xst_new CTechnique( strName );
			if( pTech == xst_null )
			{
				XST_LOG_ERR( "Failed to create technique: " << strName << " for material: " << this->GetResourceName() << ". Memory error." );
				return;
			}
			pTech->m_lTechniqueID = m_vTechniques.size();
			pTech->m_ulTechniqueNameHash = XST::CHash::GetCRC( strName );
			IPass* pPass = pTech->CreatePass( "pass0" );
			//Set default vertex and pixel shaders
			pPass->SetVertexShader( m_pShaderMgr->GetDefaultVertexShader() );
			pPass->SetPixelShader( m_pShaderMgr->GetDefaultPixelShader() );

			m_vTechniques.push_back( pTech );
			m_pCurrentTechnique = pTech;
		}

		CMaterial::~CMaterial()
		{
			for(TechniqueVector::iterator Itr = m_vTechniques.begin(); Itr != m_vTechniques.end(); ++Itr)
			{
				xst_delete( (*Itr) );
			}

			m_vTechniques.clear();
		}

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName)
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
			return pTech;
		}

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName, const VertexShaderPtr& pVS, const PixelShaderPtr& pPS)
		{
			ITechnique* pTech = CreateTechnique( strName );
			pTech->GetPass( 0 )->SetVertexShader( pVS );
			pTech->GetPass( 0 )->SetPixelShader( pPS );
			return pTech;
		}

		ITechnique*	CMaterial::CreateTechnique(xst_castring& strName, xst_castring& strEffectName)
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
			if( m_vTechniques.size() < pMat->m_vTechniques.size() )
			{
				return -1;
			}
			else if( m_vTechniques.size() > pMat->m_vTechniques.size() )
			{
				return 1;
			}
			else //if size of techniques are equal
			{
				for(u32 t = m_vTechniques.size(); t --> 0;)
				{
					if( m_vTechniques[ t ]->GetPassCount() < pMat->m_vTechniques[ t ]->GetPassCount() )
					{
						return -1;
					}
					else if( m_vTechniques[ t ]->GetPassCount() > pMat->m_vTechniques[ t ]->GetPassCount() )
					{
						return 1;
					}
					else //if pass counts are equal
					{
						for(u32 p = m_vTechniques[ t ]->GetPassCount(); p --> 0;)
						{
							IPass* pPass1 = m_vTechniques[ t ]->GetPass( p );
							IPass* pPass2 = pMat->m_vTechniques[ t ]->GetPass( p );

							if( pPass1->GetVertexShader().GetPtr() > pPass2->GetVertexShader().GetPtr() )
							{
								return 1;
							}
							else if( pPass1->GetVertexShader().GetPtr() < pPass2->GetVertexShader().GetPtr() )
							{
								return -1;
							}
							else
							{
								if( pPass1->GetPixelShader().GetPtr() > pPass2->GetPixelShader().GetPtr() )
								{
									return 1;
								}
								else if( pPass1->GetPixelShader().GetPtr() < pPass2->GetPixelShader().GetPtr() )
								{
									return -1;
								}
								else
								{
									return 0;
								}
							}
						}
					}
				}
			}

			return 0;
		}

		i32 CMaterial::Compare(const MaterialPtr& pMat) const
		{
			return Compare( pMat.GetPtr() );
		}

	}//resources
}//xse