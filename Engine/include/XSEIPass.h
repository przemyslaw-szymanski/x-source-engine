#ifndef XSE_IPASS_H
#define XSE_IPASS_H

#include "XSECommon.h"
#include "XSEIVertexShader.h"
#include "XSEIPixelShader.h"
#include "XSECTexture.h"

namespace XSE
{
	class XST_API IPass
	{
		friend class ITechnique;

		public:

			typedef l32*	ID;

		public:

												IPass(xst_castring& strName);
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

			virtual xst_fi
			TextureWeakPtr						GetTexture(MATERIAL_TEXTURE_TYPE eType) const
												{ return m_apTextures[ eType ]; }


			virtual	xst_fi	void				SetAmbientColor(const Vec4& vecCol)
												{ m_Attribs.vecAmbientColor = vecCol; }
			virtual	xst_fi	void				SetDiffuseColor(const Vec4& vecCol)
												{ m_Attribs.vecDiffuseColor = vecCol; }
			virtual	xst_fi	void				SetSpecularColor(const Vec4& vecCol)
												{ m_Attribs.vecSpecularColor = vecCol; }
			virtual	xst_fi	void				SetShininess(f32 fValue)
												{ m_Attribs.fShininess = fValue; }
			virtual	xst_fi	void				SetAttributes(const SMaterialAttributes& Attribs)
												{ m_Attribs = Attribs; }

			virtual xst_fi	
			const	SMaterialAttributes&		GetAttributes() const
												{ return m_Attribs; }

			virtual i32							Compare(const IPass* pOther) const;

			static bool							AttribEquals(const SMaterialAttributes& Left, const SMaterialAttributes& Right);

			virtual	i32							SetTexture(MATERIAL_TEXTURE_TYPE eType, xst_castring& strName, xst_castring& strGroup);
			virtual	i32							SetTexture(MATERIAL_TEXTURE_TYPE eType, TextureWeakPtr pTex);

			static	i32							SetTexture(xst_castring& strName, xst_castring& strGroup, TexturePtr* ppOut);

			virtual void						SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const STextureSamplingMode& Mode);
			virtual xst_fi	void				SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const RSHandleRef Handle)
												{ m_aSamplers[ eType ] = Handle; }

			virtual xst_fi
			const RSHandleRef					GetSamplerHandle(MATERIAL_TEXTURE_TYPE eType) const
												{ return m_aSamplers[ eType ]; }

			static i32							GetSamplerHandle(const STextureSamplingMode& Mode, RSHandlePtr pOut);

		protected:

			SMaterialAttributes		m_Attribs;

			RSHandle				m_aSamplers[ MaterialTextureTypes::_ENUM_COUNT ];
			TexturePtr				m_apTextures[ MaterialTextureTypes::_ENUM_COUNT ];

			xst_castring			m_strPassName;
			l32						m_lPassId;
			ul32					m_ulPassNameHash;

			VertexShaderPtr			m_pVS;
			PixelShaderPtr			m_pPS;
	};

}//xse

#endif