#ifndef XSE_ITECHNIQUE_H
#define XSE_ITECHNIQUE_H

#include "XSECommon.h"
#include "XSECTexture.h"

namespace XSE
{
	class XST_API ITechnique
	{
		friend class Resources::CMaterial;

		protected:

			typedef xst_vector< IPass* >		PassVector;

		public:

			typedef l32*	ID;

		public:

											ITechnique(xst_castring& strName) : m_strTechniqueName( strName ), m_lTechniqueID( -1 ), m_ulTechniqueNameHash( 0 ) {}
			virtual							~ITechnique();

			virtual	IPass*					CreatePass(xst_castring& strName);

			virtual	i32						DestroyPass(IPass* pPass);

			virtual xst_fi	i32				DestroyPass(xst_castring& strName)
											{ return DestroyPass( GetPass( strName ) ); }

			virtual	i32						DestroyPass(cul32& ulPassId);

			virtual	xst_fi IPass*			GetPass(ul32 ulPassId) const
											{ return m_vPasses[ ulPassId ]; }

			virtual IPass*					GetPass(xst_castring& strName) const;

			virtual xst_fi
			TextureWeakPtr					GetTexture(MATERIAL_TEXTURE_TYPE eType) const
											{ return m_apTextures[ eType ]; }

			virtual	xst_fi ul32				GetPassCount() const
											{ return m_vPasses.size(); }

			virtual	xst_fi xst_castring&	GetName() const
											{ return m_strTechniqueName; }

			virtual ID						GetTechniqueID() const
											{ return (ID)&m_lTechniqueID; }

			virtual	void					SetAmbientColor(const Vec4& vecCol);
			virtual	void					SetDiffuseColor(const Vec4& vecCol);
			virtual	void					SetSpecularColor(const Vec4& vecCol);
			virtual	void					SetShininess(f32 fValue);
			virtual	void					SetAttributes(const SMaterialAttributes& Attribs);
			virtual xst_fi
			const SMaterialAttributes&		GetAttributes() const
											{ return m_Attribs; }

			virtual i32						Compare(const ITechnique* pOther) const;

			virtual	i32						SetTexture(MATERIAL_TEXTURE_TYPE eType, xst_castring& strName, xst_castring& strGroup);
			virtual	i32						SetTexture(MATERIAL_TEXTURE_TYPE eType, TextureWeakPtr pTex);

			virtual void					SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const STextureSamplingMode& Mode);
			virtual void					SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const RSHandleRef Handle);
			virtual void					SetTextureSamplingMode(MATERIAL_TEXTURE_TYPE eType, const RSHandleRef Handle);

			virtual xst_fi
			const RSHandleRef				GetSamplerHandle(MATERIAL_TEXTURE_TYPE eType) const
											{ return m_aSamplers[ eType ]; }

		protected:

			virtual	IPass*					_CreatePass(xst_castring& strName) = 0;
			virtual void					_DestroyPass(IPass* pPass);

		protected:

			SMaterialAttributes	m_Attribs;
			RSHandle			m_aSamplers[ MaterialTextureTypes::_ENUM_COUNT ];
			TexturePtr			m_apTextures[ MaterialTextureTypes::_ENUM_COUNT ];
			xst_castring		m_strTechniqueName;
			PassVector			m_vPasses;
			l32					m_lTechniqueID;
			ul32				m_ulTechniqueNameHash;
	};
}//xse

#endif