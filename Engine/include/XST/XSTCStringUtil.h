
#ifndef XST_CSTRINGUTIL_H
#define XST_CSTRINGUTIL_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTCConsole.h"

#include <iostream>
#include <cmath>

namespace XST
{

	static const ch8 LettersA[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	static const ch8 SignsA[] = "~`!@#$%^&*()_+-=,<.>/?'\";:[{]}\\|";
	static const ch8 NumbersA[] = "0123456789.";

	class StringUtil
	{
		public:

			typedef xst_vector<xst_astring>	TokensA;
			typedef xst_vector<xst_wstring>	TokensW;

			static xst_castring	EmptyAString;
			static xst_cwstring EmptyUString;

		public:

			static f32 ToFloat32(xst_castring& _strString)
			{
				return (f32)atof( _strString.data() );
			}

			static f64 ToFloat64(xst_castring& _strString)
			{
				return atof( _strString.data() );
			}

			static i32 ToInt32(xst_castring& _strString)
			{
				return atoi( _strString.data() );
			}

			static l32 ToLong32(xst_castring& _strString)
			{
				return atol( _strString.data() );
			}

			static f32 ToFloat32(xst_cwstring& _strString)
			{
				return (f32)wcstod( _strString.data(), 0 );
			}

			static f64 ToFloat64(xst_cwstring& _strString)
			{
				return wcstod( _strString.data(), 0 );
			}

			static i32 ToInt32(xst_cwstring& _strString)
			{
				return wcstol( _strString.data(), 0, 10 );
			}

			static l32 ToLong32(xst_cwstring& _strString)
			{
				return wcstol( _strString.data(), 0, 10 );
			}

			static bool IsFloat(lpcastr _lpszStr)
			{
				for(i32 i = 0; _lpszStr[i] != 0; i++)
				{
					if( _lpszStr[i] == '.' )
					{
						return true;
					}
				}

				return false;
			}

			static bool IsFloat(lpcwstr _lpszStr)
			{
				for(i32 i = 0; _lpszStr[i] != 0; i++)
				{
					if( _lpszStr[i] == L'.' )
					{
						return true;
					}
				}

				return false;
			}

			static bool IsNumber(xst_castring& _strString)
			{

				u32 uiDotCount = StringCount(_strString, ".");
				if(uiDotCount > 1 || (uiDotCount == 1 && _strString.length() < 3 ))
					return false;

				size_t uiPos = _strString.find_first_of(XST::LettersA, 0);
				if(uiPos != xst_astring::npos)
					return false;

				uiPos = _strString.find_first_of(XST::SignsA, 0);
				//ch8 ch = _strString.at(uiPos);
				if(uiPos != xst_astring::npos && !(_strString.at(uiPos) == '.' || _strString.at(uiPos) == '-'))
				{
					return false;
				}

				if(_strString.at(_strString.length() - 1) == '.')
					return false;

				return true;
			}


			static u32 StringCount(xst_castring& _strString, xst_castring& _strToFind)
			{
				u32 uiCount = 0;
				size_t uiPos = _strString.find_first_of(_strToFind, 0);
				while(uiPos != xst_astring::npos)
				{
					++uiCount;
					uiPos = _strString.find_first_of(_strToFind, uiPos + 1);
				}

				return uiCount;
			}


			template<typename _T_> static xst_i xst_astring ToString(_T_ _tValue, const u8& _byPrecision = 3)
			{
				std::stringstream stream;
				stream.precision(_byPrecision);
				stream.setf ( std::ios::fixed, std::ios::floatfield );
				stream<<_tValue;
				return stream.str();
			}

			template<typename _T_, const u32 COUNT>
			static xst_i xst_astring ToString(const _T_ _afData[COUNT], const u8& _byPrecision = 3)
			{
				std::stringstream stream;
				stream.precision(_byPrecision);
				stream.setf ( std::ios::fixed, std::ios::floatfield );
				for(u32 i = 0; i < COUNT; ++i)
				{
					stream << _afData[i] << ", ";
				}
				return stream.str();
			}

			template<typename _T_> static xst_i xst_wstring ToWString(_T_ _tValue, const u8& _byPrecision = 3)
			{
				std::wstringstream stream;
				stream.precision(_byPrecision);
				stream.setf ( std::ios::fixed, std::ios::floatfield );
				stream<<_tValue;
				return stream.str();
			}

			template<typename _T_, const u32 COUNT>
			static xst_i xst_wstring ToWString(const _T_ _afData[COUNT], const u8& _byPrecision = 3)
			{
				std::wstringstream stream;
				stream.precision(_byPrecision);
				stream.setf ( std::ios::fixed, std::ios::floatfield );
				for(u32 i = 0; i < COUNT; ++i)
				{
					stream << _afData[i] << ", ";
				}
				return stream.str();
			}

			/*static xst_fi xst_astring UnicodeToAscii(const xst_wstring& _wstrText)
			{
				return xst_astring(_wstrText.begin(), _wstrText.end());
			}*/

			/*static xst_fi xst_wstring AsciiToUnicode(const xst_astring& _astrText)
			{
				return xst_wstring(_astrText.begin(), _astrText.end());
			}*/

			static xst_fi int UnicodeToAscii(u32* _puiCharConverted, ch8* _pDstBuffer, u32 _uiDstSize, const ch16* _pSrcBuffer, u32 _uiSrcSize)
			{
				//return wcstombs_s(_puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize);
				return xst_wcstombs( _puiCharConverted, _pDstBuffer, _uiDstSize, _pSrcBuffer, _uiSrcSize );
			}

			static xst_fi int AsciiToUnicode(ch16* _pDstBuffer, ul32 _ulDstSize, ch8* _pSrcBuffer, ul32 _ulSrcSize)
			{
				u32 uiCharsConverted = 0;
				//return mbstowcs_s(&uiCharsConverted, _pDstBuffer, _ulDstSize, _pSrcBuffer, _ulSrcSize);
				return xst_mbstowcs(&uiCharsConverted, _pDstBuffer, _ulDstSize, _pSrcBuffer, _ulSrcSize);
			}

			static inline int UnicodeToAscii(ch8* pDstBuffer, const ch16* pSrcBuffer, ul32 ulSrcSize)
			{
                pDstBuffer = (ch8*)pSrcBuffer;
                return 0;
			}

			static xst_astring UnicodeToAscii(xst_cwstring& _strWstr, const u32& _uiEncoding = 0)
			{
				if( _strWstr.length() == 0 )
					return "";

				#if defined(XST_WINDOWS)

				int len = WideCharToMultiByte(_uiEncoding, 0, _strWstr.c_str(), _strWstr.length(), 0, 0, 0, 0);
				if (!len)
					return "ErrorW2A";

				std::vector<char> abuff(static_cast<unsigned>(len) + 1);

				// NOTE: this does not NULL terminate the string in abuff, but this is ok
				//       since it was zero-initialized in the vector constructor
				if (!WideCharToMultiByte(_uiEncoding, 0, _strWstr.c_str(), _strWstr.length(),
					&abuff[0], len, 0, 0))
				{
					return "ErrorW2A";
				}//if

				return &abuff[0];
				#else
				#error "Not supoorted"
				return "Not supported";
				#endif
			}

			static xst_wstring AsciiToUnicode(xst_castring& _strStr, const u32& _uiEncoding = 0)
			{
				if( _strStr.length() == 0 )
					return L"";

				#if defined(XST_WINDOWS)
				int len = MultiByteToWideChar(_uiEncoding, 0, _strStr.c_str(), _strStr.length(), 0, 0);
				if (!len)
					return L"ErrorA2W";

				std::vector<wchar_t> wbuff(len + 1);
				// NOTE: this does not NULL terminate the string in wbuff, but this is ok
				//       since it was zero-initialized in the vector constructor
				if (!MultiByteToWideChar(_uiEncoding, 0, _strStr.c_str(), _strStr.length(), &wbuff[0], len))
					return L"ErrorA2W";

				return &wbuff[0];
				#else
				return L"Not supported";
				#endif
			}

			static lpcastr WCharToUTF8(xst_cwstring& rWStr )
			{
				unsigned size = rWStr.size() * sizeof(ch16);
				std::vector<ch8> buffer( size+1, 0 );
				//ch8* buffer = xst_new ch8[size+1];
				#if defined (XST_WINDOWS)
				if (0 == ::WideCharToMultiByte( CP_UTF8, 0, rWStr.c_str(), rWStr.length(), &buffer[0], (int)size, NULL, NULL ))
				{
					DWORD err = GetLastError();

					switch (err)
					{
						case ERROR_INSUFFICIENT_BUFFER:
							//Ogre::LogManager::getSingleton().logMessage( msg + "insufficient buffer", Ogre::LML_CRITICAL );
							break;

						case ERROR_INVALID_FLAGS:
						   // Ogre::LogManager::getSingleton().logMessage( msg + "invalid flags", Ogre::LML_CRITICAL );
							break;

						case ERROR_INVALID_PARAMETER:
						  //  Ogre::LogManager::getSingleton().logMessage( msg + "invalid parameter", Ogre::LML_CRITICAL );
							break;
					};

					return "n/a";
				}
				#else
					return "windows supported for now";
				#endif//XST_WINDOWS

            return (lpcastr)&buffer[0];
        }

			static inline bool AreEquals(lpcwstr _lpszStr1, lpcwstr _lpszStr2)
			{
				return wcscmp(_lpszStr1, _lpszStr2) == 0;
			}

			static inline bool AreEquals(lpcastr _lpszStr1, lpcastr _lpszStr2)
			{
				return strcmp(_lpszStr1, _lpszStr2) == 0;
			}

			static inline u32 GetLength(lpcwstr _lpszStr)
			{
				return (u32)wcslen(_lpszStr);
			}

			static inline u32 GetLength(lpcastr _lpszStr)
			{
				return (u32)strlen(_lpszStr);
			}

			static TokensA Split(const xst_astring &_strData, const xst_astring &_strDelimiters = " ")
			{
				xst_vector<xst_astring> tokens;

				// skip delimiters at beginning.
    			size_t lastPos = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t pos = _strData.find_first_of(_strDelimiters, lastPos);

				while (xst_astring::npos != pos || xst_astring::npos != lastPos)
    			{
        			// found a token, add it to the vector.
        			tokens.push_back(_strData.substr(lastPos, pos - lastPos));

        			// skip __strDataDelimiters.  Note the "not_of"
        			lastPos = _strData.find_first_not_of(_strDelimiters, pos);

        			// find next "non-delimiter"
        			pos = _strData.find_first_of(_strDelimiters, lastPos);
    			}

				return tokens;

			}

			static TokensA SplitExcept(const xst_astring &_strData, const xst_astring& _strExceptFirst, const xst_astring& _strExceptLast, const xst_astring &_strDelimiters = " ")
			{
				xst_vector<xst_astring> tokens;

				// skip delimiters at beginning.
				size_t first = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t last = _strData.find_first_of(_strDelimiters, first);

				size_t uiFirstPos = _strData.find_first_of(_strExceptFirst, 0);
				size_t uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);

				if(first == xst_astring::npos || last == xst_astring::npos)
				{
					tokens.push_back(_strData);
					return tokens;
				}

				while (xst_astring::npos != first || xst_astring::npos != last)
    			{
					if(last == xst_astring::npos || first == xst_astring::npos)
						break;

					if(last < uiLastPos && uiLastPos != xst_astring::npos && (first + 1 == uiFirstPos || first == uiFirstPos ))
					{
						last = _strData.find_first_of(_strExceptFirst, uiLastPos) + 1;
						uiFirstPos = _strData.find_first_of(_strExceptFirst, uiLastPos);
						uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);
					}

					xst_astring str = _strData.substr(first, last - first);

        			tokens.push_back(str);

        			// skip __strDataDelimiters.  Note the "not_of"
					//pos = lastPos + 1;
        			first = _strData.find_first_not_of(_strDelimiters, last);

        			// find next "non-delimiter"
        			last = _strData.find_first_of(_strDelimiters, first);


    			}

				if(first != xst_astring::npos)
				{
					tokens.push_back(_strData.substr(first, _strData.length() - first));
				}

				return tokens;

			}

