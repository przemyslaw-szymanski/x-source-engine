#include "../../include/XST/XSTCCfgFile.h"
#include "../../include/XST/XSTCLogger.h"

namespace XST
{

	i32	CCfgFile::CSection::AddValue(xst_castring& strKey, xst_castring& strValue)
	{
		m_mAVals[ strKey ] = strValue;
		return RESULT::OK;
	}

	i32	CCfgFile::CSection::AddValue(xst_cwstring& strKey, xst_cwstring& strValue)
	{
		m_mUVals[ strKey ] = strValue;
		return RESULT::OK;
	}

	xst_castring&	CCfgFile::CSection::GetValue(xst_castring& strKey, xst_castring& strDefault)
	{
		CCfgFile::AValueMap::iterator Itr = m_mAVals.find( strKey );
		if( Itr != m_mAVals.end() )
		{
			return Itr->second;
		}
		return strDefault;
	}

	xst_cwstring&	CCfgFile::CSection::GetValue(xst_cwstring& strKey, xst_cwstring& strDefault)
	{
		CCfgFile::UValueMap::iterator Itr = m_mUVals.find( strKey );
		if( Itr != m_mUVals.end() )
		{
			return Itr->second;
		}
		return strDefault;
	}

	bool CCfgFile::CSection::IsEmpty() const
	{
		return m_mAVals.size() == 0 || m_mUVals.size() == 0;
	}

	CCfgFile::AValueIterator CCfgFile::CSection::GetAsciiIterator() const
	{
		CCfgFile::AValueIterator Itr;
		Itr.m_pmValues = (xst_map< xst_astring, xst_astring >*)&this->m_mAVals;
		Itr.m_Itr = this->m_mAVals.begin();
		return Itr;
	}

	CCfgFile::UValueIterator CCfgFile::CSection::GetUnicodeIterator() const
	{
		CCfgFile::UValueIterator Itr;
		Itr.m_pmValues = (xst_map< xst_wstring, xst_wstring >*)&this->m_mUVals;
		Itr.m_Itr = this->m_mUVals.begin();
		return Itr;
	}

	CCfgFile::CCfgFile() : m_bLoaded( false ), m_pFileMgr( xst_null )
	{
	}

	CCfgFile::CCfgFile(xst_castring& strFileName) : m_bLoaded ( false ), m_pFileMgr( xst_null ), m_strFileName( strFileName )
	{
		if( m_Reader.Open( strFileName ) != RESULT::OK )
		{
			XST_LOG_ERR( "Could not open file: " << strFileName );
			m_Reader.Close();
		}

		if( Load( strFileName ) != RESULT::OK )
		{

		}
	}

	CCfgFile::CCfgFile(xst_castring& strFileName, xst_castring& strGroupName) : m_bLoaded ( false ), m_strFileName( strFileName )
	{
		Load( strFileName, strGroupName );
	}

	CCfgFile::~CCfgFile()
	{
		ASectionMap::iterator AItr;
		for(AItr = m_mASections.begin(); AItr != m_mASections.end(); ++AItr)
		{
			xst_delete( AItr->second );
		}

		m_mASections.clear();

		USectionMap::iterator UItr;
		for(UItr = m_mUSections.begin(); UItr != m_mUSections.end(); ++UItr)
		{
			xst_delete( UItr->second );
		}

		m_mUSections.clear();

		m_pFileMgr = xst_null;
		//m_Reader.Close();
	}

	bool CCfgFile::IsAsciiFile() const
	{
		return m_eFileEncoding == ITextFile::ANSI;
	}

	bool CCfgFile::IsUnicodeFile() const
	{
		return m_eFileEncoding == ITextFile::UTF16BE || m_eFileEncoding == ITextFile::UTF16LE;
	}

	i32 CCfgFile::LoadFromMemory(cu8 *pFileData, cul32 &ulDataSize)
	{
		m_eFileEncoding = ITextFile::GetEncoding( pFileData, ulDataSize );

		if( m_eFileEncoding == XST::CTextFileReader::ANSI )
		{
			return _LoadAscii( (cch8*)pFileData, ulDataSize );
		}
		else if( m_eFileEncoding == XST::CTextFileReader::UTF16LE || m_eFileEncoding == XST::CTextFileReader::UTF16BE )
		{
			cu8* pPtr = pFileData;
			pPtr += 2; //BOM
			return _LoadUnicode( (cch16*)pPtr, ulDataSize - 2 );
		}

		return RESULT::OK;
	}

