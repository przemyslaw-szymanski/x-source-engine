#include "../../include/XST/XSTCVirtualFileSystemLoader.h"
#include "../../include/XST/XSTPlatform.h"
#include "../../include/XST/XSTPath.h"
#include "../../include/XST/XSTCZlib.h"
#include "../../include/XST/XSTCLogger.h"

//ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
//                                   const Bytef *source, uLong sourceLen));

//typedef int(*PFNUNCOMPRESS)(Bytef *, uLongf *, const Bytef *, uLong);
//PFNUNCOMPRESS ZLibUncompress;

namespace XST
{
	static const Resources::CDirectory::FileMap g_mEmptyFileMap;

	CVirtualFileSystemLoader::CVirtualFileSystemLoader(bool bFileBuffer, xst_castring& strArchiveDll)
	{
		Resources::CDirectory* pRootDir = xst_new Resources::CDirectory( xst_null, "Root" );
		m_pRootDir = Resources::DirectoryPtr( pRootDir );
		m_bFileBuffer = bFileBuffer;
		m_ulLargestFileCompressedSize = 0;
		m_uiTestNum = 0;
		m_bIsReady = false;
		m_bDestroyArchive = true;

		//Load library
		/*m_hZLibDll = XST::Platform::LoadLibrary( strZlibName.data() );
		if( m_hZLibDll )
		{
			ZLibUncompress = (PFNUNCOMPRESS)XST::Platform::GetProcAddress( m_hZLibDll, "uncompress" );
		}*/

		m_pDefaultArchive = xst_new CZLib( strArchiveDll.data() );
		m_pArchive = m_pDefaultArchive;
	}

	CVirtualFileSystemLoader::CVirtualFileSystemLoader(bool bFileBuffer, IArchive* pArchive, bool bDestroyArchive)
	{
		Resources::CDirectory* pRootDir = xst_new Resources::CDirectory( xst_null, "Root" );
		m_pRootDir = Resources::DirectoryPtr( pRootDir );
		m_bFileBuffer = bFileBuffer;
		m_ulLargestFileCompressedSize = 0;
		m_uiTestNum = 0;
		m_bIsReady = false;
		m_bDestroyArchive = bDestroyArchive;

		//Load library
		/*m_hZLibDll = XST::Platform::LoadLibrary( strZlibName.data() );
		if( m_hZLibDll )
		{
			ZLibUncompress = (PFNUNCOMPRESS)XST::Platform::GetProcAddress( m_hZLibDll, "uncompress" );
		}*/

		if( pArchive )
		{
			m_pDefaultArchive = pArchive;
			m_pArchive = m_pDefaultArchive;
		}
	}

	CVirtualFileSystemLoader::~CVirtualFileSystemLoader()
	{
		/*if( m_hZLibDll )
		{
			XST::Platform::CloseLibrary( m_hZLibDll );
		}*/

		for(u32 i = 0; i < m_vListeners.size(); ++i)
		{
			xst_delete( m_vListeners[ i ] );
		}
		m_vListeners.clear();

		Close();
		/*for(u32 i = 0; i < m_vThreads.size(); ++i)
		{
			xst_delete( m_vThreads[i] );
		}
		m_vThreads.clear();*/

		if( m_bDestroyArchive )
		{
			xst_delete( m_pDefaultArchive );
		}
		m_pArchive = xst_null;
		m_bIsReady = false;
	}

	i32 CVirtualFileSystemLoader::Init(xst_castring& strDefaultDir)
	{
		return Open( strDefaultDir );
	}

	IFileLoader* CVirtualFileSystemLoader::CreateInstance()
	{
		IFileLoader* pLoader = xst_new CVirtualFileSystemLoader( m_bFileBuffer, m_pDefaultArchive, false );
		return pLoader;
	}

