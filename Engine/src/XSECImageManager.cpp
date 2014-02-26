#include "XSECImageManager.h"
#include "DevILSystem/XSECDevILSystem.h"

namespace XSE
{
	using namespace Resources;

	CImageManager::CImageManager() :
		m_pImgSystem( xst_null )
	{
		m_pImgSystem = xst_new DevIL::CImageSystem();
	}

	CImageManager::~CImageManager()
	{
		DestroyResources();
		if( m_pImgSystem && m_pImgSystem->IsAutoDestroy() )
		{
			xst_delete( m_pImgSystem );
		}
	}

	void CImageManager::DestroyResources()
	{
		xst_assert( m_pImgSystem, "(CImageManager::DestroyResources) ImageSystem is destroyed or not created" );
		IImage* pImg;
        this->ForEachResource( [&] ( ResourcePtr pRes, GroupWeakPtr pGroup )
        {
            pImg = static_cast< IImage* >( pRes.GetPtr() );
            pImg->DestroyData();
        } );
	}

	i32 CImageManager::Init()
	{
		if( XST_FAILED( m_pImgSystem->Init() ) )
		{
			return XST_FAIL;
		}

		return XST_OK;
	}

	i32 CImageManager::CreateImageData(ImagePtr pImg)
	{
		return m_pImgSystem->CreateImageData( pImg.GetPtr() );
	}

	i32	CImageManager::_CreateMemoryPool(cul32& ulObjCount, XST::IAllocator* pAllocator)
	{
		xst_assert2( m_pImgSystem );

		if( pAllocator )
		{
			xst_delete( this->m_pMemoryMgr );
			this->m_pMemoryMgr = pAllocator;
			if( !this->m_pMemoryMgr->AllocatePool( m_pImgSystem->GetResourceObjectSize(), ulObjCount ) )
			{
				XST_LOG_ERR( "Create memory pool failed in CLuaScriptManager" );
				return RESULT::FAILED;
			}
		}
		else
		{
			xst_delete( this->m_pMemoryMgr );
//			this->m_pMemoryMgr = xst_new XST::TCFreeListMemoryManager< Resources::CLuaScript >( ulObjCount );
			this->m_pMemoryMgr = this->m_pImgSystem->CreateMemoryManager( ulObjCount );
		}

		return RESULT::OK;
	}

	IResource*	CImageManager::_CreateResource(xst_castring& strName, const ResourceHandle& ulHandle, GroupWeakPtr pGroup)
	{
		IResource* pRes = m_pImgSystem->CreateImage( this, ulHandle, strName, this->m_pMemoryMgr );
		return pRes;
	}

	i32 CImageManager::RegisterImageSystem(IImageSystem *pImgSystem, bool bAutoDestroy)
	{
		xst_assert( this->m_pMemoryMgr, "(CImageManager::RegisterImageSystem) The memory manager and it's memory pool are created. You should not change now." );

		if( m_pImgSystem )
		{
			xst_assert( m_pImgSystem->IsAutoDestroy(), "(CImageManager::RegisterImageSystem) Current image system is not auto destroyable. Destroy it before register new. " );
			xst_delete( m_pImgSystem );
		}

		m_pImgSystem = pImgSystem;
		return XST_OK;
	}

	i32	CImageManager::PrepareResource(ResourcePtr pRes)
	{
		if( XST_FAILED( m_pImgSystem->PrepareResource( pRes.GetPtr() ) ) )
		{
			return XST_FAIL;
		}

		//Unload the image if loaded
		IImage* pImg = (IImage*)pRes.GetPtr();
		if( pImg->m_pResourceFile != xst_null )
		{
			i32 iResult = this->m_pFileMgr->DestroyResource( pImg->m_pResourceFile->GetName() );
			pImg->m_pResourceFile = xst_null;
			pImg->m_iResourceState = XST::ResourceStates::PREPARED;
		}

		return XST_OK;
	}

}//xse