			static TokensW Split(const xst_wstring &_strData, const xst_wstring &_strDelimiters = L" ")
			{
				xst_vector<xst_wstring> tokens;

				// skip delimiters at beginning.
    			size_t lastPos = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t pos = _strData.find_first_of(_strDelimiters, lastPos);

				while (xst_wstring::npos != pos || xst_wstring::npos != lastPos)
    			{
        			// found a token, add it to the vector.
        			tokens.push_back(_strData.substr(lastPos, pos - lastPos));

        			// skip __strDataDelimiters.  Note the "not_of"
        			lastPos = _strData.find_first_not_of(_strDelimiters, pos);

        			// find next "non-delimiter"
        			pos = _strData.find_first_of(_strDelimiters, lastPos);
    			}

				return tokens;

			}

			static void Split(const xst_astring &_strData, TokensA* _pvOutTokens, const xst_astring &_strDelimiters = " ")
			{

				// skip delimiters at beginning.
    			size_t lastPos = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t pos = _strData.find_first_of(_strDelimiters, lastPos);

				while (xst_astring::npos != pos || xst_astring::npos != lastPos)
    			{
        			// found a token, add it to the vector.
        			_pvOutTokens->push_back(_strData.substr(lastPos, pos - lastPos));

        			// skip __strDataDelimiters.  Note the "not_of"
        			lastPos = _strData.find_first_not_of(_strDelimiters, pos);

        			// find next "non-delimiter"
        			pos = _strData.find_first_of(_strDelimiters, lastPos);
    			}

			}

