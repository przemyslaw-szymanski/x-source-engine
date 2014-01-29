#ifndef ISAMPLE_H
#define ISAMPLE_H

#include <XSourceEngine.h>

using namespace XST::Types;

#if defined(NO_EXPORT_SAMPLE)
#	define SAMPLE_API
#elif defined(EXPORT_SAMPLE)
#	define SAMPLE_API __declspec(dllexport)
#else
#	define SAMPLE_API __declspec(dllimport)
#endif

class SAMPLE_API ISample : public XSE::IKeyboardListener, public XSE::IMouseListener
{
	public:

									ISample() : m_pSceneMgr( xst_null ), m_pRenderWindow( xst_null ) {}
		virtual						~ISample() {}

		virtual const char*			GetName() const = 0;

		virtual const char*			GetGroupName() const { return "Default"; }

		virtual i32					Init(XSE::CEngine* pEngine, XSE::IRenderWindow* pWnd) = 0;

		virtual i32					Run() = 0;

		virtual void				Update() 
		{
			MoveCamera();
			OnUpdate();
		}

		virtual void				Destroy() = 0;

		virtual bool				AutoDestroy() { return true; }

		virtual	void				OnKeyPressed(const XSE::SKeyboardEvent& Event) { return; }

		virtual	void				OnKeyDown(const XSE::SKeyboardEvent& Event) { return; }

		virtual	void				OnKeyUp(const XSE::SKeyboardEvent& Event) { return; }

		virtual bool				IsKeyboardListenerAutoDestroy() { return true; }

		virtual bool				IsMouseListenerAutoDestroy() { return true; }

		virtual void				SetSceneManager(XSE::CSceneManager* pSceneMgr)
									{ m_pSceneMgr = pSceneMgr; }

		virtual void				OnUpdate() { return; }

		virtual void				OnMouseMove(const XSE::SMouseEvent& Event)
		{
			if( m_pSceneMgr != xst_null )
			{
				//m_pSceneMgr->GetCurrentCamera()->SetMouseDelta( Event.Delta );//SetProperty( XSE::FPPCameraProperties::MOUSE_DELTA, (xst_unknown)&Event.Delta );
				m_pMoveCam->RotateX( (f32)-Event.Delta.x );
				m_pMoveCam->RotateY( (f32)-Event.Delta.y );
			}
		}

		virtual XSE::CSceneManager*	CreateSceneManager(XSE::CEngine* pEngine, f32 fSceneSize = 2000)
		{
			if( m_pRenderWindow == xst_null )
			{
				m_pRenderWindow = pEngine->GetRenderWindow( 0 );
			}

			m_pSceneMgr = pEngine->CreateSceneManager( this->GetName(), fSceneSize );
			m_pViewCam = m_pSceneMgr->CreateCamera( xst_astring( this->GetName() ) + "/view" );
			m_pDbgCam = m_pSceneMgr->CreateCamera( xst_astring( this->GetName() ) + "/dbg" );
			m_pDbgCam->SetFOV( XST::Math::DegreesToRadians( 45 ), 0.1f, 1000.0f );
			m_pDbgCam->SetPosition( 0.0f, 0.0f, 0.0f );
			m_pDbgCam->SetLookAt( XSE::Vec3( 0, 0, 0 ) );
			
			m_pSceneMgr->SetCamera( m_pViewCam, true, true );

			if( m_bDbgCamEnabled )
			{
				m_pSceneMgr->SetCamera( m_pViewCam, false, true );
				m_pSceneMgr->SetCamera( m_pDbgCam, true, false );
			}

			m_pViewCam->SetFOV( XST::Math::DegreesToRadians( 45 ), 0.1f, 1000.0f );
			m_pViewCam->SetPosition( 0.0f, 0.0f, 0.0f );
			m_pViewCam->SetLookAt( XSE::Vec3( 0, 0, 0 ) );

			m_pMoveCam = m_pViewCam;
			m_pMoveCam->SetSpeed( m_fCamMoveSpeed, m_fCamRotateSpeed, m_fCamRotateSpeed );

			SetSceneManager( m_pSceneMgr );
			pEngine->SetSceneManager( m_pSceneMgr );
			return m_pSceneMgr;
		}

		virtual void				EnableDbgCamera(bool bEnable)
		{
			assert( m_pSceneMgr );
			m_bDbgCamEnabled = bEnable;
			if( bEnable )
			{
				m_pSceneMgr->SetCamera( m_pViewCam, false, true );
				m_pSceneMgr->SetCamera( m_pDbgCam, true, false );
			}
			else
			{
				m_pSceneMgr->SetCamera( m_pViewCam, true, true );
				m_pSceneMgr->SetCamera( m_pDbgCam, false, false );
			}
		}

		virtual void				OnKeyPressEvent(int iKey) { return; }

		virtual void				MoveCamera()
		{
			if( m_pRenderWindow == xst_null )
				return;

			XSE::IKeyboard* pKeyboard = m_pRenderWindow->GetKeyboard();
			const XSE::Vec3& vecDir = m_pMoveCam->GetDirection();
			const XSE::Vec3& vecRight = m_pMoveCam->GetRight();
			const XSE::Vec3& vecUp = m_pMoveCam->GetUp();

			if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_W ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( 0, 0, 1 );
				m_pMoveCam->Move( vecDir );
			}

			if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_S ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( 0, 0, -1 );
				m_pMoveCam->Move( -vecDir );
			}

			if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_A ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( XSE::Vec3( -1, 0, 0 ) );
				m_pMoveCam->Move( -( vecRight ) * XSE::Vec3( 1, 0, 1 ) );
			}

			if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_D ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( XSE::Vec3( 1, 0, 0 ) );
				m_pMoveCam->Move( ( vecRight * XSE::Vec3( 1, 0, 1 ) ) );
			}

			if( pKeyboard->IsKeyPressed(  XSE::KeyCodes::CAPITAL_Q ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( XSE::Vec3( 0, -1, 0 ) );
				m_pMoveCam->Move( -vecUp );
			}

			if( pKeyboard->IsKeyPressed( XSE::KeyCodes::CAPITAL_E ) )
			{
				//m_pSceneMgr->GetCurrentCamera()->Move( XSE::Vec3( 0, 1, 0 ) );
				m_pMoveCam->Move( vecUp );
			}
		}

	protected:

		XSE::CSceneManager*		m_pSceneMgr = xst_null;
		XSE::IRenderWindow*		m_pRenderWindow = xst_null;
		XSE::CCamera*			m_pViewCam = xst_null;
		XSE::CCamera*			m_pDbgCam = xst_null;
		XSE::CCamera*			m_pMoveCam = xst_null;
		float					m_fCamRotateSpeed = 0.01f;
		float					m_fCamMoveSpeed = 0.00001f;
		bool					m_bDbgCamEnabled = false;
};

#define SAMPLE_EXPORT_FUNC(_ClassName) \
	static SAMPLE_API ISample* XST_ADD( Create, _ClassName )() { return xst_new _ClassName(); } \
	static SAMPLE_API void XST_ADD( Destroy, _ClassName )(ISample* pSample) { _ClassName* pS = (_ClassName*)pSample; delete pS; }

#endif //ISAMPLE_H