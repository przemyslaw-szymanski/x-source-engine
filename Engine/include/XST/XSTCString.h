#ifndef XST_STRING_H
#define XST_STRING_H

#include "XSTCommonTypes.h"

#include "XSTCHash.h"
#include "XSTCStringUtil.h"
/*
#include "XSTCLock.h"
*/
#include "XSTMath.h"


namespace XST
{
	using namespace Types;

	#define XST_STR_SHORT_NAME	8
	#define XST_STR_NAME		16
	#define XST_STR_LONG_NAME	32
	#define XST_STR_SHORT		64
	#define XST_STR_NORMAL		128
	#define XST_STR_LONG		256
	#define XST_STR_SHORT_TEXT	512
	#define XST_STR_TEXT		1024
	#define XST_STR_LONG_TEXT	2048

	template<typename, cu32> class CConstLengthString;
	template<cu32> class CConstLengthAsciiString;
	template<cu32> class CConstLengthUnicodeString;

	typedef CConstLengthAsciiString< XST_STR_SHORT_NAME >	xst_short_name_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_NAME >			xst_name_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_LONG_NAME >	xst_long_name_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_SHORT >		xst_short_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_NORMAL >		xst_normal_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_LONG >			xst_long_ascii_string;
	typedef CConstLengthAsciiString< XST_STR_SHORT_TEXT >	xst_short_ascii_text;
	typedef CConstLengthAsciiString< XST_STR_TEXT >			xst_ascii_text;
	typedef CConstLengthAsciiString< XST_STR_LONG_TEXT >	xst_long_ascii_text;


	typedef CConstLengthUnicodeString< XST_STR_SHORT_NAME >	xst_short_name_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_NAME >		xst_name_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_LONG_NAME >	xst_long_name_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_SHORT >		xst_short_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_NORMAL >		xst_normal_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_LONG >		xst_long_unicode_string;
	typedef CConstLengthUnicodeString< XST_STR_SHORT_TEXT >	xst_short_unicode_text;
	typedef CConstLengthUnicodeString< XST_STR_TEXT >		xst_unicode_text;
	typedef CConstLengthUnicodeString< XST_STR_LONG_TEXT >	xst_long_unicode_text;

	typedef xst_short_name_ascii_string		xst_astr8;
	typedef xst_name_ascii_string			xst_astr16;
	typedef xst_long_name_ascii_string		xst_astr32;
	typedef xst_short_ascii_string			xst_astr64;
	typedef xst_normal_ascii_string			xst_astr128;
	typedef xst_long_ascii_string			xst_astr256;
	typedef xst_short_ascii_text			xst_astr512;
	typedef xst_ascii_text					xst_astr1024;
	typedef xst_long_ascii_text				xst_astr2048;
	typedef xst_ascii_text					xst_astr1k;
	typedef xst_long_ascii_text				xst_astr2k;

	typedef xst_short_name_unicode_string	xst_wstr8;
	typedef xst_name_unicode_string			xst_wstr16;
	typedef xst_long_name_unicode_string	xst_wstr32;
	typedef xst_short_unicode_string		xst_wstr64;
	typedef xst_normal_unicode_string		xst_wstr128;
	typedef xst_long_unicode_string			xst_wstr256;
	typedef xst_short_unicode_text			xst_wstr512;
	typedef xst_unicode_text				xst_wstr1024;
	typedef xst_long_unicode_text			xst_wstr2048;
	typedef xst_unicode_text				xst_wstr1k;
	typedef xst_long_unicode_text			xst_wstr2k;

	typedef const xst_astr8					xst_castr8;
	typedef const xst_astr16				xst_castr16;
	typedef const xst_astr32				xst_castr32;
	typedef const xst_astr64				xst_castr64;
	typedef const xst_astr128				xst_castr128;
	typedef const xst_astr256				xst_castr256;
	typedef const xst_astr512				xst_castr512;
	typedef const xst_astr1024				xst_castr1024;
	typedef const xst_astr2048				xst_castr2048;