			static void Split(const xst_astring &_strData, TokensA* _pvOutTokens, const ch8 _strDelimiters = ' ')
			{

				// skip delimiters at beginning.
    			size_t lastPos = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t pos = _strData.find_first_of(_strDelimiters, lastPos);

				while (xst_astring::npos != pos || xst_astring::npos != lastPos)
    			{
        			// found a token, add it to the vector.
        			_pvOutTokens->push_back(_strData.substr(lastPos, pos - lastPos));

        			// skip __strDataDelimiters.  Note the "not_of"
        			lastPos = _strData.find_first_not_of(_strDelimiters, pos);

        			// find next "non-delimiter"
        			pos = _strData.find_first_of(_strDelimiters, lastPos);
    			}

			}

			static void Split(ch8* _lpszData, TokensA* _pvOutTokens, lpcastr _lpszDelimiters = " ")
			{
				#if defined(XST_VISUAL_STUDIO)
				ch8* lpszContext = 0;
				ch8* pch = strtok_s( _lpszData, _lpszDelimiters, &lpszContext );
				while( pch != 0 )
				{
					_pvOutTokens->push_back( pch );
					pch = strtok_s( 0, _lpszDelimiters, &lpszContext );
				}
				#else
				ch8* pch = strtok( _lpszData, _lpszDelimiters );
				while( pch != 0 )
				{
					_pvOutTokens->push_back( pch );
					pch = strtok( 0, _lpszDelimiters );
				}
				#endif
			}