	i32 CCfgFile::LoadFromMemory(cu8 *pFileData, cul32 &ulDataSize, const ITextFile::FILE_ENCODING& eEncoding)
	{
		m_eFileEncoding = eEncoding;

		if( m_eFileEncoding == XST::CTextFileReader::ANSI )
		{
			return _LoadAscii( (cch8*)pFileData, ulDataSize );
		}
		else if( m_eFileEncoding == XST::CTextFileReader::UTF16LE || m_eFileEncoding == XST::CTextFileReader::UTF16BE )
		{
			cu8* pPtr = pFileData;
			pPtr += 2; //BOM
			return _LoadUnicode( (cch16*)pPtr, ulDataSize - 2 );
		}

		return RESULT::OK;
	}

	i32	CCfgFile::Load(xst_castring& strFileName)
	{
		if( !m_Reader.IsOpened() )
		{
			if( m_Reader.Open( strFileName ) != RESULT::OK )
			{
				XST_LOG_ERR( "Could not open file: " << strFileName );
				return RESULT::FAILED;
			}
		}

		m_strFileName = strFileName;

		m_Reader.ReadAll();
		m_eFileEncoding = m_Reader.GetEncoding();
		//Get encoding
		if( m_Reader.GetEncoding() == XST::CTextFileReader::ANSI )
		{
			return _LoadAscii( m_Reader.GetAsciiData(), m_Reader.GetSize() );
		}
		else if( m_Reader.GetEncoding() == XST::CTextFileReader::UTF16LE || m_Reader.GetEncoding() == XST::CTextFileReader::UTF16BE )
		{
			return _LoadUnicode( m_Reader.GetUnicodeData(), m_Reader.GetSize() );
		}

		return RESULT::FAILED;
	}

	i32	CCfgFile::Load(xst_castring& strFileName, xst_castring& strGroupName)
	{
		m_pFileMgr = CFileManager::GetSingletonPtr();
		m_strFileName = strFileName;

		Resources::FilePtr pFile = m_pFileMgr->LoadFile( strFileName, strGroupName );
		if( !pFile.IsNull() )
		{
			m_eFileEncoding = ITextFile::GetEncoding( pFile->GetData().GetData(), pFile->GetSize() );
			if( ITextFile::IsUTF16( m_eFileEncoding ) )
			{
				ch16* pData = (ch16*)pFile->GetData().GetData();
				pData[ pFile->GetSize() / sizeof( u16 ) ] = 0;
			}

			return LoadFromMemory( pFile->GetData().GetData(), pFile->GetData().GetSize(), m_eFileEncoding );
		}

		return RESULT::FAILED;
	}

	i32 CCfgFile::_LoadAscii(cch8* pFileData, cul32& ulDataSize)
	{
		//Tokenize file
		XST::StringUtil::TokensA vTokens;
		XST::StringUtil::SplitExcept( pFileData, &vTokens, "\"", "\"", "\t\r\n= " );
		//Get default section
		CSection* pCurrSection = xst_null; //_GetOrCreateSection( "" );

		for(u32 i = 0; i < vTokens.size(); ++i)
		{
			xst_astring& strCurrToken = vTokens[ i ];

			//Skip comments
			if( strCurrToken[0] == '#' || (  strCurrToken.length() > 1 && strCurrToken[0] == '/' && strCurrToken[1] == '/' ) ) continue;
			//If section name
			if( strCurrToken[0] == '[' && strCurrToken[ strCurrToken.length() - 1 ] == ']' )
			{
				//Create new section or add to the existing
				pCurrSection = _GetOrCreateSection( strCurrToken.substr( 1, strCurrToken.length() - 2 ) );
			}
			else
			{
				//If key = value exists
				if( i + 1 < vTokens.size() )
				{

					if( pCurrSection == xst_null )
					{
						pCurrSection = _GetOrCreateSection( "" );
					}

					xst_astring& strNextToken = vTokens[ i + 1 ];
					ch16 c = strNextToken[ strNextToken.length() - 1 ];
					if( strNextToken[0] == '"' && strNextToken[ strNextToken.length() - 1 ] == '"' )
					{
						strNextToken = strNextToken.substr( 1, strNextToken.length() - 2 );
					}
					pCurrSection->AddValue( strCurrToken, strNextToken );

					++i;
				}
			}

		}

		m_bLoaded = true;
		return RESULT::OK;
	}

