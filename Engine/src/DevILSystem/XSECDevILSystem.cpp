#include "XSECDevILSystem.h"
#include "XSECDevILImage.h"

namespace XSE
{
	namespace DevIL
	{
		#define XSE_DEVIL_TD(_retType, _name)	typedef _retType ( ILAPIENTRY * XST_ADD( pfn, _name) )
		#define XSE_DEVIL_FD(_retType, _name)	XST_ADD( pfn, _name )	_name = xst_null

		//typedef void (ILAPIENTRY * ilInit)(void);
		XSE_DEVIL_TD( void, ilInit )(void);
		XSE_DEVIL_FD( void, ilInit );
		XSE_DEVIL_TD( ILuint, ilGenImage )(void);
		XSE_DEVIL_FD( ILuint, ilGenImage);
		XSE_DEVIL_TD( void, ilDeleteImages )(ILsizei, const ILuint*);
		XSE_DEVIL_FD( void, ilDeleteImages);
		XSE_DEVIL_TD( ILint, ilGetInteger )(ILenum);
		XSE_DEVIL_FD( ILint, ilGetInteger);
		XSE_DEVIL_TD( ILubyte*, ilGetData )(void);
		XSE_DEVIL_FD( ILubyte*, ilGetData);
		XSE_DEVIL_TD( void, ilBindImage )(ILuint);
		XSE_DEVIL_FD( void, ilBindImage);
		XSE_DEVIL_TD( ILboolean, ilEnable )(ILenum);
		XSE_DEVIL_FD( ILboolean, ilEnable);
		XSE_DEVIL_TD( ILenum, ilGetError )(void);
		XSE_DEVIL_FD( ILenum, ilGetError);
		XSE_DEVIL_TD( ILboolean, ilLoadL )(ILenum, const void*, ILuint);
		XSE_DEVIL_FD( ILboolean, ilLoadL );
		XSE_DEVIL_TD( ILuint, ilSaveL )(ILenum, void*, ILuint);
		XSE_DEVIL_FD( ILuint, ilSaveL );
		XSE_DEVIL_TD( ILboolean, ilSaveImage )(const char*);
		XSE_DEVIL_FD( ILboolean, ilSaveImage );
		XSE_DEVIL_TD( ILboolean, ilTexImage)(ILuint Width, ILuint Height, ILuint Depth, ILubyte NumChannels, ILenum Format, ILenum Type, void *Data);
		XSE_DEVIL_FD( ILboolean, ilTexImage );
		XSE_DEVIL_TD( ILuint, ilCloneCurImage )();
		XSE_DEVIL_FD( ILuint, ilCloneCurImage );
		XSE_DEVIL_TD( ILboolean, iluScale )(ILuint, ILuint, ILuint);
		XSE_DEVIL_FD( ILboolean, iluScale );
		XSE_DEVIL_TD( void, iluImageParameter )(ILenum, ILenum);
		XSE_DEVIL_FD( void, iluImageParameter );

