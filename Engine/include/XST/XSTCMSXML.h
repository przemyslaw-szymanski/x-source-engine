#ifndef XST_CMSXML_H
#define XST_CMSXML_H

#include "XSTCommon.h"
/*
#if defined (XST_WINDOWS) && defined (XST_VISUAL_STUDIO)


#import <msxml3.dll> raw_interfaces_only
#include <msxml6.h>
//#pragma comment(lib, "msxml6.lib")

namespace XST
{
	using namespace MSXML2;
    class CMSXML
    {
			public:

				typedef MSXML2::IXMLDOMNode*			NodePtr;
				typedef MSXML2::IXMLDOMNodeList*		NodeListPtr;
				typedef MSXML2::IXMLDOMNamedNodeMap*	NodeMapPtr;

            public:

                                        CMSXML();
                                        ~CMSXML();

                    int                 Load(lpcwstr _lpszFilename);
                    int                 Load(lpcastr _lpszFilename);

                    int                 Load(const u8* _pData, ul32 _ulDataSize);
                    i32                 Load(const ch16* _pFileData, cul32& _ulDataSize);

					MSXML2::IXMLDOMDocument*	GetXMLDocument() const
												{ return m_pXMLDoc; }

					i32					GetNodeAttribute(NodePtr pNode, BSTR lpszAttributeName, VARIANT* pValueOut);
					
					i32					GetNodeValue(NodePtr pNode, BSTR* lpszValueOut);	

            private:

                    MSXML2::IXMLDOMDocument*	DomFromCOM();

            private:

                    MSXML2::IXMLDOMDocument*        m_pXMLDoc;
                    MSXML2::IXMLDOMParseError*      m_pXMLParseError;
    };

}//xst

#else //XST_WINDOWS

namespace XST
{
	class CMSXML
	{
		public:
										CMSXML() {}
										~CMSXML() {}

					int                 Load(lpcwstr _lpszFilename) { return 0; }
					int                 Load(lpcastr _lpszFilename) { return 0; }

					int                 Load(const u8* _pData, ul32 _ulDataSize) { return 0; }
					i32                 Load(const ch16* _pFileData, cul32& _ulDataSize) { return 0; }

					void*				GetXMLDocument() const
										{ return 0; }

            private:

					void*				DomFromCOM() { return 0; }
	};
}//xst

#endif
*/
#endif //XST_CMSXML_H
