#ifndef XSE_CRESOURCE_FILE_MANAGER_H
#define XSE_CRESOURCE_FILE_MANAGER_H

#include "XSECommon.h"

namespace XSE
{
	class IResourceManager;

	class XST_API CResourceFileManager : public XST::TCSingleton< CResourceFileManager >
	{
		private:

			struct SExt
			{
				xst_astring			strExt;
				IResourceManager*	pResMgr;
				i32					iLoadingOrder;

				bool operator<(const SExt& Left)
				{
					return iLoadingOrder < Left.iLoadingOrder;
				}
			};

		public:

			typedef xst_list< SExt >					ExtList;
			typedef XST::TCDynamicArray< SExt >			ExtArray;
			typedef xst_map< i32, ExtArray >			ResTypeMap;
			typedef XST::TCDynamicArray< SExt >			TypeArray;
			typedef xst_map< i32, IResourceManager* >	ExtMap;

			static const i32			DEFAULT_ORDER = -1;

		public:

										CResourceFileManager(XST::CFileManager* pFileMgr);
			virtual						~CResourceFileManager();

					i32					AddExtension(xst_castring& strExt, i32 iResourceType, i32 iLoadingOrder, IResourceManager* pMgr);

					i32					SetManager(i32 iResourceType, IResourceManager* pMgr);
					i32					AddExtension(xst_castring& strExt, i32 iResourceType);
					i32					SetLoadingOrder(i32 iResourceType, i32 iLoadingOrder = DEFAULT_ORDER);

					i32					AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive = false);

					i32					PrepareGroup(xst_castring& strName);

					i32					PrepareAllGroups();

					i32					RegisterLoader(xst_castring& strLoaderName, XST::IFileLoader* pLoader);

					XST::FilePtr		LoadFile(xst_castring& strFileName, xst_castring& strGroupName);

					XST::FilePtr		LoadFromFileSystem(xst_castring& strDirPath, xst_castring& strFileName, xst_castring& strGroupName);

					void				AddListener(XST::IFileListener* pListener);

					i32					RemoveListener(XST::IFileListener* pListener);

		protected:

			XST::CFileManager*	m_pFileMgr;
			TypeArray			m_aResTypes;
	};

}//xse

#endif