		CImageSystem::CImageSystem() :
			m_pMemMgr( xst_null )
		{
			m_aiFormats[ ImageFormats::BGR ]		= IL_BGR;
			m_aiFormats[ ImageFormats::BGRA ]		= IL_BGRA;
			m_aiFormats[ ImageFormats::LUMINANCE ]	= IL_LUMINANCE;
			m_aiFormats[ ImageFormats::RGB ]		= IL_RGB;
			m_aiFormats[ ImageFormats::RGBA ]		= IL_RGBA;
			m_aiFormats[ ImageFormats::UNKNOWN ]	= IL_RGB;

			m_aiFormatSizes[ ImageFormats::BGR ]		= sizeof( u8 ) * 3;
			m_aiFormatSizes[ ImageFormats::BGRA ]		= sizeof( u8 ) * 4;
			m_aiFormatSizes[ ImageFormats::LUMINANCE ]	= sizeof( u8 );
			m_aiFormatSizes[ ImageFormats::RGB ]		= sizeof( u8 ) * 3;
			m_aiFormatSizes[ ImageFormats::RGBA ]		= sizeof( u8 ) * 4;
			m_aiFormatSizes[ ImageFormats::UNKNOWN ]	= sizeof( u8 ) * 3;

			m_aiChannelCounts[ ImageFormats::BGR ]			= 3;
			m_aiChannelCounts[ ImageFormats::BGRA ]			= 4;
			m_aiChannelCounts[ ImageFormats::LUMINANCE ]	= 1;
			m_aiChannelCounts[ ImageFormats::RGB ]			= 3;
			m_aiChannelCounts[ ImageFormats::RGBA ]			= 4;
			m_aiChannelCounts[ ImageFormats::UNKNOWN ]		= 3;

			m_aiDataTypes[ ImageDataTypes::FLOAT_32 ]	= IL_FLOAT;
			m_aiDataTypes[ ImageDataTypes::FLOAT_64 ]	= IL_DOUBLE;
			m_aiDataTypes[ ImageDataTypes::INT_16 ]		= IL_SHORT;
			m_aiDataTypes[ ImageDataTypes::INT_32 ]		= IL_INT;
			m_aiDataTypes[ ImageDataTypes::INT_8 ]		= IL_BYTE;
			m_aiDataTypes[ ImageDataTypes::UINT_16 ]	= IL_UNSIGNED_SHORT;
			m_aiDataTypes[ ImageDataTypes::UINT_32 ]	= IL_UNSIGNED_INT;
			m_aiDataTypes[ ImageDataTypes::UINT_8 ]		= IL_UNSIGNED_BYTE;

			m_aiDataSizes[ ImageDataTypes::FLOAT_32 ]	= sizeof( f32 );
			m_aiDataSizes[ ImageDataTypes::FLOAT_64 ]	= sizeof( f64 );
			m_aiDataSizes[ ImageDataTypes::INT_16 ]		= sizeof( i16 );
			m_aiDataSizes[ ImageDataTypes::INT_32 ]		= sizeof( i32 );
			m_aiDataSizes[ ImageDataTypes::INT_8 ]		= sizeof( i8 );
			m_aiDataSizes[ ImageDataTypes::UINT_16 ]	= sizeof( u16 );
			m_aiDataSizes[ ImageDataTypes::UINT_32 ]	= sizeof( u32 );
			m_aiDataSizes[ ImageDataTypes::UINT_8 ]		= sizeof( u8 );

			//Create hashes for faster image type recognizeing
			m_aulExtHashes[ ImageTypes::BMP ] = XST::CHash::GetCRC( "bmp" );
			m_aulExtHashes[ ImageTypes::DDS ] = XST::CHash::GetCRC( "dds" );
			m_aulExtHashes[ ImageTypes::HDR ] = XST::CHash::GetCRC( "hdr" );
			m_aulExtHashes[ ImageTypes::ICO ] = XST::CHash::GetCRC( "ico" );
			m_aulExtHashes[ ImageTypes::JPEG ] = XST::CHash::GetCRC( "jpeg" );
			m_aulExtHashes[ ImageTypes::JPG ] = XST::CHash::GetCRC( "jpg" );
			m_aulExtHashes[ ImageTypes::PNG ] = XST::CHash::GetCRC( "png" );
			m_aulExtHashes[ ImageTypes::RAW ] = XST::CHash::GetCRC( "raw" );
			m_aulExtHashes[ ImageTypes::TGA ] = XST::CHash::GetCRC( "tga" );
			m_aulExtHashes[ ImageTypes::TIFF ] = XST::CHash::GetCRC( "tiff" );
		}

		CImageSystem::~CImageSystem()
		{
			//DestroyMemoryManager( m_pMemMgr );
			_UnloadLibraries();
		}