	i32 CVirtualFileSystemLoader::Open(xst_castring& strArchiveName)
	{
		/*if( !m_hZLibDll )
		{
			XST::CLastError::Set( "ZLib dll not loaded" );
			return RESULT::FAILED;
		}*/
		if( !m_pArchive->IsOpened() )
		{
			//XST::CLastError::Set( "Archive is not opened" );
			XST_LOG_ERR( "Archive: " << strArchiveName << " is not opened" );
			return RESULT::FAILED;
		}

		//Open file
		if( m_Reader.Open(strArchiveName.data()) != RESULT::OK)
		{
			return RESULT::FAILED;
		}

		ch8 strHeader[4];
		m_Reader.ReadChars(strHeader, 3);

		if( strcmp( strHeader, "VFS" ) != 0 )
		{
			XST::CLastError::Set("Wrong file header");
			return RESULT::FAILED;
		}

		i32 iVersion = m_Reader.ReadInt32();
		i32 iPackLevel = m_Reader.ReadInt32();
		ul32 ulOffset = 3 + sizeof(i32) * 2;
		ul32 ulFileSize = m_Reader.GetSize();
		while( ulOffset < ulFileSize )
		{
			ReadBlock( m_Reader, &ulOffset );
			m_Reader.SetPosition( ulOffset, m_Reader.BEGIN );
		}

		//If file buffer get largetst file
		if( m_bFileBuffer )
		{
			GetAllFiles();
			m_Buffer.Create( m_ulLargestFileCompressedSize + 1 );
		}

		//m_pDefaultThread = xst_new CVFSLoadingThread( this );
		//m_pDefaultThread->Create();

		m_bIsReady = true;
		return RESULT::OK;
	}

	void CVirtualFileSystemLoader::AppendFiles(Resources::DirectoryPtr pDir, ul32* pulLargetSize)
	{
		xst_assert( pulLargetSize != 0, "(CVirtualFileSystemLoader::AppendFiles) largest size cannot be null" );

		const Resources::CDirectory::FileMap& mFiles = pDir->GetFiles();
		Resources::CDirectory::FileMap::const_iterator Itr = mFiles.begin();
		for(; Itr != mFiles.end(); ++Itr)
		{
			if( (*pulLargetSize) < Itr->second.GetPointer()->GetCompressedSize() )
			{
				(*pulLargetSize) = Itr->second.GetPointer()->GetCompressedSize();
			}
			m_vAllFiles.push_back( Itr->second );
		}

		const Resources::CDirectory::DirMap& mDirs = pDir->GetDirectories();
		Resources::CDirectory::DirMap::const_iterator Itr2 = mDirs.begin();
		for(; Itr2 != mDirs.end(); ++Itr2)
		{
			AppendFiles( Itr2->second, pulLargetSize );
		}
	}

	const CVirtualFileSystemLoader::FileVector& CVirtualFileSystemLoader::GetAllFiles()
	{
		if( m_vAllFiles.size() > 0 )
			return m_vAllFiles;

		AppendFiles( m_pRootDir, &m_ulLargestFileCompressedSize );
		return m_vAllFiles;
	}

	i32 CVirtualFileSystemLoader::ReadBlock(XST::CBinaryReader& Reader, ul32* pulOffset)
	{
		//Read header size
		i32 iHeaderSize = Reader.ReadInt32();
		//Block size
		i32 iBlockSize = Reader.ReadInt32();
		//Dir count
		i32 iDirCount = Reader.ReadInt32();
		//File count
		i32 iFileCount = Reader.ReadInt32();

		for(i32 i = 0; i < iFileCount; ++i)
		{
			ReadFileInfo( m_pRootDir.GetPointer(), Reader );
		}

		for(i32 i = 0; i < iDirCount; ++i)
		{
			ReadDirInfo( m_pRootDir.GetPointer(), Reader );
		}

		(*pulOffset) += (ul32)( iHeaderSize + iBlockSize );

		return RESULT::OK;
	}

	i32 CVirtualFileSystemLoader::ReadFileInfo(Resources::CDirectory* pParentDir, XST::CBinaryReader& Reader)
	{
		//Read name
		xst_astring strName = Reader.ReadString();
		if( strName.length() == 0 )
		{
			return RESULT::FAILED;
		}

		u32 uiSize = Reader.ReadUInt32();
		u32 uiCompSize = Reader.ReadUInt32();
		u32 uiOffset = Reader.ReadUInt32();

		//Resources::CFile* pFile = xst_new Resources::CFile( pParentDir, strName );
		Resources::FilePtr pFile( xst_new Resources::CFile( pParentDir, strName, xst_null, 0, 0, 0 ) );
		pFile->m_ulSize = uiSize;
		pFile->m_ulCompSize = uiCompSize;
		pFile->m_ulOffset = uiOffset;

		pParentDir->AddFile( pFile );

		return RESULT::OK;
	}

