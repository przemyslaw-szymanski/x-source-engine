#ifndef XSE_CPASS_H
#define XSE_CPASS_H

#include "XSEIPass.h"

namespace XSE
{
	class XST_API CPass : public IPass
	{
		public:

								CPass(xst_castring& strName);
			virtual				~CPass();

		protected:
	};
}//xse

#endif