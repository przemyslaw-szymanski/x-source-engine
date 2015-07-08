#include "../include/XSECLuaScript.h"

namespace XSE
{
	namespace Resources
	{
		CLuaScript::CLuaScript(XST::CLuaApi* pApi, XSE_IRESOURCE_DECL_PARAMS) : m_bIsLoaded( false ), m_bIsCompiled( false ), m_pApi( pApi ), XSE_IRESOURCE_CTOR
		{
		}

		CLuaScript::~CLuaScript()
		{
		}

		i32	CLuaScript::Load()
		{
			xst_assert( m_pApi, "Lua api not created" );
			if( this->m_pResourceFile.IsNull() )
			{
				XST_LOG_ERR( "Resource file is null. No script file loaded" );
				return RESULT::FAILED;
			}

			if( m_pApi->LoadBuffer( (lpcastr)this->m_pResourceFile->GetData().GetPointer(), 
                this->m_pResourceFile->GetData().GetSize(), this->GetResourceName().data() ) != 0 )
			{
				XST_LOG_ERR( m_pApi->GetLastError() );
				return RESULT::FAILED;
			}

			m_bIsLoaded = true;

			return RESULT::OK;
		}
						
		i32	CLuaScript::Load(ResFileWeakPtr pFile)
		{
			xst_assert( m_pApi, "Lua api not created" );
			this->m_pResourceFile = pFile;
			if( this->m_pResourceFile.IsNull() )
			{
				XST_LOG_ERR( "Resource file is null" );
				return RESULT::FAILED;
			}
			
			if( m_pApi->LoadBuffer( (lpcastr)pFile->GetData().GetPointer(), pFile->GetData().GetSize(), 
                this->GetResourceName().data() ) != 0 )
			{
				XST_LOG_ERR( m_pApi->GetLastError() );
				return RESULT::FAILED;
			}

			m_bIsLoaded = true;

			return RESULT::OK;
		}
						
		i32	CLuaScript::Load(cu8* pData, cul32& ulDataSize)
		{
			xst_assert( m_pApi, "Lua api not created" );
			if( m_pApi->LoadBuffer( (lpcastr)pData, ulDataSize, this->GetResourceName().data() ) != 0 )
			{
				XST_LOG_ERR( m_pApi->GetLastError() );
				return RESULT::FAILED;
			}

			m_bIsLoaded = true;

			return RESULT::OK;
		}

		i32	CLuaScript::Compile()
		{
			xst_assert( m_pApi, "Lua api not created" );
			if( m_pApi->PCall( 0, LUA_MULTRET, 0 ) != 0 )
			{
				XST_LOG_ERR( m_pApi->GetLastError() );
				return RESULT::FAILED;
			}

			m_bIsCompiled = true;
			return RESULT::OK;
		}

		void CLuaScript::Unload()
		{
			this->m_pResourceFile = xst_null;
			m_bIsLoaded = false;
			m_bIsCompiled = false;
		}

	}//resources
}//xse