		i32 CImageSystem::_LoadLibraries()
		{
			m_hDll = XST::Platform::LoadLibrary( "DevIL.dll" );
			if( m_hDll == xst_null )
			{
				XST_LOG_ERR( "Failed to load dll: DevIL.dll" );
				return XST_FAIL;
			}

			m_hILUDll = XST::Platform::LoadLibrary( "ILU.dll" );
			if( m_hILUDll == xst_null )
			{
				XST_LOG_ERR( "Failed to load dll: ILU.dll" );
				return XST_FAIL;
			}

			XSE_LOAD_FUNC3( ilInit, m_hDll );
			XSE_LOAD_FUNC3( ilGenImage, m_hDll );
			XSE_LOAD_FUNC3( ilDeleteImages, m_hDll );
			XSE_LOAD_FUNC3( ilGetInteger, m_hDll );
			XSE_LOAD_FUNC3( ilGetData, m_hDll );
			XSE_LOAD_FUNC3( ilBindImage, m_hDll );
			XSE_LOAD_FUNC3( ilEnable, m_hDll );
			XSE_LOAD_FUNC3( ilGetError, m_hDll );
			XSE_LOAD_FUNC3( ilLoadL, m_hDll );
			XSE_LOAD_FUNC3( ilSaveL, m_hDll );
			XSE_LOAD_FUNC3( ilSaveImage, m_hDll );
			XSE_LOAD_FUNC3( ilTexImage, m_hDll );
			XSE_LOAD_FUNC3( ilCloneCurImage, m_hDll );
			XSE_LOAD_FUNC3( iluScale, m_hILUDll );
			XSE_LOAD_FUNC3( iluImageParameter, m_hILUDll );

			return XST_OK;
		}

		void CImageSystem::_UnloadLibraries()
		{
			XST::Platform::CloseLibrary( m_hILUDll );
			XST::Platform::CloseLibrary( m_hDll );
		}

		i32 CImageSystem::Init()
		{
			if( XST_FAILED( _LoadLibraries() ) )
			{
				return XST_FAIL;
			}

			//Initialize devil
			ilInit();

			return XST_OK;
		}

		void CImageSystem::ScaleImage(Resources::IImage* pImage, cu32& uiWidth, cu32& uiHeight)
		{
			CImage* pImg = (CImage*)pImage;
			ilBindImage( pImg->m_uiImgId );
			iluImageParameter( ILU_FILTER, ILU_BILINEAR );
			iluScale( uiWidth, uiHeight, 0 );
			pImg->m_uiWidth = uiWidth;
			pImg->m_uiHeight = uiHeight;
			//pImg->m_bDirty = true;
			if( pImg->m_bManual )
			{
				xst_deletea( pImg->m_pData );
				pImg->m_bManual = false;
			}

			pImg->m_pData = ilGetData();
			pImg->m_uiPixelCount = uiWidth * uiHeight;
			pImg->m_ulDataSize = ilGetInteger( IL_IMAGE_SIZE_OF_DATA );
		}

		/*XST::IAllocator* CImageSystem::CreateMemoryManager(ul32 ulObjCount)
		{
			return xst_new XST::TCFreeListMemoryManager< CImage >( ulObjCount );
		}*/
						
		/*void CImageSystem::DestroyMemoryManager(XST::IAllocator* pAllocator)
		{
			XST::TCFreeListMemoryManager< CImage >* pMgr = (XST::TCFreeListMemoryManager< CImage >*)pAllocator;
			xst_delete( pMgr );
		}*/