	typedef const xst_wstr8					xst_cwstr8;
	typedef const xst_wstr16				xst_cwstr16;
	typedef const xst_wstr32				xst_cwstr32;
	typedef const xst_wstr64				xst_cwstr64;
	typedef const xst_wstr128				xst_cwstr128;
	typedef const xst_wstr256				xst_cwstr256;
	typedef const xst_wstr512				xst_cwstr512;
	typedef const xst_wstr1024				xst_cwstr1024;
	typedef const xst_wstr2048				xst_cwstr2048;

	typedef xst_astr64						xst_file_name;
	typedef xst_castr64						xst_cfile_name;
	typedef xst_file_name					xst_fname;
	typedef xst_cfile_name					xst_cfname;
	typedef xst_astr32						xst_aname;
	typedef xst_castr32						xst_caname;
	typedef xst_wstr32						xst_wname;
	typedef xst_cwstr32						xst_cwname;

	template<typename _CHAR_TYPE_, cu32 _MAX_LENGTH_>
	class CConstLengthString
	{
		public:

			typedef _CHAR_TYPE_		CHAR;
			typedef const CHAR*		CHAR_STR;

			enum
			{
				MAX_LENGTH = _MAX_LENGTH_,
				CHAR_SIZE = sizeof( _CHAR_TYPE_ ),
				DATA_SIZE = _MAX_LENGTH_ * CHAR_SIZE
			};

			static const i32	npos = -1;

		protected:

			CHAR	m_pData[ MAX_LENGTH + 1 ];
			ul32	m_ulLength;

		public:

								CConstLengthString() : m_ulLength(0)//, m_pData(0)
								{
								}

								CConstLengthString(CHAR_STR _strString)
								{
									this->operator=( _strString );
								}

								CConstLengthString(const CConstLengthString& _strString)
								{
									this->operator=( _strString );
								}

								CConstLengthString(const std::basic_string< CHAR >& _strString)
								{
									this->operator=( _strString );
								}

								template<typename _CH_TYPE_, cu32 _LEN_>
								CConstLengthString(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strString)
								{
									cul32 ulLength = XST::Math::Min<i32>( _strString.length(), MAX_LENGTH );
									memcpy_s( m_pData, DATA_SIZE, _strString.data(), DATA_SIZE );
									m_pData[ulLength] = 0;
									m_ulLength = ulLength;
								}

								~CConstLengthString()
								{
								}

			/*xst_fi	operator size_t() const
			{
				return *(size_t*)this->data();
			}*/

			xst_fi	void		operator=(CHAR_STR str)
								{ assign( str ); }

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	void		operator=(const CConstLengthString< _CH_TYPE_, _LEN_ >& strString)
								{ assign( strStrin ); }

			xst_fi	void		operator=(const xst_basic_string< CHAR >& strString)
								{ assign( strString ); }

			xst_fi	void		operator+=(CHAR_STR str)
								{ append( str ); }

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	void		operator+=(const CConstLengthString< _CH_TYPE_, _LEN_ >& strString)
								{ append( strString ); }

			xst_fi	void		operator+=(const xst_basic_string< CHAR >& strString)
								{ append( strString ); }

			xst_fi CConstLengthString operator+(CHAR_STR right)
			{
				return CConstLengthString::Add( *this, right );
			}

			static xst_i CConstLengthString Add(const CConstLengthString& Left, const CConstLengthString& Right)
			{
				return Add( Left, Right.data(), Right.length() );
			}

			static xst_i CConstLengthString Add(const CConstLengthString& Left, CHAR_STR Right)
			{
				return Add( Left, Right, length( Right ) );
			}

			static xst_i CConstLengthString Add(const CConstLengthString& Left, CHAR_STR Right, cu32& uiRightLength)
			{
				CConstLengthString Out;
				Out.append( Left );
				Out.append( Right, uiRightLength );
				return Out;
			}

			template<typename _CH_TYPE_, cu32 _LEN_> 
			static xst_i CConstLengthString< _CH_TYPE_, _LEN_ > Add(const CConstLengthString< _CH_TYPE_, _LEN_ >& Left, const xst_basic_string< _CH_TYPE_ >& Right)
			{
				return Add( Left, Right.data(), Right.length() );
			}

