#include "XSEITechnique.h"
#include "XSEIPass.h"

namespace XSE
{
	ITechnique::~ITechnique()
	{
		for(PassVector::iterator Itr = m_vPasses.begin(); Itr != m_vPasses.end(); ++Itr)
		{
			_DestroyPass( (*Itr) ) ;
		}

		m_vPasses.clear();
	}

	void ITechnique::_DestroyPass(IPass* pPass)
	{
		xst_delete( pPass );
	}

	IPass* ITechnique::CreatePass(xst_castring &strName)
	{
		IPass* pPass = GetPass( strName);
		if( pPass == xst_null )
		{
			pPass = _CreatePass( strName );
			if( pPass == xst_null )
			{
				return xst_null;
			}
		}
		else
		{
			//If this pass exists return it
			return pPass;
		}

		//If new pass was created add it
		pPass->m_ulPassNameHash = XST::CHash::GetCRC( strName );
		pPass->m_lPassId = m_vPasses.size();
		m_vPasses.push_back( pPass );

		return pPass;
	}

	i32 ITechnique::DestroyPass(cul32& ulPassId)
	{
		IPass* pPass = GetPass( ulPassId );

		//Get iterator
		PassVector::iterator Itr = m_vPasses.begin();
		std::advance( Itr, ulPassId );
		//Erase
		m_vPasses.erase( Itr );
		_DestroyPass( pPass );
		//Calculate new id's for passes
		for(u32 i = 0; i < m_vPasses.size(); ++i)
		{
			m_vPasses[ i ]->m_lPassId = i;
		}

		return XST_OK;
	}

	i32 ITechnique::DestroyPass(IPass* pPass)
	{ 
		return DestroyPass( pPass->m_lPassId ); 
	}

	IPass* ITechnique::GetPass(xst_castring &strName) const
	{
		ul32 ulHash = XST::CHash::GetCRC( strName );
		for(PassVector::const_iterator Itr = this->m_vPasses.begin(); Itr != this->m_vPasses.end(); ++Itr)
		{
			if( (*Itr)->m_ulPassNameHash == ulHash )
			{
				return (*Itr);
			}
		}

		return xst_null;
	}

	i32 ITechnique::Compare(const ITechnique* pOther) const
	{
		bool bAttrEq = IPass::AttribEquals( m_Attribs, pOther->m_Attribs );
		if( !bAttrEq )
			return 1;
		i32 iPassDiff = m_vPasses.size() - pOther->m_vPasses.size();
		if( iPassDiff == 0 )
		{
			for( u32 i = m_vPasses.size(); i-- > 0; )
			{
				i32 iResult = m_vPasses[i]->Compare( pOther->m_vPasses[i] );
				if( iResult != 0 )
					return iResult;
			}
		}
		else
		{
			return iPassDiff;
		}
		return 0;
	}

	void ITechnique::SetAmbientColor(const Vec4& vecCol)
	{
		m_Attribs.vecAmbientColor = vecCol;
		for( auto* pPass : m_vPasses )
		{
			pPass->SetAmbientColor( vecCol );
		}
	}
		
	void ITechnique::SetDiffuseColor(const Vec4& vecCol)
	{
		m_Attribs.vecAmbientColor = vecCol;
		for( auto* pPass : m_vPasses )
		{
			pPass->SetDiffuseColor( vecCol );
		}
	}
		
	void ITechnique::SetSpecularColor(const Vec4& vecCol)
	{
		m_Attribs.vecAmbientColor = vecCol;
		for( auto* pPass : m_vPasses )
		{
			pPass->SetSpecularColor( vecCol );
		}
	}
		
	void ITechnique::SetShininess(f32 fValue)
	{
		m_Attribs.vecAmbientColor = fValue;
		for( auto* pPass : m_vPasses )
		{
			pPass->SetShininess( fValue );
		}
	}

	void ITechnique::SetAttributes(const SMaterialAttributes& Attribs)
	{
		m_Attribs = Attribs;
		for( auto* pPass : m_vPasses )
		{
			pPass->SetAttributes( Attribs );
		}
	}

}//xse