#ifndef XSE_STATIC_DEFINITIONS_H
#define XSE_STATIC_DEFINITIONS_H

#include "XSERenderSystemCommon.h"
#include "../include/XSECLuaScriptManager.h"
#include "../include/XSECMaterialManager.h"
#include "../include/XSECResourceFileManager.h"
#include "../include/XSECModelManager.h"
#include "../include/XSECTextureManager.h"
#include "../include/XSECShaderManager.h"
#include "Win32/XSECWin32RenderWindow.h"
#include "../include/XSECRenderSystemFactory.h"
#include "D3D11/XSECD3D11RenderSystem.h"
#include "D3D9/XSECD3D9RenderSystem.h"
#include "XSECSceneManager.h"
#include "XSECMeshManager.h"
#include "XSECModelManager.h"
#include "XSEIShaderSystem.h"

namespace XSE
{
	const ul32 ShaderLanguages::HLSL = XST::CHash::GetCRC( "hlsl" );
	const ul32 ShaderLanguages::CG = XST::CHash::GetCRC( "cg" );
	const ul32 ShaderLanguages::GLSL = XST::CHash::GetCRC( "glsl" );

	xst_astring IShaderSystem::CONSTANT_NAMES[ ShaderConstants::_ENUM_COUNT ] = 
	{
		"mtxView",
		"mtxProjection",
		"mtxViewProj",
		"mtxObjWorld", // object world
		"mtxObjView", // object view
		"mtxObjProjection", // object projection
		"mtxObjWVP", // object world view projection
		"mtxObjWorldInvT", // world inverse transpose
		"fTime",
		"f3LightPos",
		"f4LightSpecular",
		"f4LightColor",
		"f4SceneAmbientColor",
		"f2ScreenSize",
		"f3CameraPos",
		"f3CameraDir"
	};

	/*struct ShaderConstants
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
			LIGHT_SPECULAR,
			LIGHT_COLOR,
			SCENE_AMBIENT_COLOR,
			SCREEN_SIZE,
			CAMERA_POSITION,
			CAMERA_DIRECTION,
			_ENUM_COUNT
		};
	};*/

}//xse

#endif