			static void Split(ch16* _lpszData, TokensW* _pvOutTokens, lpcwstr _lpszDelimiters = L" ")
			{
				#if defined(XST_VISUAL_STUDIO)
				ch16* lpszContext = 0;
				ch16* pch = wcstok_s( _lpszData, _lpszDelimiters, &lpszContext );
				while( pch != 0 )
				{
					_pvOutTokens->push_back( pch );
					pch = wcstok_s( 0, _lpszDelimiters, &lpszContext );
				}
				#else
				ch16* pch = wcstok( _lpszData, _lpszDelimiters );
				while( pch != 0 )
				{
					_pvOutTokens->push_back( pch );
					pch = wcstok( 0, _lpszDelimiters );
				}
				#endif
			}

			static void SplitExcept(const xst_astring &_strData, TokensA* _pvOutTokens, const xst_astring& _strExceptFirst, const xst_astring& _strExceptLast, const xst_astring &_strDelimiters = " ")
			{
				//xst_vector<xst_astring> tokens;

				// skip delimiters at beginning.
				size_t first = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t last = _strData.find_first_of(_strDelimiters, first);

				size_t uiFirstPos = _strData.find_first_of(_strExceptFirst, 0);
				size_t uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);

				if(first == xst_astring::npos || last == xst_astring::npos)
				{
					_pvOutTokens->push_back(_strData);
					return;
				}

				while (xst_astring::npos != first || xst_astring::npos != last)
    			{
					if(last == xst_astring::npos || first == xst_astring::npos)
						break;

					if(last < uiLastPos && uiLastPos != xst_astring::npos && (first + 1 == uiFirstPos || first == uiFirstPos ))
					{
						last = _strData.find_first_of(_strExceptFirst, uiLastPos) + 1;
						uiFirstPos = _strData.find_first_of(_strExceptFirst, uiLastPos);
						uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);
					}