			xst_fi	CHAR		operator[](cul32& _ulPos) const
			{
				xst_assert( _ulPos < m_ulLength, "([]) _ulPos is too far." );
				return m_pData[ _ulPos ];
			}

			xst_fi	bool		operator<(CHAR_STR _strString) const
			{
				return this->m_ulLength < length( _strString );
			}

			/*xst_fi	bool		operator<(const CConstLengthString< _CHAR_TYPE_, _MAX_LENGTH_ >& _strRight)
			{
				return this->m_ulLength < _strRight.m_ulLength;
			}*/

			xst_fi	bool		operator<(const CConstLengthString& _strRight) const
			{
				return this->m_ulLength < _strRight.m_ulLength;
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator<(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strString)
			{
				return this->m_ulLength < _strString.length();
			}

			xst_fi	bool		operator<(const std::basic_string< CHAR >& _strString)
			{
				return this->m_ulLength < _strString.length();
			}

			xst_fi	bool		operator<=(CHAR_STR _strString)
			{
				return this->m_ulLength <= length( _strString );
			}

			xst_fi	bool		operator<=(const CConstLengthString< _CHAR_TYPE_, _MAX_LENGTH_ >& _strRight)
			{
				return this->m_ulLength <= _strRight.m_ulLength;
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator<=(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strString)
			{
				return this->m_ulLength <= _strString.length();
			}

			xst_fi	bool		operator<=(const std::basic_string< CHAR >& _strString)
			{
				return this->m_ulLength <= _strString.length();
			}

			xst_fi	bool		operator>(CHAR_STR _strString)
			{
				return this->m_ulLength > length( _strString );
			}

			xst_fi	bool		operator>(const CConstLengthString< _CHAR_TYPE_, _MAX_LENGTH_ >& _strRight)
			{
				return this->m_ulLength > _strRight.m_ulLength;
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator>(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strString)
			{
				return this->m_ulLength > _strString.length();
			}

			xst_fi	bool		operator>(const std::basic_string< CHAR >& _strString)
			{
				return this->m_ulLength > _strString.length();
			}

			xst_fi	bool		operator>=(CHAR_STR _strString)
			{
				return this->m_ulLength >= length( _strString );
			}

			xst_fi	bool		operator>=(const CConstLengthString< _CHAR_TYPE_, _MAX_LENGTH_ >& _strRight)
			{
				return this->m_ulLength >= _strRight.m_ulLength;
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator>=(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strString)
			{
				return this->m_ulLength >= _strString.length();
			}

			xst_fi	bool		operator>=(const std::basic_string< CHAR >& _strString)
			{
				return this->m_ulLength >= _strString.length();
			}

			xst_fi	bool		operator()(CHAR_STR _strRight)
			{
				return this->m_ulLength < length( _strRight );
			}

			xst_fi	bool		operator()(const CConstLengthString& _strRight)
			{
				return this->m_ulLength < _strRight.m_ulLength;
			}

			xst_fi	bool		operator()(const CConstLengthString& _strLeft, const CConstLengthString& _strRight)
			{
				return _strLeft->m_ulLength < _strRight->m_ulRight;
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator()(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strLeft, const CConstLengthString< _CH_TYPE_, _LEN_ >& _strRight)
			{
				return _strLeft.length() < _strRight.length();
			}

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_fi	bool		operator()(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strRight)
			{
				return this->m_ulLength < _strRight.length();
			}

			xst_fi	bool		operator==(CHAR_STR _strRight) const
			{
				return equals( _strRight );
			}

			xst_fi	bool		operator==(const CConstLengthString& _strRight) const
			{
				return equals( _strRight );
			}

			template<typename _CHAR_T_, cu32 _LEN_>
			xst_fi	bool		operator==(const CConstLengthString< _CHAR_T_, _LEN_ >& _strRight) const
			{
				return equals( _strRight );
			}

			template<typename _CHAR_T_, cu32 _LEN_>
			xst_fi operator CConstLengthString< _CHAR_T_, _LEN_ >()
			{
				return m_pData;
			}

			xst_i	bool		equals(CHAR_STR _strRight) const
			{
				for(u32 i = 0; i < m_ulLength && _strRight[i] != 0; ++i)
				{
					if( m_pData[i] != _strRight[i] )
						return false;
				}

				return true;
			}

			xst_i	bool		equals(const CConstLengthString& _strRight) const
			{
				if( m_ulLength != _strRight.m_ulLength )
					return false;

				for(i32 i = m_ulLength; i -->0;)
				{
					if( m_pData[i] != _strRight[i] )
						return false;
				}

				return true;
			}

			template<typename _CHAR_T_, cu32 _LEN_>
			xst_i	bool		equals(const CConstLengthString< _CHAR_T_, _LEN_ >& _strRight) const
			{
				if( m_ulLength != _strRight.m_ulLength )
					return false;

				for(i32 i = m_ulLength; i -->0;)
				{
					if( m_pData[i] != _strRight[i] )
						return false;
				}

				return true;
			}

			xst_i	bool		equals(const std::basic_string< CHAR >& _strRight) const
			{
				if( m_ulLength != _strRight.length() )
					return false;

				for(i32 i = m_ulLength; i -->0;)
				{
					if( m_pData[i] != _strRight[i] )
						return false;
				}

				return true;
			}

			xst_fi	void		clear()
			{
				m_pData[0] = 0;
				m_ulLength = 0;
			}

			xst_fi	CHAR		at(cu32& _uiPos) const
			{
				xst_assert( _uiPos < m_ulLength && _uiPos >= 0, "Position out of range." );
				return m_pData[ _uiPos ];
			}

			xst_fi	ul32		length() const
								{ return m_ulLength; }

			xst_fi	ul32		max_size() const
								{ return MAX_LENGTH; }

			xst_fi	CHAR_STR	data() const
								{ return m_pData; }

			xst_fi	CHAR_STR	c_str() const
								{ return m_pData; }

			xst_fi	CHAR*		data()
								{ return m_pData; }

			xst_fi	ul32		size() const
								{ return m_ulLength; }

			xst_fi	ul32		capacity() const
								{ return sizeof( m_pData ); }

			xst_fi	bool		append(const CConstLengthString& Str)
								{ return append( Str.data(), Str.length() ); }

			xst_fi	bool		append(const xst_basic_string< CHAR >& Str)
								{ return append( Str.data(), Str.length() ); }

			xst_fi	bool		append(CHAR_STR str)
								{ return append( str, length( str ) ); }

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_i	bool		append(const CConstLengthString< _CH_TYPE_, _LEN_ >& strString)
								{ return append( strString.data(), strString.length() ); }

			xst_i	bool		append(CHAR_STR Str, cu32& uiStrLength)
			{
				u32 uiCurrLen = 0;
				for(; uiCurrLen < uiStrLength && m_ulLength < MAX_LENGTH; ++uiCurrLen, ++m_ulLength)
				{
					m_pData[ m_ulLength ] = Str[ uiCurrLen ];
				}

				m_pData[ m_ulLength ] = 0;

				return uiCurrLen == uiStrLength;
			}

			xst_fi	bool		assign(const CConstLengthString& Str)
								{ return assign( Str.data(), Str.length() ); }

			xst_fi	bool		assign(const xst_basic_string< CHAR >& Str)
								{ return assign( Str.data(), Str.length() ); }

			xst_fi	bool		assign(CHAR_STR str)
								{ return assign( str, length( str ) ); }

			template<typename _CH_TYPE_, cu32 _LEN_>
			xst_i	bool		assign(const CConstLengthString< _CH_TYPE_, _LEN_ >& strString)
								{ return assign( strString.data(), strString.length() ); }

			xst_fi	bool		assign(CHAR_STR str, cu32& uiStrLength)
			{
				m_ulLength = 0;
				return append( str, uiStrLength );
			}

			i32			find_first_of(CHAR_STR _strString, cul32& _ulPos = 0) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				for(ul32 i = _ulPos; i < m_ulLength; ++i)
				{
					//pData = m_pData + i;
					bEquals = true;
					for(ul32 j = 0; _strString[ j ] != 0; ++j)
					{
						if( m_pData[ i + j ] != _strString[ j ] )
						{
							bEquals = false;
							break;
						}
					}

					if( bEquals )
						return i;
				}

				return -1;
			}

			i32			find_first_of(CHAR_STR _strString, cul32& _ulPos, cul32& _ulN) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				for(ul32 i = _ulPos; i < m_ulLength; ++i)
				{
					//pData = m_pData + i;
					bEquals = true;
					for(ul32 j = 0; j < _ulN; ++j)
					{
						if( m_pData[ i + j ] != _strString[ j ] )
						{
							bEquals = false;
							break;
						}
					}

					if( bEquals )
						return i;
				}

				return -1;
			}

			i32			find_first_of(CHAR _tChar, cul32& _ulPos = 0) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				for(ul32 i = _ulPos; i < m_ulLength; ++i)
				{
					if( m_pData[ i ] == _tChar )
						return i;
				}

				return -1;
			}

			xst_fi	i32			find_first_of(const CConstLengthString& _strString, cul32& _ulPos = 0) const
			{
				return find_first_of( _strString.m_pData, _ulPos, _strString.m_ulLength );
			}


			i32			find_last_of(CHAR_STR _strString, cul32& _ulPos = 0) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				ul32 l = length( _strString );
				for(l32 i = m_ulLength - 1; i >= _ulPos; --i)
				{
					//pData = m_pData + i;
					bEquals = true;
					for(l32 j = l - 1; j >= 0; --j)
					{
						if( m_pData[ i - j ] != _strString[ j ] )
						{
							bEquals = false;
							break;
						}
					}

					if( bEquals )
						return i;
				}

				return -1;
			}

			i32			find_last_of(CHAR_STR _strString, cul32& _ulPos, cul32& _ulN) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				for(l32 i = m_ulLength - 1; i >= _ulPos; --i)
				{
					//pData = m_pData + i;
					bEquals = true;
					for(l32 j = _ulN - 1; j >= 0; --j)
					{
						if( m_pData[ i - j ] != _strString[ j ] )
						{
							bEquals = false;
							break;
						}
					}

					if( bEquals )
						return i;
				}

				return -1;
			}

