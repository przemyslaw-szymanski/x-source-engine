#include "../../include/XST/XSTCFileSystemLoader.h"
#include "../../include/XST/XSTPath.h"
#include "../../include/XST/XSTIFile.h"
#include "../../include/XST/XSTCLogger.h"
#include "../../include/XST/XSTPath.h"

namespace XST
{
	
	CFileSystemLoader::CFileSystemLoader()
	{
	}
	
	CFileSystemLoader::~CFileSystemLoader()
	{
	}

	i32 CFileSystemLoader::Init(xst_castring& strDefaultDir)
	{
		m_pRoot = Resources::DirectoryPtr( xst_new Resources::CDirectory( xst_null, strDefaultDir ) ); 
		m_pRoot->m_strPath = strDefaultDir;
		return RESULT::OK; 
	}

	Resources::FilePtr	CFileSystemLoader::Load(xst_castring& strDirName, xst_castring& strFileName)
	{
		return Load( strDirName + xst_astring( "\\" ) + strFileName );
	}

	Resources::FilePtr	CFileSystemLoader::Load(xst_castring& strFullPath)
	{
		if( m_Reader.Open( strFullPath.data() ) != RESULT::OK )
		{
			XST_LOG_ERR( "File: " << strFullPath << " does not exists" );
			return Resources::FilePtr( xst_null );
		}

		u8* pData = xst_new u8[ m_Reader.GetSize() + 1 ];
		Resources::FilePtr pFile( xst_new Resources::CFile( XST::Path::GetFileName( strFullPath ), strFullPath, pData, m_Reader.GetSize() ) );
		//pFile->m_Data.Create( m_Reader->GetSize() );
		//this->CreateFileBuffer( pFile, m_Reader.GetSize(), true );
		//u8* pData = this->GetFileBuffer( pFile );

		//-1 size has to be set because during creation of the buffer it is set size +1 
		//so the buffer size is bigger than should be
		//Reader compares bytes read with buffer size and if it is not equals reader returns fail
		//It is important to create biggest buffer because it should be set null at the end 
		if( this->m_Reader.ReadAll( pData, pFile->GetSize() ) != RESULT::OK )
		{
			XST_LOG_ERR( "Failed to load file: " << strFullPath );
			//this->DeleteFileBuffer( pFile );
			m_Reader.Close();
			return Resources::FilePtr( xst_null );
		}
		
		pData[ m_Reader.GetSize() ] = '\0';
	
		m_Reader.Close();
		return pFile;
	}

	Resources::FilePtr	CFileSystemLoader::GetFileInfo(xst_castring& strDirName, xst_castring& strFileName)
	{
		return GetFileInfo( strDirName + xst_astring( "\\" ) + strFileName );
	}

	Resources::FilePtr	CFileSystemLoader::GetFileInfo(xst_castring& strFullPath)
	{
		l32 lSize = XST::IFile::GetSize( strFullPath.data() );
		if( lSize < 0 )
		{
			XST_LOG_ERR( "File: " << strFullPath << " does not exists" );
			return Resources::FilePtr( xst_null );
		}

		Resources::FilePtr pFile( xst_new Resources::CFile( XST::Path::GetFileName( strFullPath ), strFullPath, 0, lSize, lSize ) );
		//this->SetFileSize( pFile, lSize );
		//this->SetFileCompressedSize( pFile, lSize );
		return pFile;
	}

	#if defined (XST_WINDOWS)
	void GetFileInfosRecursived(xst_castring& strDir, Resources::CDirectory::FileMap* pmFiles)
	{
		WIN32_FIND_DATAA FindFileData;
		xst_astring strPath = XST::Path::Combine( strDir, "*" );
		HANDLE hFile = ::FindFirstFileA( strPath.data(), &FindFileData );
		

		if( hFile != INVALID_HANDLE_VALUE )
		{
			if( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				Resources::FilePtr pFile( xst_new Resources::CFile( strDir, Path::GetFileName( FindFileData.cFileName ) ) );
				pmFiles->insert( Resources::CDirectory::FileMap::value_type( pFile->GetName(), pFile ) );
			}
			else if( FindFileData.cFileName[0] != '.' && FindFileData.cFileName[1] != '.' )
			{
				//Resources::DirectoryPtr pDir( xst_new Resources::CDirectory( pCurrDir, FindFileData.cFileName ) );
				GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ).data(), pmFiles );
			}