					xst_astring str = _strData.substr(first, last - first);

        			_pvOutTokens->push_back(str);

        			// skip __strDataDelimiters.  Note the "not_of"
					//pos = lastPos + 1;
        			first = _strData.find_first_not_of(_strDelimiters, last);

        			// find next "non-delimiter"
        			last = _strData.find_first_of(_strDelimiters, first);


    			}

				if(first != xst_astring::npos)
				{
					_pvOutTokens->push_back(_strData.substr(first, _strData.length() - first));
				}

			}

			static void SplitExcept(const xst_wstring &_strData, TokensW* _pvOutTokens, const xst_wstring& _strExceptFirst, const xst_wstring& _strExceptLast, const xst_wstring &_strDelimiters = L" ")
			{
				//xst_vector<xst_astring> tokens;

				// skip delimiters at beginning.
				size_t first = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t last = _strData.find_first_of(_strDelimiters, first);

				size_t uiFirstPos = _strData.find_first_of(_strExceptFirst, 0);
				size_t uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);

				if(first == xst_wstring::npos || last == xst_wstring::npos)
				{
					_pvOutTokens->push_back(_strData);
					return;
				}

				while (xst_wstring::npos != first || xst_wstring::npos != last)
    			{
					if(last == xst_wstring::npos || first == xst_wstring::npos)
						break;

					if(last < uiLastPos && uiLastPos != xst_astring::npos && (first + 1 == uiFirstPos || first == uiFirstPos ))
					{
						last = _strData.find_first_of(_strExceptFirst, uiLastPos) + 1;
						uiFirstPos = _strData.find_first_of(_strExceptFirst, uiLastPos);
						uiLastPos = _strData.find_first_of(_strExceptLast, uiFirstPos+1);
					}

					xst_wstring str = _strData.substr(first, last - first);

        			_pvOutTokens->push_back(str);

        			// skip __strDataDelimiters.  Note the "not_of"
					//pos = lastPos + 1;
        			first = _strData.find_first_not_of(_strDelimiters, last);

        			// find next "non-delimiter"
        			last = _strData.find_first_of(_strDelimiters, first);


    			}

				if(first != xst_astring::npos)
				{
					_pvOutTokens->push_back(_strData.substr(first, _strData.length() - first));
				}

			}

			static void Split(const xst_wstring &_strData, TokensW* _pvOutTokens, const xst_wstring &_strDelimiters = L" ")
			{
				//xst_vector<xst_wstring> tokens;

				// skip delimiters at beginning.
    			size_t lastPos = _strData.find_first_not_of(_strDelimiters, 0);

				// find first "non-delimiter".
    			size_t pos = _strData.find_first_of(_strDelimiters, lastPos);

				while (xst_wstring::npos != pos || xst_wstring::npos != lastPos)
    			{
        			// found a token, add it to the vector.
        			_pvOutTokens->push_back(_strData.substr(lastPos, pos - lastPos));

        			// skip __strDataDelimiters.  Note the "not_of"
        			lastPos = _strData.find_first_not_of(_strDelimiters, pos);

        			// find next "non-delimiter"
        			pos = _strData.find_first_of(_strDelimiters, lastPos);
    			}

			}

			static xst_astring Extract(xst_castring& _strString, xst_castring& _strFirst, xst_castring& _strLast)
			{

				size_t uiFirstPos = _strString.find_first_of(_strFirst, 0);
				size_t uiLastPos = _strString.find_first_of(_strLast, uiFirstPos+1);

				return _strString.substr(uiFirstPos+1, uiLastPos-1);
			}

			static xst_wstring Extract(xst_cwstring& _strString, xst_cwstring& _strFirst, xst_cwstring& _strLast)
			{

				size_t uiFirstPos = _strString.find_first_of(_strFirst, 0);
				size_t uiLastPos = _strString.find_first_of(_strLast, uiFirstPos+1);

				return _strString.substr(uiFirstPos+1, uiLastPos-1);
			}

			static xst_string Trim(const xst_string &_strData)
			{
				xst_string str = _strData;

				size_t pos = str.find_last_not_of(XST_TEXT(' '));

				if(pos != xst_string::npos)
				{
					str.erase(pos + 1);
					pos = str.find_first_not_of(XST_TEXT(' '));
					if(pos != xst_string::npos) str.erase(0, pos);
				}

				else str.erase(str.begin(), str.end());

				return str;
			}

			static xst_astring ReplaceAll(xst_astring _strString, const xst_astring& _strSearch, const xst_astring& _strReplace)
			{
				xst_astring::size_type stPos = 0;
				size_t uiSize = _strSearch.size();

				while( (stPos = _strString.find(_strSearch, stPos)) != xst_astring::npos )
				{
					_strString.replace(stPos, uiSize, _strReplace);
					++stPos;
				}
				return _strString;
			}

			static xst_wstring ReplaceAll(xst_wstring _strString, const xst_wstring& _strSearch, const xst_wstring& _strReplace)
			{
				xst_wstring::size_type stPos = 0;
				size_t uiSize = _strSearch.size();

				while( (stPos = _strString.find(_strSearch, stPos++)) != xst_wstring::npos )
				{
					_strString.replace(stPos, uiSize, _strReplace);
				}
				return _strString;
			}

			static xst_astring& ToUpper(xst_astring& _strString)
			{
				for(xst_astring::iterator Itr = _strString.begin(); Itr != _strString.end(); ++Itr)
				{
					//_strString[i] = toupper(_strString[i]);
					*Itr = (char)toupper(*Itr);
				}

				return _strString;
			}

			static xst_wstring& ToUpper(xst_wstring& _strString)
			{
				for(xst_wstring::iterator Itr = _strString.begin(); Itr != _strString.end(); ++Itr)
				{
					//_strString[i] = toupper(_strString[i]);
					*Itr = towupper(*Itr);
				}

				return _strString;
			}

			static xst_astring& ToLower(xst_astring& _strString)
			{
				for(xst_astring::iterator Itr = _strString.begin(); Itr != _strString.end(); ++Itr)
				{
					//_strString[i] = toupper(_strString[i]);
					*Itr = (char)tolower(*Itr);
				}

				return _strString;
			}

			static void xst_i ToLower(lpcastr _strString, ul32 _ulLength, lpastr* ppOut)
			{
				lpastr pTmp = *ppOut;
				for(u32 i = 0; i < _ulLength; ++i)
				{
					pTmp[i] = (char)tolower( _strString[ i ] );
				}
			}

			static xst_astring ToLower(xst_castring& _strString)
			{
				xst_astring strString;

				for(u32 i = 0; i < _strString.length(); ++i)
				{
					strString += (char)tolower( _strString[i] );
				}

				return strString;
			}


			static xst_wstring& ToLower(xst_wstring& _strString)
			{
				for(xst_wstring::iterator Itr = _strString.begin(); Itr != _strString.end(); ++Itr)
				{
					*Itr = towlower(*Itr);
				}

				return _strString;
			}

			static lpcwstr ToLower(lpwstr _strString, ul32 _ulLength)
			{
				for(u32 i = 0; i < _ulLength; ++i)
				{
					_strString[i] = towlower( _strString[ i ] );
				}

				return _strString;
			}

			static xst_wstring ToLower(xst_cwstring& _strString)
			{
				xst_wstring strString;

				for(u32 i = 0; i < _strString.length(); ++i)
				{
					strString += towlower( _strString[i] );
				}

				return strString;
			}

			static xst_astring Format(lpcastr strFormat, ...)
			{
				xst_astring retStr("");

				if (NULL != strFormat)
				{
					va_list marker = NULL;

					// initialize variable arguments
					va_start(marker, strFormat);

					// Get formatted string length adding one for NULL
					size_t len = (size_t)xst_vscprintf( (const char*)strFormat, marker) + 1;

					// Create a char vector to hold the formatted string.
					xst_vector<char> buffer(len, '\0');
					//int nWritten = _vsnprintf_s(&buffer[0], buffer.size(), len, strFormat, marker);
					i32 iWritten = xst_vsnprintf( &buffer[0], buffer.size(), len, strFormat, marker );

					if (iWritten > 0)
					{
						retStr = &buffer[0];
					}

					// Reset variable arguments
					va_end(marker);
				}

				return retStr;
			}

	};

}//XST

#endif //XST_CSTRINGUTIL_H