			i32			find_last_of(CHAR _tChar, cul32& _ulPos = 0) const
			{
				xst_assert( _ulPos < m_ulLength, "Search position is to far." );
				//CHAR_STR pData;
				bool bEquals;
				for(l32 i = m_ulLength - 1; i >= _ulPos; --i)
				{
					if( m_pData[ i ] == _tChar )
						return i;
				}

				return -1;
			}

			xst_fi	i32			find_last_of(const CConstLengthString& _strString, cul32& _ulPos = 0) const
			{
				return find_first_of( _strString.m_pData, _ulPos, _strString.m_ulLength );
			}

			xst_fi CHAR			at(cul32& _ulPos)
			{
				xst_assert( _ulPos < m_ulLength, "(at) _ulPos too far." );
				return m_pData[ _ulPos ];
			}

			xst_i static ul32	length(CHAR_STR _strString)
			{
				ul32 ulLen;
				for(ulLen = 0; _strString[ ulLen ] != 0; ++ulLen) {}
				return ulLen;
			}

			xst_i CConstLengthString substr(ul32 _ulStartPos, ul32 _ulCount = -1 )
			{
				CHAR buff[ MAX_LENGTH ];
				if( _ulCount == -1 ) _ulCount = m_ulLength;
				xst_memcpy( buff, MAX_LENGTH * sizeof( CHAR ), m_pData + _ulStartPos, _ulCount * sizeof( CHAR ) );
				return CConstLengthString( buff );
			}

