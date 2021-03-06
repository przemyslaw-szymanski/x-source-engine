#ifndef XSE_RENDER_SYSTEM_COMMON_H
#define XSE_RENDER_SYSTEM_COMMON_H

#include "XSECommon.h"

namespace XSE
{
    using namespace XST::Types;

    struct TopologyTypes
    {
        enum TOPOLOGY_TYPE
        {
            UNDEFINED	= 0,
            POINT_LIST,
            LINE_LIST,
            LINE_STRIP,
            TRIANGLE_LIST,
            TRIANGLE_STRIP,
            _ENUM_COUNT
        };
    };
    typedef TopologyTypes::TOPOLOGY_TYPE	TOPOLOGY_TYPE;

    struct ShaderTypes
    {
        enum TYPE
        {
            UNKNOWN = 0,
            VERTEX,
            PIXEL,
            GEOMETRY,
            COMPUTE,
            _ENUM_COUNT
        };
    };
    typedef ShaderTypes::TYPE	SHADER_TYPE;

    struct XST_API ShaderLanguages
    {
        static const ul32 HLSL;
        static const ul32 CG ;
        static const ul32 GLSL;
    };
    typedef ul32	SHADER_LANGUAGE;

    struct ShaderFlags
    {
        enum FLAG
        {
            DEBUG					= 0x00000001,
            BACKWARD_COMPAPILITY	= 0x00000010,
            OPTIMIZED				= 0x00000100,
            _ENUM_COUNT				= 3
        };
    };
    typedef ShaderFlags::FLAG	SHADER_FLAG;

    struct ShaderCompilers
    {
        enum COMPILER
        {
            UNDEFINED	= 0x00000000L,
            HLSL		= 0x80000000L,
            GLSL		= 0x80000000L,
            CG			= 0x40000000L,
            _ENUM_COUNT	= 4
        };
    };
    typedef ShaderCompilers::COMPILER	SHADER_COMPILER;

    struct ShaderConstants
    {
        enum CONSTANT
        {
            MTX_VIEW,
            MTX_PROJECTION,
            MTX_VIEW_PROJ,
            MTX_OBJ_WORLD,
            MTX_OBJ_VIEW,
            MTX_OBJ_PROJECTION,
            MTX_OBJ_WORLD_VIEW_PROJECTION,
            MTX_OBJ_WORLD_INVERSE_TRANSPOSE,
            TIME,
            LIGHT_POSITION,
            LIGHT_SPECULAR_POWER,
            LIGHT_COLOR,
            LIGHT_POWER,
            LIGHT_SPECULAR_COLOR,
            SCENE_AMBIENT_COLOR,
            SCREEN_SIZE,
            CAMERA_POSITION,
            CAMERA_DIRECTION,
            MATERIAL_DIFFUSE_COLOR,
            MATERIAL_AMBIENT_COLOR,
            MATERIAL_SPECULAR_COLOR,
            MATERIAL_ALPHA,
            MATERIAL_SHININESS,
            _ENUM_COUNT
        };
    };
    typedef ShaderConstants::CONSTANT	SHADER_CONSTANT;

    struct ConstantBuffers
    {
        enum CONSTANT_BUFFER
        {
            ONCE_PER_FRAME,
            VS_ONCE_PER_FRAME,
            VS_ONCE_PER_DRAWCALL,
            PS_ONCE_PER_FRAME,
            PS_ONCE_PER_DRAWCALL,
            ONCE_PER_MATERIAL,
            CB_VS_USER0,
            CB_VS_USER1,
            CB_VS_USER2,
            CB_VS_USER3,
            CB_PS_USER0,
            CB_PS_USER1,
            CB_PS_USER2,
            CB_PS_USER3,
            _ENUM_COUNT,
            _SYS_COUNT = CB_VS_USER0
        };
    };
    typedef ConstantBuffers::CONSTANT_BUFFER CONSTANT_BUFFER;

    struct ShaderModels
    {
        enum MODEL
        {
            UNKNOWN = 0,
            SM_1_0,
            SM_2_0,
            SM_3_0,
            SM_4_0,
            SM_5_0,
            _ENUM_COUNT
        };
    };
    typedef ShaderModels::MODEL	SHADER_MODEL;

