#ifndef XST_CSYSTEM_FILE_LOADER_H
#define XST_CSYSTEM_FILE_LOADER_H

#include "XSTIFileLoader.h"
#include "XSTCBinaryReader.h"

namespace XST
{
	class CFileSystemLoader : public IFileLoader
	{
		public:

										CFileSystemLoader();
			virtual						~CFileSystemLoader();

			virtual i32					Init(xst_castring& strDefaultDir); 

			virtual	Resources::FilePtr	Load(xst_castring& strDirName, xst_castring& strFileName);

			virtual Resources::FilePtr	Load(xst_castring& strFullPath);

			virtual Resources::FilePtr	GetFileInfo(xst_castring& strDirName, xst_castring& strFileName);

			virtual Resources::FilePtr	GetFileInfo(xst_castring& strFullPath);

			virtual Resources::CDirectory::FileMap GetFileInfos(xst_castring& strDirName, bool bRecursive = false);

			virtual bool				IsReady()
										{ return !m_pRoot.IsNull(); }

			virtual IFileLoader*		CreateInstance()
										{ return xst_new CFileSystemLoader(); }

		protected:

			XST::CBinaryReader				m_Reader;
			Resources::CDirectory::FileMap	m_mFiles;
			Resources::DirectoryPtr			m_pRoot;
	};

}//xst

#endif