	i32 CCfgFile::_LoadUnicode(cch16* pFileData, cul32& ulDataSize)
	{
		//TCData< ch16 > Data;
		//Set ulDataSize / sizeof( ch16 ) because it must be size of the wide char buffer
		//ulDataSize is the size (in bytes) of all file data except BOM
		//Data.Create( (const xst_unknown)pFileData, ulDataSize, ulDataSize / sizeof( ch16 ), true );

		//Tokenize file
		XST::StringUtil::TokensW vTokens;
		XST::StringUtil::SplitExcept( pFileData, &vTokens, L"\"", L"\"", L"\t\r\n= " );
		//Get default section
		CSection* pCurrSection = xst_null; //_GetOrCreateSection( "" );

		for(u32 i = 0; i < vTokens.size(); ++i)
		{
			xst_wstring& strCurrToken = vTokens[ i ];

			//Skip comments
			if( strCurrToken[0] == L'#' || (  strCurrToken.length() > 1 && strCurrToken[0] == L'/' && strCurrToken[1] == L'/' ) ) continue;
			//If section name
			if( strCurrToken[0] == L'[' && strCurrToken[ strCurrToken.length() - 1 ] == L']' )
			{
				//Create new section or add to the existing
				pCurrSection = _GetOrCreateSection( strCurrToken.substr( 1, strCurrToken.length() - 2 ) );
			}
			else
			{
				//If key = value exists
				if( i + 1 < vTokens.size() )
				{

					if( pCurrSection == xst_null )
					{
						pCurrSection = _GetOrCreateSection( L"" );
					}

					xst_wstring& strNextToken = vTokens[ i + 1 ];
					ch16 c = strNextToken[ strNextToken.length() - 1 ];
					if( strNextToken[0] == L'"' && strNextToken[ strNextToken.length() - 1 ] == L'"' )
					{
						strNextToken = strNextToken.substr( 1, strNextToken.length() - 2 );
					}
					pCurrSection->AddValue( strCurrToken, strNextToken );

					++i;
				}
			}

		}

		m_bLoaded = true;
		return RESULT::OK;
	}

	CCfgFile::CSection* CCfgFile::_GetOrCreateSection(xst_castring& strSectionName)
	{
		ASectionMap::iterator Itr = m_mASections.find( strSectionName );
		if( Itr == m_mASections.end() )
		{
			CSection* pSection = xst_new CSection();
			m_mASections.insert( ASectionMap::value_type( strSectionName, pSection ) );
			return pSection;
		}

		return Itr->second;
	}

	CCfgFile::CSection* CCfgFile::_GetOrCreateSection(xst_cwstring& strSectionName)
	{
		USectionMap::iterator Itr = m_mUSections.find( strSectionName );
		if( Itr == m_mUSections.end() )
		{
			CSection* pSection = xst_new CSection();
			m_mUSections.insert( USectionMap::value_type( strSectionName, pSection ) );
			return pSection;
		}

		return Itr->second;
	}

	CCfgFile::ASectionIterator	CCfgFile::GetAsciiSectionIterator() const
	{
		ASectionIterator Itr;
		Itr.m_Itr = m_mASections.begin();
		Itr.m_pmSections = (xst_map< xst_astring, CSection* >*)&m_mASections;
		return Itr;
	}

	CCfgFile::USectionIterator	CCfgFile::GetUnicodeSectionIterator() const
	{
		USectionIterator Itr;
		Itr.m_Itr = m_mUSections.begin();
		Itr.m_pmSections = (xst_map< xst_wstring, CSection* >*)&m_mUSections;
		return Itr;
	}

	CCfgFile::CSection* CCfgFile::_GetSection(xst_castring& strName)
	{
		ASectionMap::iterator Itr = m_mASections.find( strName );
		if( Itr != m_mASections.end() )
		{
			return Itr->second;
		}
		return xst_null;
	}

	CCfgFile::CSection* CCfgFile::_GetSection(xst_cwstring& strName)
	{
		USectionMap::iterator Itr = m_mUSections.find( strName );
		if( Itr != m_mUSections.end() )
		{
			return Itr->second;
		}
		return xst_null;
	}

	xst_castring& CCfgFile::GetValue(xst_castring& strKey, xst_castring& strSection, xst_castring& strDefault)
	{
		CSection* pSec = _GetSection( strSection );
		if( pSec == xst_null )
		{
			return strDefault;
		}

		return pSec->GetValue( strKey, strDefault );
	}

	xst_cwstring& CCfgFile::GetValue(xst_cwstring& strKey, xst_cwstring& strSection, xst_cwstring& strDefault)
	{
		CSection* pSec = _GetSection( strSection );
		if( pSec == xst_null )
		{
			return strDefault;
		}

		return pSec->GetValue( strKey, strDefault );
	}

	const CCfgFile::CSection& CCfgFile::GetSection(xst_castring& strSectionName)
	{
		CSection* pSec = _GetSection( strSectionName );
		if( pSec != xst_null )
		{
			return *pSec;
		}

		return m_BlankSection;
	}

	const CCfgFile::CSection& CCfgFile::GetSection(xst_cwstring& strSectionName)
	{
		CSection* pSec = _GetSection( strSectionName );
		if( pSec != xst_null )
		{
			return *pSec;
		}

		return m_BlankSection;
	}

}//xst
