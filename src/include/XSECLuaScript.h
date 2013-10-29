#ifndef XSE_CLUASCRIPT_H
#define XSE_CLUASCRIPT_H

#include "XSEIResource.h"
#include "XSEMemoryAllocators.h"

namespace XSE
{
	namespace Resources
	{
		class XST_API CLuaScript : public IResource, public XST::TIAllocableObject< LuaScriptAllocator >
		{
			friend class XSE::CLuaScriptManager;
			friend class XSE::IResourceManager;
			public:

									CLuaScript();
									CLuaScript(XST::CLuaApi* pApi, XSE_IRESOURCE_DECL_PARAMS_DEFAULT);
				virtual				~CLuaScript();

						i32			Load();

						i32			Load(XST::FilePtr pFile);

						i32			Load(cu8* pData, cul32& ulDataSize);

						i32			Compile();

						void		Unload();

				xst_fi	CLuaApi&	GetApi()
									{ return *m_pApi; }

				xst_fi	bool		IsLoaded()
									{ return m_bIsLoaded; }

				xst_fi	bool		IsCompiled()
									{ return m_bIsCompiled; }

			protected:

				XST::CLuaApi*	m_pApi;
				bool			m_bIsLoaded;
				bool			m_bIsCompiled;
		};

	}//resources

	typedef XST::TCObjectSmartPointer< Resources::CLuaScript >	LuaScriptPtr;

}//xse

#endif
