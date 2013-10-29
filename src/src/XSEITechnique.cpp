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



}//xse