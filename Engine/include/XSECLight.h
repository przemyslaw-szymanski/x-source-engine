#ifndef XSE_CLIGHT_H
#define XSE_CLIGHT_H

#include "XSECObject.h"

namespace XSE
{
	struct SLightAttributes
	{
		Vec4	vecColor;
		Vec4	vecSpecularColor;
		f32		fSpecularPower;
		f32		fPower;
	};

	class CLight : public CObject
	{
		public:
								CLight() = delete;
								CLight(xst_castring& strName, ul32 uHandle, CObject* pParent);

			virtual				~CLight();
			
			xst_fi	void		SetAttributes(const SLightAttributes& Attribs)
								{ m_Attribs = Attribs; }

			xst_fi	const 
			SLightAttributes&	GetAttributes() const
								{ return m_Attribs; }

			xst_fi	void		SetColor(const Vec4& vecVal)
								{ m_Attribs.vecColor = vecVal; }

			xst_fi
			const Vec4&			GetColor() const
								{ return m_Attribs.vecColor; }

			xst_fi	void		SetSpecularColor(const Vec4& vecVal)
								{ m_Attribs.vecSpecularColor = vecVal; }

			xst_fi
			const Vec4&			GetSpecularColor() const
								{ return m_Attribs.vecSpecularColor; }

			xst_fi	void		SetPower(f32 fValue)
								{ m_Attribs.fPower = fValue; }

			xst_fi f32			GetPower() const
								{ return m_Attribs.fPower; }

			xst_fi	void		SetSpecularPower(f32 fValue)
								{ m_Attribs.fSpecularPower = fValue; }

			xst_fi f32			GetSpecularPower() const
								{ return m_Attribs.fSpecularPower; }

		protected:

			SLightAttributes	m_Attribs;
	};
} // xse

#endif // XSE_CLIGHT_H