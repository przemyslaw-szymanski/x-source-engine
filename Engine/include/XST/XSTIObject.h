
#ifndef XST_IOBJECT_H
#define XST_IOBJECT_H


#include "XSTCommon.h"
#include "XSTCStringUtil.h"
#include "XSTCConsole.h"
#include "XSTPlatform.h"
#include "XSTCLogger.h"

namespace XST
{

	class XST_API IObject
	{
		public:

								IObject() : m_lRefCounter(1), m_bIsNull(false) {}
		virtual					~IObject() 
								{ 
									//xst_assert(m_lRefCounter <= 0, "Reference counter must be 0. Object not deleted!"); 
									#if defined ( XST_OBJ_DEBUG )
										if( m_lRefCounter > 0 )
										{
											//XST_LOG_WRN( "Object: " << this->m_strDbgName << " reference counter is: " << m_lRefCounter << ". This object could not be destroyed" );
										}
									#endif //XST_OBJ_DEBUG
										m_lRefCounter = 0;
										m_bIsNull = true;
								}

				void			Release()
								{
									//IObject* p = this;
									if( RemoveRef() == 0 )
									{
										delete this;
									}
								}

		xst_fi	l32			    AddRef()
								{ return ++m_lRefCounter; }

		xst_fi	l32			    RemoveRef()
								{ return --m_lRefCounter; }

		xst_fi	l32				GetRefCount() const
								{ return m_lRefCounter; }

		xst_fi	void			_SetDbgName(lpcastr _strDbgName)
								{
									#if defined( XST_OBJ_DEBUG )
										m_strDbgName = _strDbgName;
									#endif //XST_ENGINE_DEBUG
								}

		xst_fi	lpcastr			_GetDbgName() const
								{
									#if defined( XST_OBJ_DEBUG )
												return m_strDbgName.c_str();
									#else	
												return XST::StringUtil::EmptyAString;
									#endif //XST_ENGINE_DEBUG
								}


		private:

			l32		    m_lRefCounter;
			bool		m_bIsNull;
		#if defined(XST_OBJ_DEBUG)
			xst_astring	m_strDbgName;
		#endif //XST_OBJ_DEBUG
	};

#ifndef xst_release
#	define xst_release( _pPtr ) if( (_pPtr) ) (_pPtr)->Release(); (_pPtr) = xst_null
#endif

#if defined( XST_OBJ_DEBUG )
#	define XST_SET_DBG_NAME(_pObj, _strName) (_pObj)->_SetDbgName( _strName )
#	define XST_GET_DBG_NAME(_pObj) (_pObj)->_GetDbgName()
#else
#	define XST_SET_DBG_NAME(_pObj, _strName)
#	define XST_GET_DBG_NAME(_pObj) XST::StringUtil::EmptyAString
#endif //XST_ENGINE_DEBUG

}//XST

#endif //XST_IOBJECT_H