	i32	CVirtualFileSystemLoader::ReadDirInfo(Resources::CDirectory* pParentDir, XST::CBinaryReader& Reader)
	{
		//Read name
		xst_astring strName = Reader.ReadString();
		if( strName.length() == 0 )
		{
			return RESULT::FAILED;
		}

		u32 uiDirCount = Reader.ReadUInt32();
		u32 uiFileCount = Reader.ReadUInt32();

		//Resources::CDirectory* pDir = xst_new Resources::CDirectory( pParentDir, strName );
		Resources::DirectoryPtr pDir( xst_new Resources::CDirectory( pParentDir, strName ) );
		pParentDir->AddDirectory( pDir );

		for(u32 i = 0; i < uiFileCount; ++i)
		{
			ReadFileInfo( pDir.GetPointer(), Reader );
		}

		for(u32 i = 0; i < uiDirCount; ++i)
		{
			ReadDirInfo( pDir.GetPointer(), Reader );
		}

		return RESULT::OK;
	}

	Resources::FilePtr	CVirtualFileSystemLoader::LoadFile(xst_castring& strDirPath, xst_castring& strFileName)
	{
		Resources::DirectoryPtr pDir = FindDirectory( strDirPath );
		if( pDir.IsNull() )
		{
			XST::CLastError::Set( "Directory not exists" );
			return Resources::FilePtr( xst_null );
		}

		/*if( bBackground )
		{
			return LoadFileInBackground( pDir, strFileName );
		}
		else*/
		{
			return LoadFileInMainThread( pDir, strFileName );
		}
	}


	Resources::FilePtr	CVirtualFileSystemLoader::LoadFileInBackground(Resources::DirectoryPtr pDir, xst_castring& strFileName)
	{
		Resources::FilePtr pNull( xst_null );

		/*if( m_vThreads.size() == 0 )
		{
			this->CreateLoadingThread();
		}*/

		//m_pDefaultThread->m_pDefaultDir = pDir;
		//m_pDefaultThread->m_strFileNameToLoad = strFileName;
		//m_pDefaultThread->Start();

		return pNull;
	}

	Resources::FilePtr	CVirtualFileSystemLoader::LoadFileInMainThread(Resources::DirectoryPtr pDir, xst_castring& strFileName)
	{
		Resources::FilePtr pFileOut;

		if( !m_bFileBuffer )
		{
			pFileOut = LoadFile( pDir, strFileName, 0, 0 );
		}
		else
		{
			pFileOut = LoadFile( pDir, strFileName, m_Buffer.GetData(), m_Buffer.GetSize() );
		}

		//for(int i = 0; i < 10; ++i) std::cout<< "load main" << std::endl;
		m_uiTestNum++;

		return pFileOut;
	}

	Resources::FilePtr CVirtualFileSystemLoader::LoadFile(Resources::DirectoryPtr pDir, xst_castring& strFileName, u8* pCompBuff, cul32& ulBuffSize)
	{
		m_CritSection.Enter();

		//m_Timer.StartQPerf();
		
		Resources::FilePtr pNull( xst_null );

		if( !m_Reader.IsOpened() )
		{
			return pNull;
		}

		//Find directory
		//Resources::DirectoryPtr pDir = FindDirectory( strDirPath );
		if( pDir.IsNull() )
		{
			return pNull;
		}

		Resources::FilePtr pFile = pDir->FindFile( strFileName );
		if( pFile.IsNull() )
		{
			return pNull;
		}

		//Go to offset
		m_Reader.SetPosition( pFile->m_ulOffset, XST::IFile::BEGIN );
		ul32 ulPos = m_Reader.GetPosition();
		ul32 ulSize = m_Reader.GetSize();
		if( m_Reader.IsEOF() )
		{
			return pNull;
		}

		XST::TCData<u8> Buff;
		u8* pData = pCompBuff;
		if( pData == xst_null )
		{
			Buff.Create( pFile->m_ulCompSize + 1 );
			pData = Buff.GetData();
		}

		m_Reader.ReadBytes( pData, pFile->m_ulCompSize );
		if( m_Reader.IsReadError() )
		{
			return pNull;
		}

		//u8* aData = xst_new u8[ pFile->m_ulDataSize ];
		Resources::FilePtr pFileOut( xst_new Resources::CFile( pFile->m_pParentDir, pFile->m_strName, xst_null, 0, 0, 0 ) );
		//pFileOut->m_aData = aData;
		pFileOut->m_Data.Create( pFile->m_ulSize );
		u8* aData = pFileOut->GetData().GetData();
		pFileOut->m_ulSize = pFile->m_ulSize;

		//i32 iResult = ZLibUncompress( aData, (uLongf*)&pFile->m_ulSize, pData, pFile->m_ulCompSize );

		if( m_pArchive->Uncompress( aData, pFile->m_ulSize, pData, pFile->m_ulCompSize ) != RESULT::OK )
		{
			return pNull;
		}

		/*if( iResult != Z_OK )
		{
			switch( iResult )
			{
				case Z_MEM_ERROR: XST::CLastError::Set( "Not enough memory to uncompress data" ); break;
				case Z_BUF_ERROR: XST::CLastError::Set( "Too small output buffer" ); break;
				case Z_DATA_ERROR: XST::CLastError::Set( "Corrupted or incomplete input data" ); break;
			}
			return pNull;
		}*/	
		
		//pFileOut->m_fLoadingTime = m_Timer.GetQPerfElapsedTime();

		m_CritSection.Leave();
		return pFileOut;
	}