    struct ShaderProfiles
    {
        enum PROFILE
        {
            UNKNOWN = 0,
            VS_UNKNOWN,
            PS_UNKNOWN,
            GS_UNKNOWN,
            HS_UNKNOWN,
            CS_UNKNOWN,
            DS_UNKNOWN,
            //SM 1.0
            VS_1_1,
            //SM 2.0
            VS_2_0,
            VS_2_X,
            PS_2_0,
            PS_2_X,
            //SM 3.0
            VS_3_0,
            PS_3_0,
            //SM 4.0
            VS_4_0,
            VS_4_1,
            PS_4_0,
            PS_4_1,
            GS_4_0,
            GS_4_1,
            //SM 5.0
            VS_5_0,
            PS_5_0,
            GS_5_0,
            DS_5_0,
            CS_5_0,
            HS_5_0,
            VS_BEST,
            PS_BEST,
            GS_BEST,
            DS_BEST,
            CS_BEST,
            HS_BEST,
            _ENUM_COUNT
        };
    };
    typedef ShaderProfiles::PROFILE	SHADER_PROFILE;

    struct BufferUsages
    {
        enum USAGE
        {
            DEFAULT = 0,
            DYNAMIC,
            STATIC,
            DYNAMIC_DOUBLEBUFFER,
            _ENUM_COUNT
        };
    };
    typedef BufferUsages::USAGE	BUFFER_USAGE;

    struct BufferLocationTypes
    {
        enum TYPE
        {
            HARDWARE = 0,
            HARDWARE_AND_SOFTWARE,
            _ENUM_COUNT
        };
    };
    typedef BufferLocationTypes::TYPE	BUFFER_LOCATION_TYPE;

    struct InputLayoutElements
    {
        enum ELEMENT
        {
            POSITION		= 0x80000000L,
            NORMAL			= 0x40000000L,
            TEXCOORD0		= 0x20000000L,
            BINORMAL		= 0x10000000L,
            TANGENT			= 0x08000000L,
            SPECULAR		= 0x04000000L,
            DIFFUSE			= 0x02000000L,
            COLOR			= 0x01000000L,
            TEXCOORD1		= 0x00800000L,
            TEXCOORD2		= 0x00400000L,
            TEXCOORD3		= 0x00200000L,
            TEXCOORD4		= 0x00100000L,
            TEXCOORD5		= 0x00080000L,
            TEXCOORD6		= 0x00040000L,
            TEXCOORD7		= 0x00020000L,
            _ENUM_COUNT		= 15
        };
    };
    typedef InputLayoutElements::ELEMENT	INPUT_LAYOUT_ELEMENT;
    typedef InputLayoutElements				VertexAttributes;
    typedef VertexAttributes::ELEMENT		VERTEX_ATTRIBUTE;
    typedef InputLayoutElements				ILE;

    struct InputLayoutElementIds
    {
        enum ID
        {
            POSITION = 0,
            NORMAL,
            TEXCOORD0,
            BINORMAL,
            TANGENT,
            SPECULAR,
            DIFFUSE,
            COLOR,
            TEXCOORD1,
            TEXCOORD2,
            TEXCOORD3,
            TEXCOORD4,
            TEXCOORD5,
            TEXCOORD6,
            TEXCOORD7,
            _ENUM_COUNT
        };
    };
    typedef InputLayoutElementIds::ID		INPUT_LAYOUT_ELEMENT_ID;
    typedef InputLayoutElementIds			ILEIds;
    typedef ILEIds::ID						ILE_ID;

    struct InputLayoutElementsSize
    {
        enum
        {
            POSITION	= sizeof( Vec3 ),
            NORMAL		= sizeof( Vec3 ),
            TEXCOORD	= sizeof( Vec2 ),
            BINORMAL	= sizeof( Vec3 ),
            TANGENT		= sizeof( Vec3 ),
            COLOR		= sizeof( Vec4 ),
            _ENUM_COUNT	= 6
        };
    };
    typedef InputLayoutElementsSize	ILESizes;

    struct RSFormats
    {
        enum FORMAT
        {
            UNKNOWN = 0,
            R32F,
            R32G32F,
            R16F,
            R16G16F,
            R16G16B16A16U,
            R16G16B16A16F,
            R16G16U,
            R16U,
            R8U,
            R8G8B8U,
            R8G8B8A8U,
            R32G32B32F,
            R32G32B32A32F,
            B5G6R5U,
            G5G5R5A1U,
            B8G8R8A8,
            _ENUM_COUNT,
            RGBA32F		= R32G32B32A32F,
            RGB32F		= R32G32B32F,
            RGBA8U		= R8G8B8A8U,
            RGBA16F		= R16G16B16A16F,
            RGBA16U		= R16G16B16A16U
        };
    };
    typedef RSFormats::FORMAT	RS_FORMAT;