			xst_i CConstLengthString substr(ul32 _ulStartPos, ul32 _ulCount = -1 ) const
			{
				CHAR buff[ MAX_LENGTH ];
				if( _ulCount == -1 ) _ulCount = m_ulLength;
				xst_memcpy( buff, MAX_LENGTH * sizeof( CHAR ), m_pData + _ulStartPos, _ulCount * sizeof( CHAR ) );
				return CConstLengthString( buff );
			}

			xst_fi	ul32		hash() const
			{
				return CHash::GetCRC( m_pData, m_ulLength );
			}

			xst_i static ul32	hash(CHAR_STR _strString, cul32& _ulLength)
			{
				return CHash::GetCRC( _strString, _ulLength );
			}

			xst_fi operator size_t() const
			{
				return m_ulLength;
			}

			//CMutex	m_Mutex;
	};

	template<typename _CH_TYPE_, cu32 _LEN_>
	xst_fi bool operator==(const CConstLengthString< _CH_TYPE_, _LEN_ >& _strLeft, const CConstLengthString< _CH_TYPE_, _LEN_ >& _strRight)
	{
		return _strLeft.equals( _strRight );
	}

	template<typename _CH_TYPE_, cu32 _LEN_>
	xst_fi xst_ostream& operator<<(xst_ostream& out, const CConstLengthString< _CH_TYPE_, _LEN_ >& Right)
	{
		out << Right.c_str();
		return out;
	}

