#ifndef XSE_COMMON_H
#define XSE_COMMON_H

#include "XST/XST.h"

#define XSE_MAJOR_VER	1
#define XSE_MINOR_VER	0
#define XSE_SUB_VER		0
#define XSE_VER			100

#if defined( XST_DEBUG )
#	define XSE_ENGINE_DEBUG				1
#	define XSE_SCENE_DEBUG				1
#	define XSE_AI_DEBUG					1
#	define XSE_PHYSICS_DEBUG			1
#	define XSE_RENDERER_DEBUG			1
#endif //XST_DEBUG

// ENGINE COMPILE CONSTANTS
#ifndef XSE_MESH_MAX_LOD_LEVELS
#   define XSE_MESH_MAX_LOD_LEVELS 4
#endif

#pragma warning( disable: 4251 ) //dll-linkage of members

#define XSE_PROP(_type, _name, _var) \
	xst_fi void Set##_name(const _type& value) { _var = value; } \
	xst_fi _type Get##_name() { return _var; }

#define XSE_PROP_V2(_type, name_, _var) \
	XSE_PROP( _type, _name, _var ) \
	xst_fi void Set##_name(cf32& fX, cf32& fY) { _var.fX = fX; _var.fY = fY; }

#define XSE_PROP_V3(_type, name_, _var) \
	XSE_PROP( _type, _name, _var ) \
	xst_fi void XST_ADD( Set, XST_TOSTRING( _name ) )(cf32& fX, cf32& fY, cf32& fZ) { _var.fX = fX; _var.fY = fY; _var.fZ = fZ; }

#define XSE_PROP_V4(_type, name_, _var) \
	XSE_PROP( _type, _name, _var ) \
	xst_fi void Set##_name(cf32& fX, cf32& fY, cf32& fZ, cf32& fW) { _var.fX = fX; _var.fY = fY; _var.fZ = fZ; _var.fW = fW; }

#if defined (XST_WINDOWS)
#	define xse_main() WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#	define xse_main() main(int argc, char** argv)
#endif

#define XSE_LOAD_FUNC(_func, _ptrFunc, _handle, _funcName) \
	_func = (_ptrFunc)XST::Platform::GetProcAddress( _handle, _funcName ); \
	if( _func == xst_null ) { XST_LOG_ERR( "Function: " << _funcName << " doesn't exists in library" ); return RESULT::FAILED; }

//Uses to string macro from _func
#define XSE_LOAD_FUNC2(_func, _ptrFunc, _handle) \
	_func = (_ptrFunc)XST::Platform::GetProcAddress( _handle, XST_TOSTRING( _func ) ); \
	if( _func == xst_null ) { XST_LOG_ERR( "Function: " << XST_TOSTRING( _func ) << " doesn't exists in library" ); return RESULT::FAILED; }

//Uses to string macro from _func
//Uses add macro to add 'pfn' before _func as function typedef pointer
#define XSE_LOAD_FUNC3(_func, _handle) \
	_func = ( XST_ADD( pfn, _func ) )XST::Platform::GetProcAddress( _handle, XST_TOSTRING( _func ) ); \
	if( _func == xst_null ) { XST_LOG_ERR( "Function: " << XST_TOSTRING( _func ) << " doesn't exists in library" ); return RESULT::FAILED; }

#define XSE_NEW_CHECK(_ptr, _class, _errorText, _returnValue) _ptr = xst_new _class; if( _ptr == xst_null ) { XST_LOG_ERR( _errorText ); return ( _returnValue ); }

namespace XSE
{
	using namespace XST::Types;

    class IResourceGroup;
	class IResourceManager;
    class IResourceManager2;
	class CLuaScriptManager;
	class CMaterialManager;
	class CTextureManager;
	class CMeshManager;
	class CEngine;
	class CResourceFileManager;
	class CModelManager;
	class CShaderManager;
	class IRenderWindow;
	class CLuaMaterialParser;
	class CBinaryMaterialReader;
	class CLuaMaterialReader;
	class CSceneManager;
	class CSceneNode;
	class CObject;
	class CRenderSystemFactory;
	class IRenderSystem;
	class IWindowListener;
	class IKeyboardListener;
	class IMouseListener;
	class IRenderWindowListener;
	class IViewport;
	class IInputLayout;
	class IRenderableObject;
	class CMeshManager;
	class IVertexBuffer;
	class IIndexBuffer;
	class CRenderQueue;
	class CCamera;
	class IShaderSystem;
	class ITechnique;
	class IPass;
	class CStaticGeometry;
	class CStaticGeometryGroup;
	class CDynamicGeometry;
	class CDynamicGeometryGroup;
	class CDynamicGeometryObject;
	class CStaticGeometryObject;
	class CImageManager;
	class IImageSystem;
	class ITerrainSystem;
	class CTerrainSystem;
	class CPagingTerrainSystem;
	class CMipMapPagingTerrain;
	class ITerrain;
	class IKeyboard;
	class IMouse;
	
	struct SWindowEvent;
	struct SKeyboardEvent;
	struct SMouseEvent;

	namespace Resources
	{
		class IResource;
		class CLuaScript;
		class CMaterial;
		class ITexture;
		class CMesh;
		class CModel;
		class IShader;
		class IVertexShader;
		class IPixelShader;
		class IGeometryShader;
		class IComputeShader;
		class IImage;
	}//resources

