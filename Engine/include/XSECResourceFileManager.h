#ifndef XSE_CRESOURCE_FILE_MANAGER_H
#define XSE_CRESOURCE_FILE_MANAGER_H

#include "XSECommon.h"

namespace XSE
{
	class IResourceManager;
#define XSE_MAX_FILE_PATH_LEN	512
#define XSE_MAX_FILE_NAME_LEN	24
#define XSE_MAX_FILE_EXT_LEN	8

	class IFileSystem;

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

			struct SFileInfo
			{
				enum
				{
					NAME_BUFF_LEN = XSE_MAX_FILE_PATH_LEN + 
									XSE_MAX_FILE_NAME_LEN +
									XSE_MAX_FILE_EXT_LEN
				};

				ch8*			pBuff;
				u16				uPathLen	: 16;
				u16				uNameLen	: 8;
				u16				uExtLen		: 8;
				ul32			uNameHash;
				ul32			uPathHash;
				ul32			uExtHash;
				ul32			uFileSize;
			};

			struct SFileSystem
			{
				IFileSystem*	pSys;
				bool			bAutoDestroy;
			};

		public:

			typedef xst_list< SExt >					ExtList;
			typedef XST::TCDynamicArray< SExt >			ExtArray;
			typedef xst_map< i32, ExtArray >			ResTypeMap;
			typedef XST::TCDynamicArray< SExt >			TypeArray;
			typedef xst_map< i32, IResourceManager* >	ExtMap;
			typedef xst_map< u32, SFileInfo >			FileInfoMap;
			typedef XST::TCDynamicArray< ch8 >			NameArray;
			
			class CGroup : public XST::IObject
			{
				struct SLocation
				{
					xst_astring strDir;
					bool		bRecursive;
				};
				typedef xst_vector< SLocation > LocVec;
				friend class CResourceFileManager;
				public:

										CGroup(xst_castring& strName, ul32 uHandle) : 
											m_strName(strName), m_uHash(uHandle)
										{ m_aNames.reserve( SFileInfo::NAME_BUFF_LEN * 500 ); }

					i32					AddFileInfo(xst_castring& strFullPath);
					i32					AddFileInfo(xst_castring& strDirPath, xst_castring& strName, xst_castring& strExt);

					const SFileInfo&	GetFileInfoByName(xst_castring& strFileName) const;
					const SFileInfo&	GetFileInfoByPath(xst_castring& strFullPath) const;

					lpcastr				GetFileDirPath(const SFileInfo& Info);
					lpcastr				GetFileName(const SFileInfo& Info);
					lpcastr				GetFileExt(const SFileInfo& Info);

					xst_fi void			SetFileSystem(IFileSystem* pFS)
										{ m_pFS = pFS; }

					xst_fi IFileSystem*	GetFileSystem() const
										{ return m_pFS; }

					xst_fi	void		AddLocation(xst_castring& strDir, bool bRecursive)
										{ m_vLocations.push_back( { strDir, bRecursive } ); }

					i32					Prepare();

					xst_fi	bool		IsPrepared() const
										{ return m_bPrepared; }

				protected:

					FileInfoMap		m_mFileInfos;
					IFileSystem*	m_pFS;
					NameArray		m_aNames;
					xst_astring		m_strName;
					LocVec			m_vLocations;
					ul32			m_uHash;
					bool			m_bPrepared = false;
			};

			static const i32			DEFAULT_ORDER = -1;
			typedef XST::TCObjectSmartPointer< CGroup > GroupPtr;
			typedef XST::TCWeakPointer< CGroup >		GroupWeakPtr;
			typedef xst_map< u32, GroupPtr >			GroupMap;
			typedef xst_map< u32, XST::IFileLoader* >	FileLoaderMap;
			typedef xst_map< u32, SFileSystem >			FilesSystemMap;

		public:

										CResourceFileManager(XST::CFileManager* pFileMgr);
			virtual						~CResourceFileManager();

					i32					AddExtension(xst_castring& strExt, i32 iResourceType, i32 iLoadingOrder, IResourceManager* pMgr);

					i32					SetManager(i32 iResourceType, IResourceManager* pMgr);
					i32					AddExtension(xst_castring& strExt, i32 iResourceType);
					i32					SetLoadingOrder(i32 iResourceType, i32 iLoadingOrder = DEFAULT_ORDER);

					i32					AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive = false);
					i32					AddLocation2(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive = false);
					i32					PrepareGroup(xst_castring& strName);

					i32					PrepareAllGroups();

					i32					RegisterLoader(xst_castring& strLoaderName, XST::IFileLoader* pLoader);

					IFileSystem*		RegisterFileSystem(xst_castring& strName, IFileSystem* pSys, bool bAutoDestroy);
					IFileSystem*		GetFileSystem(xst_castring& strName) const;

					XST::FilePtr		LoadFile(xst_castring& strFileName, xst_castring& strGroupName);

					XST::FilePtr		LoadFromFileSystem(xst_castring& strDirPath, xst_castring& strFileName, xst_castring& strGroupName);

					void				AddListener(XST::IFileListener* pListener);

					i32					RemoveListener(XST::IFileListener* pListener);
					
					GroupWeakPtr		GetOrCreateGroup(xst_castring& strName);

		protected:

					XST::IFileLoader*	_GetLoader(xst_castring& strName);

		protected:

			XST::CFileManager*	m_pFileMgr;
			TypeArray			m_aResTypes;
			GroupMap			m_mGroups;
			FileLoaderMap		m_mLoaders;
			FilesSystemMap		m_mFileSystems;
	};

}//xse

#endif