		ImagePtr CImageSystem::CloneImage(const XSE::Resources::IImage *pImg)
		{
			CImage* pTmpImg = (CImage*)pImg;
			xst_stringstream ssName;
			ssName << XST::CTime::GetQPerfTickCount() << "_clone";

			CImage* pImage = xst_new CImage(	this, pTmpImg->m_pResourceCreator, 0, ssName.str(), 
												pTmpImg->m_iResourceType, pTmpImg->m_iResourceState, xst_null );

			ilBindImage( pTmpImg->m_uiImgId );

			//pImage->m_bDirty = false;
			pImage->m_bManual = false;
			pImage->SetDataType( pTmpImg->m_eDataType);
			pImage->SetFormat( pTmpImg->m_eFormat );
			pImage->m_uiBPP = pTmpImg->m_uiBPP;
			pImage->m_uiBytesPerChannel = pTmpImg->m_uiBytesPerChannel;
			pImage->m_uiBytesPerPixel = pTmpImg->m_uiBytesPerPixel;
			pImage->m_uiChannelCount = pTmpImg->m_uiChannelCount;
			pImage->m_uiFormat = pTmpImg->m_uiFormat;
			pImage->m_uiWidth = pTmpImg->m_uiWidth;
			pImage->m_uiHeight = pTmpImg->m_uiHeight;
			pImage->m_uiImgId = ilCloneCurImage();
			pImage->m_uiPixelCount = pTmpImg->m_uiPixelCount;
			pImage->m_uiType = pTmpImg->m_uiType;
			pImage->m_ulDataSize = pTmpImg->m_ulDataSize;
			pImage->m_pImageSystem = pTmpImg->m_pImageSystem;
			ilBindImage( pImage->m_uiImgId );
			pImage->m_pData = ilGetData();

			return ImagePtr( pImage );
		}
		
		Resources::IResource*	CImageSystem::CreateResource(IResourceManager* pCreator, ul32 ulResHandle, xst_castring& strResName, XST::IAllocator* pAllocator)
		{
			CImage* pImg = xst_new CImage( this, pCreator, ulResHandle, strResName, ResourceTypes::IMAGE, ResourceStates::CREATED, pAllocator );
			if( pImg == xst_null )
			{
				XST_LOG_ERR( "Could not create an image: " << strResName << ". Memory error." );
				return xst_null;
			}

			return pImg;
		}
		
		void CImageSystem::DestroyResource(Resources::IResource* pResource)
		{
			xst_assert2( pResource != xst_null );
			CImage* pImage = (CImage*)pResource;

			if( pImage->m_bManual )
			{
				xst_deletea( pImage->m_pData );
			}

			ilDeleteImages( 1, &pImage->m_uiImgId );
			
			pImage->m_uiImgId			= 0;
			pImage->m_uiWidth			= 0;
			pImage->m_uiHeight			= 0;
			pImage->m_uiBPP				= 0;
			pImage->m_uiBytesPerPixel	= 0;
			pImage->m_pData				= xst_null;
			pImage->m_ulDataSize		= 0;
			pImage->m_uiFormat			= 0;
			pImage->m_uiChannelCount	= 0;
			pImage->m_uiPixelCount		= 0;
			pImage->m_uiBytesPerChannel = 0;
			pImage->m_eFormat			= ImageFormats::UNKNOWN;
			pImage->m_uiType			= 0;
			pImage->m_bManual			= false;
		}
		
		u32	CImageSystem::GetResourceObjectSize() const
		{
			return sizeof( CImage );
		}