	/*template<class _STRING_>
	xst_fi bool operator==(const _STRING_& _strLeft, const _STRING_& _strRight)
	{
		return _strLeft.equals( _strRight );
	}*/

	template<cu32 _MAX_LENGTH_>
	class CConstLengthAsciiString : public CConstLengthString<ch8, _MAX_LENGTH_>
	{
	    public:

            //typedef CConstLengthString<ch8, _MAX_LENGTH_>::CHAR_STR CHAR_STR;
            typedef ch8		CHAR;
			typedef const CHAR*		CHAR_STR;
			typedef CConstLengthString<ch8, _MAX_LENGTH_>   BaseClass;

		public:

								CConstLengthAsciiString()
								{
								}

				    			CConstLengthAsciiString(CHAR_STR _strString) : CConstLengthString<ch8, _MAX_LENGTH_>( _strString )
								{
									//this->operator=( _strString );
								}

								CConstLengthAsciiString(const CConstLengthAsciiString& _strString) : CConstLengthString<ch8, _MAX_LENGTH_>( _strString )
								{
									//this->operator=( _strString );
								}

								CConstLengthAsciiString(const CConstLengthString<ch8, _MAX_LENGTH_>& _strString) : CConstLengthString<ch8, _MAX_LENGTH_>( _strString )
								{
									//this->operator=( _strString );
								}

								CConstLengthAsciiString(const xst_astring& _strString)
								{
									this->operator=( _strString );
								}

								CConstLengthAsciiString(const xst_wstring& _strString)
								{
									this->operator=( _strString );
								}

								~CConstLengthAsciiString()
								{
								}

			xst_i	void		operator=(CHAR_STR _strString)
			{
				ul32 ulLen;
				for(ulLen = 0; _strString[ulLen] != 0; ulLen++) { }
				this->m_ulLength = XST::Math::Min<ul32>( ulLen, CConstLengthString<ch8, _MAX_LENGTH_>::MAX_LENGTH );
				memcpy_s( this->m_pData, CConstLengthString<ch8, _MAX_LENGTH_>::DATA_SIZE, _strString, CConstLengthString<ch8, _MAX_LENGTH_>::DATA_SIZE );
				this->m_pData[ this->m_ulLength ] = 0;
			}

			xst_i	void		operator=(const xst_astring& _strString)
			{
				cul32 ulLength = XST::Math::Min< ul32 >( _strString.length(), CConstLengthString<ch8, _MAX_LENGTH_>::MAX_LENGTH );
				memcpy_s( this->m_pData, sizeof( this->m_pData ), _strString.data(), sizeof( this->m_pData ) );
				this->m_pData[ulLength] = 0;
				this->m_ulLength = ulLength;
			}

			xst_fi	void		operator=(const xst_wstring& _strString)
			{
				this->operator =( XST::StringUtil::UnicodeToAscii( _strString ) );
			}

			xst_fi bool			operator==(const CConstLengthAsciiString& _strRight)
			{
				return this->equals( _strRight );
			}

