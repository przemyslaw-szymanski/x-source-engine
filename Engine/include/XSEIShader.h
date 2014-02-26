#ifndef XSE_ISHADER_H
#define XSE_ISHADER_H

#include "XSEIResource.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{
	namespace Resources
	{
		class IShader : public XSE::Resources::IResource
		{
			friend class CShaderManager;
			friend class IShaderSystem;
			friend class IRenderSystem;

			public:

										IShader(IShaderSystem* pSS, SHADER_TYPE eType, XSE_IRESOURCE_DECL_PARAMS) : m_eProfile( ShaderProfiles::UNKNOWN ), m_pShaderSystem( pSS ), m_eShaderType( eType ), XSE_IRESOURCE_CTOR {}
				
				virtual					~IShader() {}

				//virtual i32				Load(xst_castring& strFileName, xst_castring& strGroupName) = 0; 

				virtual i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile) = 0;

				//Compile directly from code
				virtual i32				Compile(xst_castring& strEntryPoint, SHADER_PROFILE eProfile, xst_castring& strCode) = 0;

				virtual IRenderSystem*	GetRenderSystem() = 0;

				virtual bool			IsCompiled() = 0;

				virtual xst_castring&	GetEntryPoint() const
										{ return m_strShaderEntryPoint; }

				virtual SHADER_PROFILE	GetProfile() 
										{ return m_eProfile; }

				virtual SHADER_TYPE		GetShaderType()
										{ return m_eShaderType; }

				virtual SHADER_LANGUAGE	GetShaderLanguage()
										{ return m_eShaderLanguage; }

			protected:

				virtual	void			_Destroy() = 0;

			protected:

				SHADER_TYPE		m_eShaderType;
				SHADER_LANGUAGE	m_eShaderLanguage;
				IShaderSystem*	m_pShaderSystem;
				SHADER_PROFILE	m_eProfile;
				xst_astring		m_strShaderEntryPoint;
		};

	}//resources

	typedef XST::TCObjectSmartPointer< Resources::IShader >	ShaderPtr;
    //typedef XST::TCWeakPointer< Resources::IShader> ShaderWeakPtr;

}//xse

#endif //XSE_ISHADER_H