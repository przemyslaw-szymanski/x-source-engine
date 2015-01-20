#ifndef XSE_ITECHNIQUE_H
#define XSE_ITECHNIQUE_H

#include "XSECommon.h"

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

		protected:

			virtual	IPass*					_CreatePass(xst_castring& strName) = 0;
			virtual void					_DestroyPass(IPass* pPass);

		protected:

			SMaterialAttributes	m_Attribs;
			xst_castring		m_strTechniqueName;
			PassVector			m_vPasses;
			l32					m_lTechniqueID;
			ul32				m_ulTechniqueNameHash;
	};
}//xse

#endif