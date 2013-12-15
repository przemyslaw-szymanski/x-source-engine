#ifndef XST_CCFG_FILE_H
#define XST_CCFG_FILE_H

#include "XSTCTextFileReader.h"
#include "XSTCFileManager.h"
#include "XSTCStringUtil.h"

namespace XST
{
	class CCfgFile
	{

		public:

			typedef xst_map< xst_astring, xst_astring >	AValueMap;
			typedef xst_map< xst_wstring, xst_wstring > UValueMap;
			typedef xst_vector< xst_astring >			AValueVector;
			typedef xst_vector< xst_wstring >			UValueVector;

			class CSection;

			template<class _T_>
			class TCValueIterator
			{
				friend class CSection;

				public:

					xst_fi	bool			HasMoreElements()
											{ return m_Itr != m_pmValues->end(); }

					xst_fi	const _T_&		GetFirst()
											{ return m_Itr->first; }

					xst_fi	const _T_&		GetSecond()
											{ return m_Itr->second; }

					xst_fi	void			MoveNext()
											{ ++m_Itr; }

				protected:

					typename xst_map< _T_, _T_ >::const_iterator	m_Itr;
					typename xst_map< _T_, _T_ >*					m_pmValues;
			};

			typedef TCValueIterator< xst_astring >		AValueIterator;
			typedef TCValueIterator< xst_wstring >		UValueIterator;

			class CSection
			{
				public:

					virtual					~CSection() {}

							i32				AddValue(xst_castring& strKey, xst_castring& strValue);

							i32				AddValue(xst_cwstring& strKey, xst_cwstring& strValue);

							xst_castring&	GetValue(xst_castring& strKey, xst_castring& strDefault = StringUtil::EmptyAString);

							xst_cwstring&	GetValue(xst_cwstring& strKey, xst_cwstring& strDefault = StringUtil::EmptyUString);

							CCfgFile::AValueIterator	GetAsciiIterator() const;

							CCfgFile::UValueIterator	GetUnicodeIterator() const;

							bool			IsEmpty() const;

				protected:

					CCfgFile::AValueMap	m_mAVals;
					CCfgFile::UValueMap	m_mUVals;
			};

			typedef xst_map< xst_astring, CSection* > ASectionMap;
			typedef xst_map< xst_wstring, CSection* > USectionMap;

			//typedef ASectionMap::const_iterator	ASectionIterator;
			//typedef USectionMap::const_iterator	USectionIterator;

			template<class _T_>
			class TCSectionIterator
			{
				friend class CCfgFile;

				public:

					xst_fi	bool			HasMoreElements()
											{ return m_Itr != m_pmSections->end(); }

					xst_fi	const _T_&		GetFirst()
											{ return m_Itr->first; }

					xst_fi	const CSection*	GetSecond()
											{ return m_Itr->second; }

					xst_fi	void			MoveNext()
											{ ++m_Itr; }

				protected:

					typename xst_map< _T_, CSection* >::const_iterator m_Itr;
					typename xst_map< _T_, CSection* >*			m_pmSections;
			};			

			typedef TCSectionIterator< xst_astring >	ASectionIterator;
			typedef TCSectionIterator< xst_wstring >	USectionIterator;
			

		public:	

										CCfgFile();
										CCfgFile(xst_castring& strFileName);
										CCfgFile(xst_castring& strFileName, xst_castring& strGroupName);
			virtual						~CCfgFile();

					i32					Load(xst_castring& strFileName);

					i32					Load(xst_castring& strFileName, xst_castring& strGroupName);

					i32					LoadFromMemory(cu8* pFileData, cul32& ulDataSize);

					i32					LoadFromMemory(cu8* pFileData, cul32& ulDataSize, const ITextFile::FILE_ENCODING& eEncoding);

					ASectionIterator	GetAsciiSectionIterator() const;

					USectionIterator	GetUnicodeSectionIterator() const;

					bool				IsAsciiFile() const;

					bool				IsUnicodeFile() const;										

			xst_fi	bool				IsLoaded();

					xst_cwstring&		GetValue(xst_cwstring& strKey, xst_cwstring& strSection, xst_cwstring& strDefault = StringUtil::EmptyUString);

					xst_castring&		GetValue(xst_castring& strKey, xst_castring& strSection, xst_castring& strDefault = StringUtil::EmptyAString);

			const	CSection&			GetSection(xst_castring& strSectionName);

			const	CSection&			GetSection(xst_cwstring& strSectionName);

		protected:

					i32					_LoadAscii(cch8* pFileData, cul32& ulDataSize);

					i32					_LoadUnicode(cch16* pFileData, cul32& ulDataSize);

					CSection*			_GetOrCreateSection(xst_castring& strSectionName);

					CSection*			_GetOrCreateSection(xst_cwstring& strSectionName);

					CSection*			_GetSection(xst_castring& strSectionName);

					CSection*			_GetSection(xst_cwstring& strSectionName);

		protected:

			ASectionMap					m_mASections;
			USectionMap					m_mUSections;

			CSection					m_BlankSection;

			XST::CTextFileReader		m_Reader;
			XST::CFileManager*			m_pFileMgr;

			bool						m_bLoaded;
			ITextFile::FILE_ENCODING	m_eFileEncoding;

			xst_astring					m_strFileName;
	};

}//xst

#endif