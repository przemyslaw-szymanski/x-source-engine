#ifndef XSE_CLUA_MATERIAL_PARSER_H
#define XSE_CLUA_MATERIAL_PARSER_H

#include "../include/XSECommon.h"

namespace XSE
{
	namespace Resources
	{
		class CMaterial;
	}

	class CLuaMaterialParser
	{
		public:

									CLuaMaterialParser();
			virtual					~CLuaMaterialParser();

					i32				Parse(Resources::CMaterial* pMat, XST::CLuaTable* pTable);

		protected:

					void			_SetColor(XST::Math::CVector3* pvecOut, XST::CLuaTable* pTable);
					void			_AddTexture(Resources::CMaterial* pMat, XST::CLuaTable* pTable);
	};

}//xse

#endif