	Resources::DirectoryPtr CVirtualFileSystemLoader::FindDirectory(xst_castring& strFullPath)
	{
		XST::StringUtil::TokensA vTokens = XST::StringUtil::Split(strFullPath, "\\");
		if( vTokens.size() == 0 )
		{
			return m_pRootDir;
		}

		Resources::DirectoryPtr pDir = m_pRootDir;
		for(u32 i = 0; i < vTokens.size(); ++i)
		{
			pDir = FindDirectory( pDir, vTokens[ i ] );
		}

		if( pDir.IsNull() ) pDir = m_pRootDir;

		if( pDir->GetName() == vTokens.back() )
		{
			return pDir;
		}

		return Resources::DirectoryPtr( xst_null );
	}

	Resources::DirectoryPtr CVirtualFileSystemLoader::FindDirectory(Resources::DirectoryPtr CurrDir, XST::Types::xst_castring &strDirName)
	{
		if( CurrDir.IsNull() )
			return CurrDir;

		return CurrDir->FindDirectory( strDirName );
	}

	void CVirtualFileSystemLoader::Close()
	{
		//m_pDefaultThread->Kill();
		//xst_delete( m_pDefaultThread );
		m_Reader.Close();
	}

	Resources::FilePtr CVirtualFileSystemLoader::Load(xst_castring& strDirName, xst_castring& strFileName)
	{
		return this->LoadFile( strDirName, strFileName );
	}

	Resources::FilePtr CVirtualFileSystemLoader::Load(xst_castring& strFullPath)
	{
		xst_astring strDirName, strFileName;
		XST::Path::GetParentDirPathAndFileName( strFullPath, &strDirName, &strFileName );
		return this->LoadFile( strDirName, strFileName );
	}

	Resources::FilePtr CVirtualFileSystemLoader::GetFileInfo(xst_castring& strDirName, xst_castring& strFileName)
	{
		Resources::DirectoryPtr pDir = this->FindDirectory( strDirName );
		if( pDir.IsNull() )
		{
			return Resources::FilePtr( xst_null );
		}

		return pDir->FindFile( strFileName );
	}

	Resources::FilePtr CVirtualFileSystemLoader::GetFileInfo(xst_castring& strFullPath)
	{
		xst_astring strDirName, strFileName;
		XST::Path::GetParentDirPathAndFileName( strFullPath, &strDirName, &strFileName );
		return this->GetFileInfo( strDirName, strFileName );
	}

	void GetFiles(Resources::DirectoryPtr pCurrDir, Resources::CDirectory::FileMap* pmFiles)
	{
		Resources::CDirectory::FileMap::const_iterator Itr = pCurrDir->GetFiles().begin();
		for(; Itr != pCurrDir->GetFiles().end(); ++Itr)
		{
			pmFiles->insert( Resources::CDirectory::FileMap::value_type( Itr->first, Itr->second ) );
		}

		Resources::CDirectory::DirMap::const_iterator DirItr = pCurrDir->GetDirectories().begin();
		for(; DirItr != pCurrDir->GetDirectories().end(); ++DirItr)
		{
			GetFiles( DirItr->second, pmFiles );
		}
	}

	Resources::CDirectory::FileMap	CVirtualFileSystemLoader::GetFileInfos(xst_castring& strDirName, bool bRecursive)
	{
		Resources::DirectoryPtr pDir = this->FindDirectory( strDirName );
		if( pDir.IsNull() )
		{
			return g_mEmptyFileMap;
		}
		
		if( bRecursive )
		{
			Resources::CDirectory::FileMap mFiles;
			GetFiles( pDir, &mFiles );
			return mFiles;
		}
		else
		{
			return pDir->GetFiles( );
		}
	}

}//xst