    struct RSFormatSizes
    {
        enum SIZE
        {
            UNKNOWN			= 0,
            R32F			= 32 * 1,
            R32G32F			= 32 * 2,
            R16F			= 16 * 1,
            R16G16F			= 16 * 2,
            R16G16B16A16U	= 16 * 4,
            R16G16B16A16F	= 16 * 4,
            R16G16U			= 16 * 2,
            R16U			= 16 * 1,
            R8U				= 8 * 1,
            R8G8B8A8U		= 8 * 4,
            R32G32B32F		= 32 * 3,
            R32G32B32A32F	= 32 * 4,
            _ENUM_COUNT		= 14,
            RGBA32F		= R32G32B32A32F,
            RGB32F		= R32G32B32F,
            RGBA8U		= R8G8B8A8U,
            RGBA16F		= R16G16B16A16F,
            RGBA16U		= R16G16B16A16U
        };
    };

    struct SRendererResourceHandle
    {
        xst_fi bool operator!() const { return uHandle != 0; }
        xst_fi bool operator>(const SRendererResourceHandle& o) const { return uHandle > o.uHandle; }
        xst_fi bool operator<(const SRendererResourceHandle& o) const { return uHandle < o.uHandle; }
        xst_fi operator bool() const { return uHandle > 0; }
        xst_fi void operator=( const SRendererResourceHandle& Other ) { uHandle = Other.uHandle; }
        u64 uHandle = 0;
    };
    typedef SRendererResourceHandle RSHandle;
    typedef const RSHandle& RSHandleRef;
    typedef RSHandle* RSHandlePtr;
#define RSHANDLE_OK(_handle) ((_handle).uHandle > 0)

    struct TextureTypes
    {
        enum TYPE
        {
            UNKNOWN,
            TEX_1D,
            TEX_2D,
            TEX_3D,
            TEX_CUBE,
            _ENUM_COUNT
        };
    };
    typedef TextureTypes::TYPE TEXTURE_TYPE;

    struct SImageMipMapDesc
    {
        cu8*	pData;
        u32		uDataSize;
        u16		uWidth;
        u16		uHeight;
    };

    typedef xst_vector< SImageMipMapDesc > MipMapDescVec;

    struct SImageMipMapBuffer
    {
        cu8*				pBuffer; // Buffer for mipmaps
        MipMapDescVec		vMipMaps;
        ul32				ulBufferSize;
    };

    struct STextureDesc
    {
        SImageMipMapBuffer	MipMapDesc;
        cu8*				pData;
        ul32				uDataSize;
        u16					uWidth;
        u16					uHeight;
        u16					uMipCount : 12;
        u16					bGenerateMipMaps : 1;
        u16					bMultisampled : 1;
        u16					bRawData : 1; // Uncompressed data
        u16					bNative : 1; // Native format for a 3D API (e.g. dds for D3D)
        u16					uPixelSize; // in bytes - bits per pixel / 4
        RS_FORMAT			eFormat;
        TEXTURE_TYPE		eType;
    };

    /*struct BasicShapes
    {
        enum BASIC_SHAPE
        {
            BOX = 1,
            LINE_BOX,
            PLANE,
            RECT_2D,
            CIRCLE,
            _ENUM_COUNT
        };
    };
    typedef BasicShapes::BASIC_SHAPE	BASIC_SHAPE;*/
    enum class BasicShapes : u8
    {
        BOX = 1,
        LINE_BOX,
        PLANE,
        RECT_2D,
        CIRCLE,
        _ENUM_COUNT
    };
    typedef BasicShapes	BASIC_SHAPE;

    struct SBasicShapeOptions
    {
        SBasicShapeOptions() : vecPos( Vec3::ZERO ) {}
    
        Vec3 vecPos;
    };

    struct SBoxOptions : SBasicShapeOptions
    {
        SBoxOptions() : vecSize( 1, 1, 1 ), colColor( CColor::WHITE ) {}

        Vec3	vecSize;
        CColor	colColor;
    };

    struct SLineBoxOptions : SBoxOptions
    {

    };

    struct SCircleOptions : SBasicShapeOptions
    {
        SCircleOptions() {}

        f32		fRadius	= 1.0f;
        u8		uStep = 1;
    };

    struct SPlaneOptions : SBasicShapeOptions
    {
        SPlaneOptions() : vecNormal( 0, 1, 0 ), vecVertexCount( 2, 2 ), vecSize( 2, 2 ) {}

