#ifndef XST_IFILE_LOADER_H
#define XST_IFILE_LOADER_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTTCSmartPointer.h"
#include "XSTTCData.h"
#include "XSTCStringUtil.h"

//Disable warning that stl containers are not exported
//#pragma warning( disable: 4251 )

namespace XST
{
	class IFileLoader;
	class CFileSystemLoader;
	class CVirtualFileSystemLoader;
	class CFileManager;

	namespace Resources
	{
		class CDirectory;

		class XST_API CFile : public XST::IObject
		{
			friend class XST::IFileLoader;
			friend class XST::CVirtualFileSystemLoader;
			friend class XST::CFileSystemLoader;
			friend class XST::CFileManager;

			public:

				typedef XST::TCObjectSmartPointer<CFile>		FilePtr;

			public:

										CFile(xst_castring& strFileName, xst_castring& strPath = StringUtil::EmptyAString, u8* pData = xst_null, cul32& ulDataSize = 0, cul32& ulCompressedSize = 0, cul32& ulOffset = 0);
										CFile(CDirectory* pParentDir, xst_castring& strFileName, u8* pData, cul32& ulDataSize, cul32& ulCompressedSize = 0, cul32& ulOffset = 0);

										//CFile(const CFile& File);
				virtual					~CFile();

				xst_fi	TCData<u8>&		GetData()
										{ return m_Data; }

				xst_fi	u32				GetSize() const
										{ return IsLoaded() ? m_Data.GetSize() : m_ulSize; }

				xst_fi	void			DestroyData()
										{ m_Data.Delete(); }

				xst_fi	ul32			GetCompressedSize() const
										{ return m_ulCompSize; }

				xst_fi	xst_castring	GetName() const
										{ return m_strName; }

						xst_astring		GetExtension() const;

				xst_fi	bool			IsLoaded() const
										{ return m_Data.IsLoaded(); }

				xst_fi	xst_castring&	GetPath() const
										{ return m_strPath; }

			protected:

				CDirectory*		m_pParentDir;
				xst_astring		m_strName;
				xst_astring		m_strPath;
				TCData<u8>		m_Data;
				ul32			m_ulSize;
				ul32			m_ulCompSize;
				ul32			m_ulOffset;
		};

		typedef XST::TCObjectSmartPointer<CFile>		FilePtr;
		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< CFile >;


		class XST_API CDirectory : public XST::IObject
		{
			friend class XST::IFileLoader;
			friend class XST::CVirtualFileSystemLoader;
			friend class XST::CFileSystemLoader;
			friend class XST::CFileManager;

			public:

				typedef XST::TCObjectSmartPointer<CFile>							FilePtr;
				typedef XST::TCObjectSmartPointer<CDirectory>						DirPtr;
				xst_tmapx( xst_astring, XST::TCObjectSmartPointer< CFile > )		FileMap;
				xst_tmapx( xst_astring, XST::TCObjectSmartPointer< CDirectory > )	DirMap;

			public:

											CDirectory(CDirectory* pParentDir, xst_castring& strName, xst_castring& strPath = StringUtil::EmptyAString);
				virtual						~CDirectory();

				xst_fi	const FileMap&		GetFiles()
											{ return m_mFiles; }

				xst_fi	const DirMap&		GetDirectories()
											{ return m_mDirs; }

				xst_fi	xst_castring&		GetName()
											{ return m_strName; }

				xst_fi	xst_castring&		GetPath()
											{ return m_strPath; }

				xst_fi	xst_castring&		GetName() const
											{ return m_strName; }

						DirPtr				FindDirectory(xst_castring& strName);

						FilePtr				FindFile(xst_castring& strFileName);


						i32					AddDirectory(DirPtr pDir);

						i32					AddFile(FilePtr pFile);

			protected:

				CDirectory*		m_pParentDir;
				FileMap			m_mFiles;
				DirMap			m_mDirs;
				xst_astring		m_strName;
				xst_astring		m_strPath;
				ul32			m_ulDirCount;
				ul32			m_ulFileCount;
		};

		typedef XST::TCObjectSmartPointer<CDirectory>	DirectoryPtr;
		XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< CDirectory >;

	}//resources

	typedef Resources::FilePtr	FilePtr;

	class XST_API IFileLoaderListener
	{
		public:

			virtual			~IFileLoaderListener() {}

			virtual	bool	OnBeforeLoad(xst_castring& strDirName, xst_castring& strFileName) { return true; }
			virtual void	OnAfterLoad(Resources::FilePtr pFile) { }

		protected:
	};

	class CFileManager;

	class XST_API IFileLoader
	{
		friend class CFileManager;

		public:

			xst_tvectorx( Resources::FilePtr )		FileVector;
			xst_tvectorx( IFileLoaderListener* )	ListenerVector;

		public:

                                        IFileLoader() : m_bDestroyListeners( false ) {}
										IFileLoader(bool bDestroyListeners) : m_bDestroyListeners( bDestroyListeners ) {}
			virtual						~IFileLoader();

			virtual i32					Init(xst_castring& strDefaultDir = XST::StringUtil::EmptyAString) = 0;

			virtual	Resources::FilePtr	Load(xst_castring& strDirName, xst_castring& strFileName) = 0;

			virtual Resources::FilePtr	Load(xst_castring& strFullPath) = 0;

			virtual Resources::FilePtr	GetFileInfo(xst_castring& strDirName, xst_castring& strFileName) = 0;

			virtual Resources::FilePtr	GetFileInfo(xst_castring& strFullPath) = 0;

			virtual Resources::CDirectory::FileMap GetFileInfos(xst_castring& strDirName, bool bRecursive = false) = 0;

					void				AddListener(IFileLoaderListener* pListener)
										{ m_vListeners.push_back( pListener ); }

					xst_castring&		GetName() const
										{ return m_strLoaderName; }

			virtual	bool				IsReady() = 0;

			virtual IFileLoader*		CreateInstance() = 0;

			virtual void				DestroyInstance(IFileLoader** ppLoader)
										{ xst_delete( *ppLoader ); *ppLoader = 0; }

		protected:

			xst_astring					m_strLoaderName;
			ListenerVector				m_vListeners;
			bool						m_bDestroyListeners;
	};

}//xst

//Enable this warning


#endif