			while( ::FindNextFileA( hFile, &FindFileData ) )
			{
				if( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
				{
					Resources::FilePtr pFile( xst_new Resources::CFile( Path::GetFileName( FindFileData.cFileName ), strDir ) );
					pmFiles->insert( Resources::CDirectory::FileMap::value_type( pFile->GetName(), pFile ) );
				}
				else if( FindFileData.cFileName[0] != '.' && FindFileData.cFileName[1] != '.' )
				{
					//Resources::DirectoryPtr pDir( xst_new Resources::CDirectory( pCurrDir, FindFileData.cFileName ) );
					GetFileInfosRecursived( XST::Path::Combine( strDir, FindFileData.cFileName ).data(), pmFiles );
				}
			}
		}

		FindClose( hFile );
	}

	//void FindFilesRecursively(lpcastr lpFolder, Resources::CDirectory::FileMap* pmFiles, Resources::DirectoryPtr pCurrDir, lpcastr lpFilePattern)
	//{
	//	ch8 szFullPattern[MAX_PATH];
	//	WIN32_FIND_DATAA FindFileData;
	//	HANDLE hFindFile;
	//	// first we are going to process any subdirectories
	//	//PathCombine(szFullPattern, lpFolder, "*");
	//	xst_astring strFullPattern = XST::Path::Combine( lpFolder, "*" );
	//	//hFindFile = FindFirstFileA( szFullPattern, &FindFileData);
	//	hFindFile = FindFirstFileA( strFullPattern.data(), &FindFileData );

	//	if(hFindFile != INVALID_HANDLE_VALUE)
	//	{
	//		do
	//		{
	//			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && strcmp( FindFileData.cFileName, "." ) != 0 )
	//			{
	//				// found a subdirectory; recurse into it
	//				//PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
	//				strFullPattern = XST::Path::Combine( lpFolder, FindFileData.cFileName );
	//				Resources::DirectoryPtr pDir( xst_new Resources::CDirectory( pCurrDir, FindFileData.cFileName ) );
	//				FindFilesRecursively( strFullPattern.data(), pmFiles, pDir, lpFilePattern);
	//			}
	//		} while(FindNextFileA(hFindFile, &FindFileData));
	//		FindClose(hFindFile);
	//	}
	//	// now we are going to look for the matching files
	//	//PathCombine(szFullPattern, lpFolder, lpFilePattern);
	//	strFullPattern = XST::Path::Combine( lpFolder, lpFilePattern );
	//	hFindFile = FindFirstFileA( strFullPattern.data(), &FindFileData);
	//	if(hFindFile != INVALID_HANDLE_VALUE)
	//	{
	//		do
	//		{
	//			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//			{
	//				// found a file; do something with it
	//				//PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
	//				Resources::FilePtr pFile( xst_new Resources::CFile( pCurrDir.GetPointer(), Path::GetFileName( FindFileData.cFileName ) ) );
	//				pmFiles->insert( Resources::CDirectory::FileMap::value_type( pFile->GetName(), pFile ) );
	//			}
	//		} while(FindNextFileA(hFindFile, &FindFileData));
	//		FindClose(hFindFile);
	//	}
	//}

	#else
	#error "XSTCSystemFileLoader.cpp GetFileInfosRecursived not implemented"
	#endif

	Resources::CDirectory::FileMap CFileSystemLoader::GetFileInfos(xst_castring& strDirName, bool bRecursive)
	{
		m_mFiles.clear();
		#if defined (XST_WINDOWS)
			
			if( !bRecursive )
			{
				xst_astring strDir = strDirName + "\\*";
				::WIN32_FIND_DATAA	FindFileData;
				HANDLE hFile = ::FindFirstFileA( strDir.data(), &FindFileData );

				if( hFile != INVALID_HANDLE_VALUE )
				{
					if( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
					{
						Resources::FilePtr pFile( xst_new Resources::CFile( Path::GetFileName( FindFileData.cFileName ), FindFileData.cFileName ) );
						m_mFiles.insert( Resources::CDirectory::FileMap::value_type( pFile->GetName(), pFile ) );
					}

					while( ::FindNextFileA( hFile, &FindFileData ) != 0 )
					{
						if( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
						{
							Resources::FilePtr pFile( xst_new Resources::CFile( m_pRoot.GetPointer(), Path::GetFileName( FindFileData.cFileName ), xst_null, 0, 0, 0 ) );
							m_mFiles.insert( Resources::CDirectory::FileMap::value_type( pFile->GetName(), pFile ) );
						}
					}
				}
			}
			else
			{
				GetFileInfosRecursived( strDirName.data(), &m_mFiles );
				//FindFilesRecursively( strDirName.data(), &m_mFiles, m_pRoot, "*.*" );
			}

		#else

		#endif

		return m_mFiles;
	}

}//xst