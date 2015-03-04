#ifndef XSE_CMATERIAL_H
#define XSE_CMATERIAL_H

#include "XSEIResource.h"
#include "XSEIVertexShader.h"
#include "XSEIPixelShader.h"
#include "XSECTexture.h"
#include "XSEITechnique.h"

namespace XSE
{
	namespace Resources
	{
		using namespace XST::Math;

		class XST_API CMaterial : public IResource
		{
			friend class XSE::CLuaMaterialParser;
			friend class XSE::CBinaryMaterialReader;
			friend class XSE::CLuaMaterialReader;

			protected:

				typedef xst_vector< ITechnique* >			TechniqueVector;

			public:

				typedef XST::TCObjectSmartPointer< CMaterial >	MaterialPtr;
				typedef XST::TCWeakPointer< CMaterial >			MaterialWeakPtr;
				typedef XST::TCIterator< TechniqueVector >		TechniqueIterator;
				typedef XST::TCConstIterator< TechniqueVector >	ConstTechniqueIterator;

			public:

											CMaterial(CShaderManager* pShaderMgr, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
											CMaterial(CShaderManager* pShaderMgr, xst_castring& strDefaultTechniqueName, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual						~CMaterial();

						ITechnique*			CreateTechnique(xst_castring& strName, bool bSetAsCurrent = true);
						ITechnique*			CreateTechnique(xst_castring& strName, const VertexShaderPtr& pVS, const PixelShaderPtr& pPS, bool bSetAsCurrent = true);
						ITechnique*			CreateTechnique(xst_castring& strName, xst_castring& strEffectName, bool bSetAsCurrent = true); //For directx or cg

						ConstTechniqueIterator	GetTechniqueIterator() const;
						TechniqueIterator	GetTechniqueIterator();

				xst_fi	ITechnique*			GetCurrentTechnique()
											{ return m_pCurrentTechnique; }

				xst_fi	const ITechnique*	GetCurrentTechnique() const
											{ return m_pCurrentTechnique; }

						ITechnique*			GetTechniqueByName(xst_castring& strName);
						ITechnique*			GetTechniqueByNameHash(cul32& ulNameHash);
						ITechnique*			GetTechniqueByHandle(const ITechnique::ID& pulId);
						ITechnique*			GetTechnique(cu32& uiTechId);
				const	ITechnique*			GetTechniqueByName(xst_castring& strName) const;
				const	ITechnique*			GetTechniqueByNameHash(cul32& ulNameHash) const;
				const	ITechnique*			GetTechniqueByHandle(const ITechnique::ID& pulId) const;
				const	ITechnique*			GetTechnique(cu32& uiTechId) const;

				xst_fi	u32					GetTechniqueCount() const
											{ return m_vTechniques.size(); }

						i32					UseTechnique(xst_castring& strName);

						i32					Compare(const CMaterial* pMat) const;
						i32					Compare(const MaterialPtr& pMat) const;

									/*XSE_PROP_V3(CVector3, Ambient, m_vecAmbient);
									XSE_PROP_V3(CVector3, Diffuse, m_vecDiffuse);
									XSE_PROP_V3(CVector4, Specular, m_vecSpecular);*/

						VertexShaderPtr		GetVertexShader();
										
						PixelShaderPtr		GetPixelShader();

				xst_fi	TextureWeakPtr		GetTexture(MATERIAL_TEXTURE_TYPE eType) const
											{ return m_apTextures[ eType ]; }
						
						void				SetAmbientColor(const Vec4& vecCol);
						void				SetDiffuseColor(const Vec4& vecCol);
						void				SetSpecularColor(const Vec4& vecCol);
						void				SetShininess(f32 fValue);
						void				SetAttributes(const SMaterialAttributes& Attribs);
				xst_fi
				const SMaterialAttributes&	GetAttributes() const
											{ return m_Attribs; }

						i32					SetTexture(MATERIAL_TEXTURE_TYPE eType, xst_castring& strName, xst_castring& strGroup);
						i32					SetTexture(MATERIAL_TEXTURE_TYPE eType, TextureWeakPtr pTex);

						void				SetSamplingMode(MATERIAL_TEXTURE_TYPE eType, const STextureSamplingMode& Mode);

			protected:

				SMaterialAttributes		m_Attribs;

				STextureSamplingMode	m_aSamplingModes[ MaterialTextureTypes::_ENUM_COUNT ];
				TexturePtr				m_apTextures[ MaterialTextureTypes::_ENUM_COUNT ];

				CShaderManager*			m_pShaderMgr;

				TechniqueVector			m_vTechniques;
				ITechnique*				m_pCurrentTechnique;
		};

	}//resources

	typedef Resources::CMaterial::MaterialPtr		MaterialPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< Resources::CMaterial >;
	typedef Resources::CMaterial::MaterialWeakPtr	MaterialWeakPtr;
	XST_TEMPLATE_CLASS XST::TCWeakPointer< Resources::CMaterial >;

}//xse

#endif
