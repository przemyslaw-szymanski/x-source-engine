#include "XSECD3D9RenderSystem.h"
#if defined (XST_WINDOWS)

namespace XSE
{
	namespace D3D9
	{
		CRenderSystem::CRenderSystem(xst_castring& strName) : XSE::IRenderSystem( strName )
		{
		}

		CRenderSystem::~CRenderSystem()
		{
			_UnloadLibraries();
		}

		void CRenderSystem::_UnloadLibraries()
		{
			for(u32 i = 0; i < LIB_COUNT; ++i)
			{
				XST::Platform::CloseLibrary( m_ahDlls[ i ] );
			}
		}

		i32 CRenderSystem::_LoadLibraries()
		{
			XST::SPlatformInfo PlatformInfo;
			XST::Platform::GetPlatformInfo( &PlatformInfo );

			#if defined (XST_DEBUG)

				if( PlatformInfo.uiSystemType == XST::SystemType::WINDOWS_XP )
				{
					m_ahDlls[ D3D9 ] = XST::Platform::LoadLibrary( "d3d9.dll" );
				}
				else
				{
					m_ahDlls[ D3D9 ] = XST::Platform::LoadLibrary( "d3d9d.dll" );
				}

				m_ahDlls[ D3DX9 ] = XST::Platform::LoadLibrary( "D3DX9d_43.dll" );

			#else
				m_ahDlls[ D3D9 ] = XST::Platform::LoadLibrary( "d3d19.dll" );
				m_ahDlls[ D3DX9 ] = XST::Platform::LoadLibrary( "D3DX9d_43.dll" );
			#endif

			for(u32 i = 0; i < LIB_COUNT; ++i)
			{
				if( !m_ahDlls[ i ] )
				{
					XST_LOG_ERR( "Unable to load one or more Direct3D9 dlls" );
					return RESULT::FAILED;
				}
			}

			return RESULT::OK;
		}

		i32 CRenderSystem::Init(const SRenderSystemOptions& Options)
		{
			
			HRESULT hr = S_OK;
			RECT rc;
			HWND hWnd = (HWND)Options.uiHWND;
			
			GetClientRect( hWnd, &rc );
			u32 uiWidth = rc.right - rc.left;
			u32 uiHeight = rc.bottom - rc.top;

			u32 uiDeviceFlags = 0;

			#if defined (XST_DEBUG)

				//uiDeviceFlags |= D3D9_CREATE_DEVICE_DEBUG;

			#else

			#endif

			

			return RESULT::OK;
		}

		i32 CRenderSystem::BeginRender()
		{
			return RESULT::OK;
		}

		i32 CRenderSystem::EndRender()
		{
			return RESULT::OK;
		}

		i32 CRenderSystem::ClearScene(const XST::CColor& Color)
		{
			return RESULT::OK;
		}

	}//d3d9
}//xse

#endif //XST_WINDOWS