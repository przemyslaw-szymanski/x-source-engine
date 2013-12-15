#ifndef XSE_IPASS_H
#define XSE_IPASS_H

#include "XSECommon.h"
#include "XSEIVertexShader.h"
#include "XSEIPixelShader.h"

namespace XSE
{
	class XST_API IPass
	{
		friend class ITechnique;

		public:

			typedef l32*	ID;

		public:

												IPass(xst_castring& strName) : m_strPassName( strName ), m_lPassId( -1 ), m_ulPassNameHash( 0 ) {}
			virtual								~IPass() {}

			//virtual	void						SetVertexShader(Resources::IVertexShader* pShader);
			virtual xst_fi void					SetVertexShader(VertexShaderPtr pShader)
												{ m_pVS = pShader; }

			//virtual	void						SetPixelShader(PixelShaderPtr pShader);
			virtual xst_fi void					SetPixelShader(PixelShaderPtr pShader)
												{ m_pPS = pShader; }

			virtual VertexShaderPtr				GetVertexShader()
												{ return m_pVS; }

			virtual	PixelShaderPtr				GetPixelShader()
												{ return m_pPS; }

			virtual	ID							GetPassID() const
												{ return (ID)&m_lPassId; }

		protected:

			xst_castring	m_strPassName;
			l32				m_lPassId;
			ul32			m_ulPassNameHash;

			VertexShaderPtr	m_pVS;
			PixelShaderPtr	m_pPS;
	};

}//xse

#endif