			xst_fi operator size_t() const
			{
				return this->m_ulLength;
			}
	};


	template<cu32 _MAX_LENGTH_>
	#if defined (XST_WINDOWS)
	class CConstLengthUnicodeString : public CConstLengthString<ch16, _MAX_LENGTH_>
	#else
	class CConstLengthUnicodeString : public CConstLengthString<u16, _MAX_LENGTH_>
	#endif
	{
	    public:

            #if defined (XST_WINDOWS)
            typedef ch16    CHAR;
            #else
            typedef u16     CHAR;
            #endif

            typedef const CHAR* CHAR_STR;
            typedef CConstLengthString<CHAR, _MAX_LENGTH_>  BaseClass;

		public:

								CConstLengthUnicodeString()
								{
								}

								CConstLengthUnicodeString(CHAR_STR _strString) : BaseClass( _strString )
								{
									//this->operator=( _strString );
								}

								//CConstLengthUnicodeString(const CConstLengthAsciiString& _strString) : CConstLengthString( _strString )
								//{
								//	//this->operator=( _strString );
								//}

								CConstLengthUnicodeString(const BaseClass& _strString) : BaseClass( _strString )
								{
									//this->operator=( _strString );
								}

								CConstLengthUnicodeString(const xst_astring& _strString)
								{
									this->operator=( _strString );
								}

								CConstLengthUnicodeString(const xst_wstring& _strString)
								{
									this->operator=( _strString );
								}

								~CConstLengthUnicodeString()
								{
								}

			xst_fi	void		operator=(const xst_astring& _strString)
			{
				this->operator=( XST::StringUtil::AsciiToUnicode( _strString ) );
			}


			xst_fi	void		operator=(const xst_wstring& _strString)
			{
				#if defined (XST_WINDOWS)
					cul32 ulLength = Min( _strString.length(), BaseClass::MAX_LENGTH );
					memcpy_s( this->m_pData, sizeof( this->m_pData ), _strString.data(), sizeof( this->m_pData ) );
					this->m_pData[ this->ulLength ] = 0;
					this->m_ulLength = ulLength;
				#else
					this->m_pData[0] = 0;
					this->m_ulLength = 0;
				#endif
			}

			xst_fi lpcwstr		wc_str() const
								{ return (lpcwstr)this->m_pData; }

			xst_fi bool			operator==(const CConstLengthUnicodeString& _strRight)
			{
				return this->equals( _strRight );
			}

			xst_fi operator size_t() const
			{
				return *(size_t*)this->data();
			}

	};

	template<cu32 _MAX_LENGTH_>
	struct TSSimpleString
	{
		enum
		{
			MAX_LENGTH = _MAX_LENGTH_
		};

		void Set(lpcastr _lpszData)
		{
			#if defined (XST_VISUAL_STUDIO)
				memcpy_s( strData, MAX_LENGTH, _lpszData, MAX_LENGTH );

			#else
				memcpy( strData, _lpszData, MAX_LENGTH );
			#endif
				strData[ MAX_LENGTH ] = 0;
		}

		operator lpcastr()
		{
			return strData;
		}

		ch8 strData[ MAX_LENGTH + 1 ];
		u32	ulLength;
	};

}//xst

#if defined (_XHASH_)

template< > xst_fi
std::size_t stdext::hash_value< XST::xst_astr8 >( XST::xst_castr8& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr16 >(  XST::xst_castr16& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr32 >(  XST::xst_castr32& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr64 >(  XST::xst_castr64& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr128 >(  XST::xst_castr128& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr256 >(  XST::xst_castr256& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr512 >(  XST::xst_castr512& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value< XST::xst_astr1024 >(  XST::xst_castr1024& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_astr2048 >(  XST::xst_castr2048& s )
{
	return stdext::hash_value( s.data() );
}

template< > xst_fi
std::size_t stdext::hash_value<  XST::xst_wstr8 >(  XST::xst_cwstr8& s )
{
	return stdext::hash_value( s.data() );
}

#endif//_XHASH_

#endif//XSTCString