	typedef XST::CFileManager		CFileManager;
	typedef XST::Math::CVector2		Vec2;
	typedef XST::Math::CVector3		Vec3;
	typedef XST::Math::CVector4		Vec4;
	typedef XST::Math::CMatrix3		Mtx3;
	typedef XST::Math::Mtx4			Mtx4;
	typedef XST::Math::CPoint		CPoint;
	typedef XST::CColor				CColor;
	typedef XST::Math::CQuaternion	Quaternion;
	typedef Quaternion				Quat;

	static xst_castring	ALL_GROUPS( XST::ALL_GROUPS );
	static xst_castring DEFAULT_GROUP( XST::DEFAULT_GROUP );

	template<class _T_>
	using ConstPtr = _T_ *const;

	template<class _T_>
	using ConstPtrToConstPtr = _T_ *const *const;

	xst_tvectorx( xst_astring )	StringVector;
	xst_tvectorx( i32 )			IntVector;
	xst_tvectorx( u32 )			UIntVector;

	struct MatrixTypes
	{
		enum TYPE
		{
			WORLD,
			VIEW,
			PROJECTION,
			VIEW_PROJ,
			WORLD_VIEW_PROJ,
			TRANSPOSED_WORLD,
			TRANSPOSED_VIEW,
			TRANSPOSED_PROJ,
			TRANSPOSED_VIEW_PROJ,
			TRANSPOSED_WORLD_VIEW_PROJ,
			INVERTED_VIEW_PROJ,
			INVERTED_TRANSPOSED_WORLD,
			_ENUM_COUNT
		};
	};
	typedef MatrixTypes::TYPE	MATRIX_TYPE;

	struct ResourceTypes : public XST::ResourceType
	{
		enum TYPE
		{
			IMAGE = XST::ResourceType::SHADER + 1,
			_ENUM_COUNT
		};
	};
	typedef ResourceTypes::TYPE		RESOURCE_TYPE;

	struct ImageTypes
	{
		enum TYPE
		{
			UNKNOWN = 0,
			BMP,
			PNG,
			DDS,
			JPG,
			JPEG,
			TGA,
			HDR,
			RAW,
			ICO,
			TIFF,
			_ENUM_COUNT
		};
	};
	typedef ImageTypes::TYPE	IMAGE_TYPE;

	struct ImageFormats
	{
		enum FORMAT
		{
			UNKNOWN = 0,
			RGB,
			RGBA,
			BGR,
			BGRA,
			LUMINANCE,
			_ENUM_COUNT
		};
	};
	typedef ImageFormats::FORMAT	IMAGE_FORMAT;

	struct ImageDataTypes
	{
		enum TYPE
		{
			INT_8,
			UINT_8,
			INT_16,
			UINT_16,
			INT_32,
			UINT_32,
			FLOAT_32,
			FLOAT_64,
			_ENUM_COUNT
		};
	};
	typedef ImageDataTypes::TYPE	IMAGE_DATA_TYPE;

	struct ImageBPPTypes
	{
		enum TYPE
		{
			BPP_8 = 8,
			BPP_24 = 24,
			BPP_32 = 32,
			_ENUM_COUNT = 3
		};
	};
	typedef ImageBPPTypes::TYPE	IMAGE_BPP_TYPE;

	struct ObjectTypes
	{
		enum OBJECT_TYPE
		{
			UNKNOWN = 0,
			TERRAIN,
			MESH,
			MODEL,
			BILLBOARD,
			CAMERA,
			SCENE_NODE,
			STATIC_GEOMETRY_GROUP,
			STATIC_GEOMETRY,
			DYNAMIC_GEOMETRY_GROUP,
			DYNAMIC_GEOMETRY,
			GEOMETRY_BATCH,
			_ENUM_COUNT
		};
	};
	typedef ObjectTypes		OT;
	typedef OT::OBJECT_TYPE	OBJECT_TYPE;

	struct ObjectDisableReasons
	{
		enum REASON
		{
			NOT_DISABLED		= 0x00000000,
			FRUSTUM_CULLING		= 0x00000001,
			OCCLUSION_CULLING	= 0x00000002,
			RANGE_CULLING		= 0x00000004,
            OTHER				= 0x00000008,
			_ENUM_COUNT = 5
		};
	};
	typedef ObjectDisableReasons	ODR;
	typedef ODR::REASON				OBJECT_DISABLE_REASON;

	struct CameraTypes
	{
		enum TYPE
		{
			FREE = 0,
			FPP,
			TPP,
			RTS,
			_ENUM_COUNT
		};
	};
	typedef CameraTypes::TYPE	CAMERA_TYPE;

	struct FPPCameraProperties
	{
		enum PROPERTY
		{
			UNKNOWN = 0,
			MOUSE_DELTA,
			_ENUM_COUNT
		};
	};
	typedef FPPCameraProperties::PROPERTY	FPP_CAM_PROPERTY;

	enum class ViewFrustumCullTypes
	{
		NONE,
		SPHERE,
		AABB,
		SPHERE_AABB,
		RANGE,
		RANGE_SPHERE,
		RANGE_AABB,
		RANGE_SPHERE_AABB
	};
	typedef ViewFrustumCullTypes VIEW_FRUSTUM_CULL_TEST_TYPE;

}//xse

#endif