        Vec3	vecNormal;
        Vec2	vecSize;
        Vec2	vecVertexCount;
    };

    struct SRect2DOptions : SBasicShapeOptions
    {
        SRect2DOptions() : vecSize( 100, 100 ), vecPosition( 0, 0 ), eUsage( BufferUsages::STATIC ) {}

        Vec2			vecSize;
        Vec2			vecPosition;
        BUFFER_USAGE	eUsage;
    };

    struct SRSDiagnostics
    {
        ul32	ulDrawCallCount = 0;
        ul32	ulIndexedDrawCallCount = 0;
        ul32	ulSetPixelShaderCount = 0;
        ul32	ulSetVertexShaderCount = 0;
        ul32	ulSetGeometryShaderCount = 0;
        ul32	ulSetInputLayoutCount = 0;
        ul32	ulSetVertexBufferCount = 0;
        ul32	ulSetIndexBufferCount = 0;
        ul32    ulCreatedVertexBufferCount = 0;
        ul32    ulCreatedIndexBufferCount = 0;
    };

    struct FillModes
    {
        enum MODE
        {
            SOLID = 0,
            WIREFRAME,
            _ENUM_COUNT
        };
    };
    typedef FillModes::MODE	FILL_MODE;

    struct CullModes
    {
        enum MODE
        {
            NONE = 0,
            FRONT,
            BACK,
            _ENUM_COUNT
        };
    };
    typedef CullModes::MODE	CULL_MODE;

    struct IndexElementSizes
    {
        enum SIZE
        {
            UNSIGNED_16,
            UNSIGNED_32,
            _ENUM_COUNT
        };
    };
    typedef IndexElementSizes::SIZE INDEX_ELEMENT_SIZE;

    struct TextureFilters
    {
        enum FILTER
        {
            POINT,
            LINEAR,
            ANISOTROPIC,
            _ENUM_COUNT
        };
    };
    typedef TextureFilters::FILTER TEXTURE_FILTER;

    struct TextureAddresses
    {
        enum ADDRESS
        {
            WRAP,
            CLAMP,
            MIRROR,
            _ENUM_COUNT
        };
    };
    typedef TextureAddresses::ADDRESS TEXTURE_ADDRESS;

    struct TextureLODs
    {
        enum LOD
        {
            LEVEL_0,
            LEVEL_1,
            LEVEL_2,
            LEVEL_3,
            LEVEL_4,
            LEVEL_5,
            LEVEL_MAX,
            _ENUM_COUNT,
            LEVEL_MIN = LEVEL_0,
        };
    };
    typedef TextureLODs::LOD TEXTURE_LOD;

    struct STextureSamplingMode
    {
        TEXTURE_FILTER	eMinFilter	= TextureFilters::LINEAR;
        TEXTURE_FILTER	eMagFilter	= TextureFilters::LINEAR;
        TEXTURE_FILTER	eMipFilter	= TextureFilters::LINEAR;
        TEXTURE_ADDRESS	eAddressU	= TextureAddresses::WRAP;
        TEXTURE_ADDRESS eAddressV	= TextureAddresses::WRAP;
        TEXTURE_ADDRESS eAddressW	= TextureAddresses::WRAP;
        TEXTURE_LOD		eMinLOD		= TextureLODs::LEVEL_MIN;
        TEXTURE_LOD		eMaxLOD		= TextureLODs::LEVEL_MAX;
    };

    struct XST_API SViewportOptions
    {
        SViewportOptions() { }

        u32				uiResolutionWidth = 800;
        u32				uiResolutionHeight = 600;
        FILL_MODE		eFillMode = FillModes::SOLID;
        CULL_MODE		eCullMode = CullModes::BACK;
        bool			bAntialiasing = false;
        bool			bMultisampling = false;
        bool			bScissor = false;
    };

    struct XST_API SRenderSystemMemorySettings
    {
        u32 uiVertexBufferCount = 2000;
        u32 uiIndexBufferCount = 2000;
        u32 uiPixelShaderCount = 100;
        u32 uiVertexShaderCount = 100;
    };

    struct XST_API SRenderSystemSettings : public SViewportOptions
    {

        SRenderSystemSettings() {}

