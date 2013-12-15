#ifndef XST_CVIRTUAL_FILE_SYSTEM_H
#define XST_CVIRTUAL_FILE_SYSTEM_H

#include "XSTIFileLoader.h"
#include "XSTCBinaryReader.h"
#include "XSTIThread.h"
#include "XSTCTimer.h"
#include "XSTIArchive.h"

namespace XST
{
	class CVirtualFileSystemLoader : public IFileLoader
	{
		public:

			typedef xst_vector< Resources::FilePtr >	FileVector;

		public:
											CVirtualFileSystemLoader(bool bFileBuffer, xst_castring& strArchiveDll);
											CVirtualFileSystemLoader(bool bFileBuffer, IArchive* pArchive, bool bDestroyArchive);
			virtual							~CVirtualFileSystemLoader();

			i32								Init(xst_castring& strDefaultDir);

			Resources::FilePtr				Load(xst_castring& strDirName, xst_castring& strFileName);

			Resources::FilePtr				Load(xst_castring& strFullPath);

			Resources::FilePtr				GetFileInfo(xst_castring& strDirName, xst_castring& strFileName);

			Resources::FilePtr				GetFileInfo(xst_castring& strFullPath);

			Resources::CDirectory::FileMap	GetFileInfos(xst_castring& strDirName, bool bRecursive = false);

			i32								Open(xst_castring& strArchiveName);
					
			void							Close();

			Resources::FilePtr				LoadFile(xst_castring& strDirPath, xst_castring& strFileName);

			Resources::DirectoryPtr			FindDirectory(xst_castring& strFullPath);

			const Resources::DirectoryPtr	GetRootDir() const
											{ return m_pRootDir; }

			const FileVector&				GetAllFiles();

			xst_fi	void					SetArchive(IArchive* pArchive)
											{ m_pArchive = pArchive; }

			bool							IsReady()
											{ return m_bIsReady; }

			IFileLoader*					CreateInstance();

		protected:

			i32						ReadBlock(XST::CBinaryReader& Reader, ul32* pulOffset);
			
			i32						ReadFileInfo(Resources::CDirectory* pParentDir, XST::CBinaryReader& Reader);

			i32						ReadDirInfo(Resources::CDirectory* pParentDir, XST::CBinaryReader& Reader);

			Resources::DirectoryPtr	FindDirectory(Resources::DirectoryPtr CurrDir, xst_castring& strDirName);

			void					AppendFiles(Resources::DirectoryPtr pDir, ul32* pulLargestSize);

			Resources::FilePtr		LoadFileInBackground(Resources::DirectoryPtr pDir, xst_castring& strFileName);

			Resources::FilePtr		LoadFileInMainThread(Resources::DirectoryPtr pDir, xst_castring& strFileName);

			Resources::FilePtr		LoadFile(Resources::DirectoryPtr pDir, xst_castring& strFileName, u8* pCompBuff, cul32& ulBuffSize);


		protected:

			XST::CBinaryReader	m_Reader;

			Resources::DirectoryPtr	m_pRootDir;
			TCData<u8>				m_Buffer;
			bool					m_bFileBuffer;
			FileVector				m_vAllFiles;
			ul32					m_ulLargestFileCompressedSize;
			//ThreadVector			m_vThreads;
			//CVFSLoadingThread*		m_pDefaultThread;
			u32						m_uiTestNum;
			XST::CTimer				m_Timer;
			XST::CCriticalSection	m_CritSection;
			//xst_unknown				m_hZLibDll;
			IArchive*				m_pArchive;
			IArchive*				m_pDefaultArchive;
			bool					m_bIsReady;
			bool					m_bDestroyArchive;
	};

}//xst

#endif