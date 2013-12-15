#ifndef XSE_CTECHNIQUE_H
#define XSE_CTECHNIQUE_H

#include "XSEITechnique.h"

namespace XSE
{
	class XST_API CTechnique : public ITechnique
	{
		friend class CMaterial;
		public:

									CTechnique(xst_castring& strName);
			virtual					~CTechnique();

		protected:

					IPass*			_CreatePass(xst_castring& strName);
					void			_DestroyPass(IPass* pPass);

		protected:

	};

}//xse

#endif