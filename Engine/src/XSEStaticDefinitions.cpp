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
		"mtxWorld",
		"mtxView",
		"mtxProjection",
		"mtxWVP",
		"mtxObjWorld", // object world
		"mtxObjView", // object view
		"mtxObjProjection", // object projection
		"mtxObjWVP", // object world view projection
		"mtxObjWInvT", // world inverse transpose
		"fTime",
		"f3LightPos",
		"f4LightSpecular",
		"f4LightDiffuse",
		"f4SceneAmbient",
		"f2ScreenSize"
	};

}//xse

#endif