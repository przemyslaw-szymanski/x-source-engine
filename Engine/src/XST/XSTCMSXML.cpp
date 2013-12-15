#include "../../include/xst/XSTCMSXML.h"
#include "../../include/xst/XSTCStringUtil.h"
#include "../../include/xst/XSTCLogger.h"
/*
#if defined (XST_WINDOWS) && defined (XST_VISUAL_STUDIO)

// Macro that calls a COM method returning HRESULT value:
#define XST_HRCALL(a, errmsg) \
do { \
    hr = (a); \
    if (FAILED(hr)) { \
        goto clean; \
    } \
} while (0)


namespace XST
{
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //PRIVATE

        // Helper function to create a DOM instance:
		MSXML2::IXMLDOMDocument* CMSXML::DomFromCOM()
        {

           HRESULT hr;
           MSXML2::IXMLDOMDocument *pxmldoc = NULL;

           XST_HRCALL( CoCreateInstance(__uuidof(MSXML2::DOMDocument30),NULL,CLSCTX_INPROC_SERVER,__uuidof(MSXML2::IXMLDOMDocument),(void**)&pxmldoc), "Create a new DOMDocument");
           XST_HRCALL( pxmldoc->put_async(VARIANT_FALSE),"should never fail");
           XST_HRCALL( pxmldoc->put_validateOnParse(VARIANT_FALSE), "should never fail");
           XST_HRCALL( pxmldoc->put_resolveExternals(VARIANT_FALSE), "should never fail");

           return pxmldoc;

           clean:
                if (pxmldoc)
                {
                        pxmldoc->Release();
                        pxmldoc = 0;
                }

                return NULL;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //PUBLIC

        CMSXML::CMSXML() : m_pXMLDoc(0)
        {
        }

        CMSXML::~CMSXML()
        {
                if( m_pXMLDoc )
                        m_pXMLDoc->Release();
        }

        int CMSXML::Load(lpcastr _lpszFilename)
        {
                return Load(XST::StringUtil::AsciiToUnicode(_lpszFilename).data());
        }

        int CMSXML::Load(lpcwstr _lpszFilename)
        {

                VARIANT_BOOL Status;
                VARIANT Var;
                HRESULT hr;
                BSTR lpszStr = 0;

                CoInitialize(0);
                
                m_pXMLDoc = DomFromCOM();
                if(!m_pXMLDoc)
                {
                        goto clean;
                }
        
                VariantInit(&Var);
                V_BSTR(&Var) = SysAllocString(_lpszFilename);
                V_VT(&Var) = VT_BSTR;
                XST_HRCALL(m_pXMLDoc->load(Var, &Status), "");

                if(Status != VARIANT_TRUE)
                {
                        XST_HRCALL(m_pXMLDoc->get_parseError(&m_pXMLParseError), "");
                        XST_HRCALL(m_pXMLParseError->get_reason(&lpszStr), "");
                        goto clean;
                }

                XST_HRCALL(m_pXMLDoc->get_xml(&lpszStr), "");

                return RESULT::OK;

                clean:
                        if(lpszStr) SysFreeString(lpszStr);
                        if(&Var) VariantClear(&Var);
                        if(m_pXMLParseError) m_pXMLParseError->Release();
                        if(m_pXMLDoc) { m_pXMLDoc->Release(); m_pXMLDoc = 0; }

                        CoUninitialize();
        
                return RESULT::FAILED;
        }

        int CMSXML::Load(const u8* _pData, ul32 _ulDataSize)
        {
                xst_assert( _pData != 0, "(CMSXML::Load) File data should not be null." );
                if(!_pData || _ulDataSize < 3)
                {
                        //XST::CLastError::Set( "File size is too small." );
						XST_SET_ERR( "The XML file size is too small" );
                        return RESULT::FAILED;
                }

                VARIANT_BOOL Status;
                //VARIANT Var;
                HRESULT hr;
                BSTR lpszStr = 0;

                CoInitialize(0);
                
                m_pXMLDoc = DomFromCOM();
                if(!m_pXMLDoc)
                {
                        goto clean;
                }

                //Check the BOM
                cu8* pFile = _pData;
                //Check utf-8
                if( pFile[0] == 0xEF && pFile[1] == 0xBB && pFile[2] == 0xBF )
                {
                        //Read utf-8
                        pFile += 3;
                        _ulDataSize -= 3;
                }
                else
                {
                        //If utf-16
                        if( pFile[0] == 0xFF && pFile[1] == 0xFE )
                        {
                                //Read utf-16
                                pFile += 2;
                                _ulDataSize -= 2;
                        }
                }

                //Create unicode buffer
                ch16* pBuffer = xst_new ch16[_ulDataSize + 1];
                if(!pBuffer)
                {
                        goto clean;
                }

                XST::StringUtil::AsciiToUnicode(pBuffer, _ulDataSize + 1, (ch8*)pFile, _ulDataSize);
                pBuffer[_ulDataSize] = 0;

                //VariantInit(&Var);
                //V_BSTR(&Var) = SysAllocString(pBuffer);
                //V_VT(&Var) = VT_BSTR;

                XST_HRCALL(m_pXMLDoc->loadXML(pBuffer, &Status), "");
        
                if(Status != VARIANT_TRUE)
                {
                        XST_HRCALL(m_pXMLDoc->get_parseError(&m_pXMLParseError), "");
                        XST_HRCALL(m_pXMLParseError->get_reason(&lpszStr), "");
                        //Core::CLastError::Set( XST::CStringUtil::UnicodeToAscii( lpszStr ) );
						XST_SET_ERR( XST::StringUtil::UnicodeToAscii( lpszStr ) );
                        goto clean;
                }

                XST_HRCALL(m_pXMLDoc->get_xml(&lpszStr), "");

                delete[] pBuffer;
                return RESULT::OK;

                clean:
                        if(lpszStr) SysFreeString(lpszStr);
                        //if(&Var) VariantClear(&Var);
                        if(m_pXMLParseError) m_pXMLParseError->Release();
                        if(m_pXMLDoc) { m_pXMLDoc->Release(); m_pXMLDoc = 0; }

                        CoUninitialize();
                        if(!pBuffer)
                        {
                                return RESULT::FAILED;
                        }
                        else
                        {
                                delete[] pBuffer;
                        }
        
                return RESULT::FAILED;

        }

        i32     CMSXML::Load(const ch16* _pData, cul32& _ulDataSize)
        {
                xst_assert( _pData != 0, "(CMSXML::Load) File data should not be null." );
                if(!_pData || _ulDataSize < 3)
                {
                        XST_SET_ERR( "File size is too small." );
                        return RESULT::FAILED;
                }

                VARIANT_BOOL Status;
                //VARIANT Var;
                HRESULT hr;
                BSTR lpszStr = 0;

                CoInitialize(0);
                
                m_pXMLDoc = DomFromCOM();
                if(!m_pXMLDoc)
                {
                        goto clean;
                }

                BSTR pBuffer = (BSTR)_pData;

                //VariantInit(&Var);
                //V_BSTR(&Var) = SysAllocString(pBuffer);
                //V_VT(&Var) = VT_BSTR;

                XST_HRCALL(m_pXMLDoc->loadXML( pBuffer, &Status), "");
        
                if(Status != VARIANT_TRUE)
                {
                        XST_HRCALL(m_pXMLDoc->get_parseError(&m_pXMLParseError), "");
                        XST_HRCALL(m_pXMLParseError->get_reason(&lpszStr), "");
                        XST_SET_ERR( XST::StringUtil::UnicodeToAscii( lpszStr ) );
                        goto clean;
                }

                XST_HRCALL(m_pXMLDoc->get_xml(&lpszStr), "");

                //delete[] pBuffer;
                return RESULT::OK;

                clean:
                        if(lpszStr) SysFreeString(lpszStr);
                        //if(&Var) VariantClear(&Var);
                        if(m_pXMLParseError) m_pXMLParseError->Release();
                        if(m_pXMLDoc) { m_pXMLDoc->Release(); m_pXMLDoc = 0; }

                        CoUninitialize();
        
                return RESULT::FAILED;

        }


		i32 CMSXML::GetNodeAttribute(CMSXML::NodePtr pNode, BSTR lpszAttributeName, VARIANT* pValueOut)
		{
			NodeMapPtr pAttrMap;
			pNode->get_attributes( &pAttrMap );
			if( pAttrMap == xst_null )
			{
				return RESULT::FAILED;
			}

			NodePtr pAttr;
			pAttrMap->getNamedItem( lpszAttributeName, &pAttr );
			if( pAttr == xst_null )
			{
				return RESULT::FAILED;
			}

			pAttr->get_nodeValue( pValueOut );
	
			return RESULT::OK;
		}

		i32 CMSXML::GetNodeValue(XST::CMSXML::NodePtr pNode, BSTR *lpszValueOut)
		{
			NodePtr pTmpNode;
			pNode->get_firstChild( &pTmpNode );
			if( pTmpNode == xst_null )
			{
				return RESULT::FAILED;
			}

			VARIANT Val;
			pTmpNode->get_nodeValue( &Val );
			*lpszValueOut = Val.bstrVal;

			return RESULT::OK;
		}

}//xst

#endif //XST_WINDOWS
*/