#include "XSTIFileLoader.h"

namespace XST
{
	namespace Resources
	{
		CFile::CFile(xst_castring& strFileName, xst_castring& strPath, u8** ppData, cul32& ulDataSize, cul32& ulCompressedSize, cul32& ulOffset) : 
			m_pParentDir( xst_null ), m_strName( strFileName ), m_strPath( strPath ), m_ulSize( ulDataSize ), m_ulCompSize( ulCompressedSize ), m_ulOffset( ulOffset )
		{
			if( ppData && *ppData )
				m_Data.Move( ppData, ulDataSize, true );
		}

		CFile::CFile(CDirectory* pParentDir, xst_castring& strFileName, u8** ppData, cul32& ulDataSize, cul32& ulCompressedSize, cul32& ulOffset) : 
			m_pParentDir( pParentDir ), m_strName( strFileName ), m_ulSize( ulDataSize ), m_ulCompSize( ulCompressedSize ), m_ulOffset( ulOffset )
		{
			if( ppData && *ppData )
				m_Data.Move( ppData, ulDataSize, true );
			if( pParentDir != xst_null )
			{
				m_strPath = pParentDir->GetPath();
			}
		}

		CFile::~CFile()
		{
			m_ulCompSize = m_ulOffset = 0;
		}

		xst_astring CFile::GetExtension() const
		{
			u32 uPos = m_strName.find_last_of( "." );
			if( uPos == xst_astring::npos )
				return XST::StringUtil::EmptyAString;
			return m_strName.substr( uPos + 1, m_strName.length() - uPos );
		}

		CDirectory::CDirectory(CDirectory* pParentDir, xst_castring& strName, xst_castring& strPath) : m_pParentDir( pParentDir ), m_strName( strName )
		{
			if( m_pParentDir && strPath.length() == 0 )
			{
				if( m_pParentDir->GetPath().length() == 0 )
				{
					m_strPath = m_strName;
				}
				else
				{
					m_strPath = m_pParentDir->GetPath() + "\\" + m_strName;
				}
			}
			else
			{
			    m_strPath = strPath;
			}

			m_ulDirCount = m_ulFileCount = 0;
		}

		CDirectory::~CDirectory()
		{
		}

		i32 CDirectory::AddDirectory(DirPtr pDir)
		{
			DirMap::iterator Itr = m_mDirs.find( pDir->GetName() );
			if( Itr == m_mDirs.end() )
			{
				m_mDirs.insert( DirMap::value_type( pDir->GetName(), pDir ) );
				return RESULT::OK;
			}

			return RESULT::FAILED;
		}

		i32 CDirectory::AddFile(FilePtr pFile)
		{
			FileMap::iterator Itr = m_mFiles.find( pFile->GetName() );
			if( Itr == m_mFiles.end() )
			{
				m_mFiles.insert( FileMap::value_type( pFile->GetName(), pFile ) );
				return RESULT::OK;
			}

			return RESULT::FAILED;
		}

		CDirectory::DirPtr CDirectory::FindDirectory(xst_castring &strName)
		{
			DirMap::iterator Itr = m_mDirs.find( strName );
			if( Itr == m_mDirs.end() )
			{
				return DirPtr( xst_null );
			}

			return Itr->second;
		}

		CDirectory::FilePtr CDirectory::FindFile(xst_castring &strName)
		{
			FileMap::iterator Itr = m_mFiles.find( strName );
			if( Itr == m_mFiles.end() )
			{
				return FilePtr( xst_null );
			}

			return Itr->second;
		}

	}//resources


	IFileLoader::~IFileLoader()
	{
		if( m_bDestroyListeners )
		{
			for(u32 i = 0; i < m_vListeners.size(); ++i)
			{
				xst_delete( m_vListeners[ i ] );
			}
		}
		m_vListeners.clear();
	}

}//xst
