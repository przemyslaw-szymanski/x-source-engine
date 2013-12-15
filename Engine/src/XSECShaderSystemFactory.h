#ifndef XSE_CSHADER_SYSTEM_FACTORY_H
#define XSE_CSHADER_SYSTEM_FACTORY_H

#include "XSECommon.h"

namespace XSE
{
	class CShaderSystemFactory
	{
		friend class CEngine;

		typedef xst_map< ul32, IShaderSystem* >	SystemMap;

		public:

			static 			i32				RegisterSystem(ul32 ulShaderSystemFlag);
			static xst_fi	i32				RegisterSystem(xst_castring& strName)
											{ return RegisterSystem( XST::CHash::GetCRC( strName ) ); }

			static			void			DestroySystems();

			static 			IShaderSystem*	GetShaderSystem(ul32 ulSystemId);
			static xst_fi	IShaderSystem*	GetShaderSystem(xst_castring& strName)
											{ return GetShaderSystem( XST::CHash::GetCRC( strName ) ); }

		protected:

			xst_fi	void		_SetRenderSystem(IRenderSystem* pRS)
								{ m_pRS = pRS; }

		protected:

			static IRenderSystem*	m_pRS;
			static SystemMap		m_mSystems;
	};

}//xse

#endif