		i32	CImageSystem::PrepareResource(Resources::IResource* pResource)
		{
			xst_assert2( pResource != xst_null );
			CImage* pImage = (CImage*)pResource;

			//If image was not generated
			if( pImage->m_uiImgId == 0 )
			{
				pImage->m_uiImgId = ilGenImage();
			}

			//Bind the image
			ilBindImage( pImage->m_uiImgId );
			if( pImage->m_pResourceFile != xst_null )
			{
				XST::TCData< u8 >& Data = pImage->m_pResourceFile->GetData();
				if( !ilLoadL( (ILenum)_GetImageType( XST::IFile::GetFileExtension( pImage->GetResourceName() ) ), Data.GetData(), Data.GetSize() ) )
				{
					return XST_FAIL;
				}

				pImage->m_uiWidth			= ilGetInteger( IL_IMAGE_WIDTH );
				pImage->m_uiHeight			= ilGetInteger( IL_IMAGE_HEIGHT );
				pImage->m_uiBPP				= ilGetInteger( IL_IMAGE_BITS_PER_PIXEL );
				pImage->m_uiBytesPerPixel	= ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
				pImage->m_pData				= ilGetData();
				pImage->m_ulDataSize		= ilGetInteger( IL_IMAGE_SIZE_OF_DATA );
				pImage->m_uiFormat			= ilGetInteger( IL_IMAGE_FORMAT );
				pImage->m_uiType			= ilGetInteger( IL_IMAGE_TYPE );
				pImage->m_uiChannelCount	= ilGetInteger( IL_IMAGE_CHANNELS );
				pImage->m_uiPixelCount		= pImage->m_uiWidth * pImage->m_uiHeight;
				pImage->m_uiBytesPerChannel = pImage->m_uiBytesPerPixel / pImage->m_uiChannelCount;
				pImage->m_bManual			= false;

				//Check the format
				switch( pImage->m_uiFormat )
				{
					case IL_RGB: pImage->m_eFormat = ImageFormats::RGB; pImage->_SetColor = &CImage::_SetColorRGB; break;
					case IL_RGBA: pImage->m_eFormat = ImageFormats::RGBA; pImage->_SetColor = &CImage::_SetColorRGBA; break;
					case IL_BGR: pImage->m_eFormat = ImageFormats::BGR; pImage->_SetColor = &CImage::_SetColorBGR; break;
					case IL_BGRA: pImage->m_eFormat = ImageFormats::BGRA; pImage->_SetColor = &CImage::_SetColorBGRA; break;
					case IL_LUMINANCE: pImage->m_eFormat = ImageFormats::LUMINANCE; pImage->_SetColor = &CImage::_SetColorR; break;
					default: pImage->m_eFormat = ImageFormats::UNKNOWN; pImage->_SetColor = &CImage::_SetColorRGB; break;
				}

				switch( pImage->m_uiType )
				{
					case IL_UNSIGNED_BYTE: pImage->m_eDataType = ImageDataTypes::UINT_8; break;
					case IL_BYTE: pImage->m_eDataType = ImageDataTypes::INT_8; break;
					case IL_FLOAT: pImage->m_eDataType = ImageDataTypes::FLOAT_32; break;
					case IL_DOUBLE: pImage->m_eDataType = ImageDataTypes::FLOAT_64; break;
					case IL_SHORT: pImage->m_eDataType = ImageDataTypes::INT_16; break;
					case IL_INT: pImage->m_eDataType = ImageDataTypes::INT_32; break;
					case IL_UNSIGNED_SHORT: pImage->m_eDataType = ImageDataTypes::UINT_16; break;
					case IL_UNSIGNED_INT: pImage->m_eDataType = ImageDataTypes::UINT_32; break;
					default: pImage->m_eDataType = ImageDataTypes::UINT_8; break;
				}
			}
			else
			{
				//If this image is new set default values
				pImage->m_uiWidth			= 0;
				pImage->m_uiHeight			= 0;
				pImage->m_uiBPP				= 0;
				pImage->m_uiBytesPerPixel	= 0;
				pImage->m_pData				= xst_null;
				pImage->m_ulDataSize		= 0;
				pImage->m_uiFormat			= 0;
				pImage->m_uiChannelCount	= 0;
				pImage->m_uiPixelCount		= 0;
				pImage->m_uiBytesPerChannel = 0;
				pImage->m_eFormat			= ImageFormats::UNKNOWN;
				pImage->m_eDataType			= ImageDataTypes::UINT_8;
				pImage->m_bManual			= true;
			}
			
			return XST_OK;
		}

