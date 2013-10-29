#ifndef XST_CPROFILER_LOGGER_H
#define XST_CPROFILER_LOGGER_H

#include "XSTCommon.h"

namespace XST
{
	class CProfilerLogger
	{
		public:

							CProfilerLogger(xst_castring& _strFileName)
							{
								//fopen_s(&File, _strFileName.c_str(), "w+");
								xst_fopen( &pFile, _strFileName.data(), "w+" );
							}

							~CProfilerLogger()
							{
								fclose(pFile);
							}

			static void		Log(xst_castring& _strText, xst_castring& _strFile, u32 _uiLine)
			{
				xst_assert(pFile != 0, "File not opened");
				fprintf(pFile, "[%s][%d]: %s\n", _strFile.c_str(), _uiLine, _strText.c_str() );
				fflush(pFile);
			}

			static void		Log(xst_castring& _strText)
			{
				xst_assert(pFile != 0, "File not opened");
				fprintf(pFile, "%s\n", _strText.c_str() );
				fflush(pFile);
			}


			static FILE*	pFile;
	};

}//XST

#define XST_DEFINE_PROFILER_LOGGER() FILE* XST::CProfilerLogger::File = 0

#endif
