#ifndef XST_CLUA_SCRIPT_H
#define XST_CLUA_SCRIPT_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"
#include "XSTIResource.h"

namespace XST
{
	class CLuaScriptManager;
	class CLuaApi;

	class XST_API CLuaScriptAllocator
	{
		public:

			virtual						~CLuaScriptAllocator() {}

			static	void*				Allocate(size_t uiSize);

			static	void*				Allocate(size_t uiSize, const char* lpszFile, u32 uiLine);

			static	void*				Allocate(size_t uiSize, u32 uiBlock, const char* lpszFile, u32 uiLine);

			static	void*				Allocate(size_t uiSize, void* pPtr);

			static	void*				Allocate(size_t uiSize, const std::nothrow_t& nt);

			static	void				Deallocate(void* pPtr);
	};

	namespace Resources
	{
		class XST_API CLuaScript : public IResource, public TIAllocableObject< CLuaScriptAllocator >
		{
			friend class XST::CLuaScriptManager;

			public:

										//CLuaScript() : m_pApi( xst_null ) {}
										CLuaScript(CLuaApi* pApi, XST_IRESOURCE_DECL_PARAMS);
				virtual					~CLuaScript();

						i32				Load(xst_castring& strFileName);

						i32				Load(xst_castring& strFileName, xst_castring& strGroupName);

						void			Unload();

				xst_fi	CLuaApi*		GetApi()
										{ return m_pApi; }

			protected:

				xst_fi	void			_SetFile(FilePtr pFile)
										{ this->m_pResourceFile = pFile; }

			protected:

				CLuaApi*		m_pApi;

		};

	}//resouces

	typedef TCObjectSmartPointer< Resources::CLuaScript >	LuaScriptPtr;

}//xst

#endif