		i32 CImageSystem::CreateImageData(Resources::IImage* pImg)
		{
			CImage* pImage = (CImage*)pImg;

			if( pImage->m_pData != xst_null )
			{
				XST_LOG_ERR( "Image: " << pImage->GetResourceName() << " data is already created" );
				return XST_FAIL;
			}

			if( pImg->GetWidth() == 0 || pImg->GetHeight() == 0 || pImg->GetBitsPerPixel() == 0 )
			{
				XST_LOG_ERR( "Image: " << pImg->GetResourceName() << " informations were not set properly" );
				return XST_FAIL;
			}

			pImage->m_uiChannelCount = m_aiChannelCounts[ pImage->m_eFormat ];
			pImage->m_uiPixelCount = pImage->m_uiWidth * pImage->m_uiHeight;
			pImage->m_ulDataSize = pImage->m_uiPixelCount * pImage->m_uiBytesPerPixel;
			pImage->m_uiBytesPerChannel = pImage->m_uiBytesPerPixel / pImage->m_uiChannelCount;
			pImage->m_pData = xst_new u8[ pImage->m_ulDataSize ];
			pImage->m_bManual = true;

			return XST_OK;
		}

		i32 CImageSystem::SaveImage(xst_castring& strFileName, Resources::IImage* pImg)
		{
			xst_assert2( pImg != xst_null );
			CImage* pImage = (CImage*)pImg;
			//If image was not generated
			if( pImage->m_uiImgId == 0 )
			{
				pImage->m_uiImgId = ilGenImage();
			}

			ilBindImage( pImage->m_uiImgId );
			if( pImage->m_bManual )
			{
				if( !ilTexImage(	pImage->m_uiWidth, pImage->m_uiHeight, 0, pImage->m_uiChannelCount, pImage->m_uiFormat, 
									pImage->m_uiType, pImage->m_pData ) )
				{
					XST_LOG_ERR( "Failed to save image: " << pImage->GetResourceName() << " : " << ilGetError() );
					return XST_FAIL;
				}
			}

			ilEnable( IL_FILE_OVERWRITE );
			if( !ilSaveImage( strFileName.data() ) )
			{
				ILenum eError = ilGetError();
				XST_LOG_ERR( "Failed to save image file: " << strFileName << ". Error: " << eError );
				return XST_FAIL;
			}
			return XST_OK;
		}

		u32 CImageSystem::_GetImageType(u32 uiImageType)
		{
			switch( uiImageType )
			{
				case ImageTypes::DDS:
				{
					return IL_DDS;
				}
				break;

				case ImageTypes::BMP:
				{
					return IL_BMP;
				}
				break;

				case ImageTypes::JPG:
				case ImageTypes::JPEG:
				{
					return IL_JPG;
				}
				break;

				case ImageTypes::PNG:
				{
					return IL_PNG;
				}
				break;

				case ImageTypes::TGA:
				{
					return IL_TGA;
				}
				break;

				case ImageTypes::RAW:
				{
					return IL_RAW;
				}
				break;

				case ImageTypes::HDR:
				{
					return IL_HDR;
				}
				break;

				case ImageTypes::ICO:
				{
					return IL_ICO;
				}
				break;

				case ImageTypes::TIFF:
				{
					return IL_TIF;
				}
				break;

				default:
				{
					return IL_TYPE_UNKNOWN;
				}
				break;
			};

			return IL_TYPE_UNKNOWN;
		}

		u32 CImageSystem::_GetImageType(xst_castring& strFileExt)
		{
			ul32 ulExt = XST::CHash::GetCRC( strFileExt );
			if( ulExt == m_aulExtHashes[ ImageTypes::BMP ] )
			{
				return IL_BMP;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::DDS ] )
			{
				return IL_DDS;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::HDR ] )
			{
				return IL_HDR;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::ICO ] )
			{
				return IL_ICO;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::JPEG ] )
			{
				return IL_JPG;
			}
			else
				if( ulExt == m_aulExtHashes[ ImageTypes::JPG ] )
			{
				return IL_JPG;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::PNG ] )
			{
				return IL_PNG;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::RAW ] )
			{
				return IL_RAW;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::TGA ] )
			{
				return IL_TGA;
			}
			else
			if( ulExt == m_aulExtHashes[ ImageTypes::TIFF ] )
			{
				return IL_TIF;
			}

			return IL_TYPE_UNKNOWN;
		}


	}//devil
}//xse