#ifndef XST_TEXT_FILE_WRITER_H
#define XST_TEXT_FILE_WRITER_H

#include "XSTIFile.h"

namespace XST
{

	class XST_API CTextFileWriter : public IFile
	{

		public:

						CTextFileWriter(xst_castring& _strFileName);

						CTextFileWriter();

						~CTextFileWriter();

				i32		Open(xst_castring& strFileName);

				i32		Open(xst_cwstring& strFileName);

				i32		OpenAppend(xst_castring& strFileName);

				i32		OpenAppend(xst_cwstring& strFileName);

			 void		Close();

				i32		WriteLine(xst_castring& strText);
				i32		WriteLine(xst_cwstring& strText);
	
				i32		Write(xst_castring& strText);
				i32		Write(xst_cwstring& strText);

				i32		Write(lpcastr _lpszFormat, ...);

				i32		Flush();

		private:

			FileHandle		m_pHFile;

	};

}//XST

#endif