        u32				uiRefreshRate = 60;
        u32				uiBitsPerPixel = 24;
        u32				uiHWND = 0;
        ul32			ulShaderFlags = 0;
        SHADER_MODEL	eMinFeatureLevel = ShaderModels::UNKNOWN;
        SHADER_MODEL	eMaxFeatureLevel = ShaderModels::UNKNOWN;
        u8				uMaxAnisotropy = 16;
        bool			bFullScreen = false;
        bool			bDebugMode = false;
        bool			bVSync = false;
        SRenderSystemMemorySettings	MemOptions;
    };

    struct XST_API SResolution
    {
        typedef xst_vector< u16 > RefreshVector;

        u32				uiWidth;
        u32				uiHeight;
        RefreshVector	vRefreshRates;
    };

    struct SRSLimits
    {
        ul64            ulMaxResourceSize;
        ul64            ulMaxVideoMemory;
        ul64            ulMaxSystemMemory;
        ul64            ulMaxVertexBufferSize;
        ul64            ulMaxIndexBufferSize;
        ul32            ulMaxConstantBufferElementCount;
        ul32            ulMaxTexture1DDimmension;
        ul32            ulMaxTexture1DArrayDimmension;
        ul32            ulMaxTexture2DDimmension;
        ul32            ulMaxTexture2DArrayDimmension;
        ul32            ulMaxTexture3DDimmension;
        ul32            ulMaxTextureCubeDimmension;
        ul32            ulMaxAnisotropy;
        ul32            ulMaxResourceCount;
        ul32            ulMaxDrawVertexCount;
        ul32            ulMaxSamplerCount;
        ul32            ulMaxMultisampleCount;
        ul32            ulMaxMRTs;
        ul32            ulMaxTextureRepeat;
        ul32            ulMaxMipLevels;
        ul32            ulMaxPrimitiveCount;
        ul32            ulMaxVertexIndex;
        ul32            ulMaxTextureUnits;
    };

    struct SRSFeatures
    {
        SRSLimits       Limits;
        SHADER_MODEL	eShaderModel;
        bool			bTextureArrays;
        bool			bCubemapArrays;
        bool			bOcclusionQueries;
        bool			bInstancing;
        bool			bNonPowerOfTwoDimTextures;
    };

    struct XST_API SRSCapability
    {
        u32		uiWidth;
        u32		uiHeight;
        u16		usRefreshRate;
        u16		usMultiSampleLevel;
    };

    struct XST_API SRSCapapiblities
    {
        typedef xst_vector< SResolution >		ResolutionVector;
        typedef xst_vector< u8 >				MultiSampleLevelVector;
        
        SRSCapapiblities()
        {
            xst_zero( abShaderModels, sizeof( u32 ) * ShaderModels::_ENUM_COUNT );
            xst_zero( abShaderProfiles, sizeof( u32 ) * ShaderProfiles::_ENUM_COUNT );
        }

        bool IsResolutionAvailable(u32 uiWidth, u32 uiHeight, u16 usRefRate)
        {
            ResolutionVector::iterator Itr;
            SResolution::RefreshVector::iterator RItr;
            xst_stl_foreach( Itr, vResolutions )
            {
                if( (*Itr).uiWidth == uiWidth && (*Itr).uiHeight == uiHeight )
                {
                    xst_stl_foreach( RItr, (*Itr).vRefreshRates )
                    {
                        if( (*RItr) == usRefRate )
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        xst_fi bool IsShaderModelAvailable(SHADER_MODEL eShaderModel)
        {
            xst_assert2( eShaderModel >= (SHADER_MODEL)0 && eShaderModel < ShaderModels::_ENUM_COUNT );
            return abShaderModels[ eShaderModel ];
        }

        xst_fi bool IsShaderProfileAvailable(SHADER_PROFILE eShaderProfile)
        {
            xst_assert2( eShaderProfile >= (SHADER_PROFILE)0 && eShaderProfile < ShaderProfiles::_ENUM_COUNT );
            return abShaderProfiles[ eShaderProfile ];
        }

        xst_fi bool IsMultiSampleLevelAvailable(u8 byLevel)
        {
            MultiSampleLevelVector::iterator Itr;
            xst_stl_foreach( Itr, vMultiSampleLevels )
            {
                if( (*Itr) == byLevel )
                {
                    return true;
                }
            }

            return false;
        }

        ResolutionVector		vResolutions;
        bool					abShaderModels[ ShaderModels::_ENUM_COUNT ];
        bool					abShaderProfiles[ ShaderProfiles::_ENUM_COUNT ];
        MultiSampleLevelVector	vMultiSampleLevels;
        SRSFeatures				Features;
    };

}//xse

#endif