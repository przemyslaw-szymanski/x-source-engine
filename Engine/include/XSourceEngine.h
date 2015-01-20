#ifndef XSE_XSOURCE_ENGINE_H
#define XST_XSOURCE_ENGINE_H

#include "XSECommon.h"
#include "XSECEngine.h"
#include "XSECLuaScriptManager.h"
#include "XSECMaterialManager.h"
#include "XSEIRenderWindow.h"
#include "XSEIRenderSystem.h"
#include "XSECTextureManager.h"
#include "XSECModelManager.h"
#include "XSEIKeyboardListener.h"
#include "XSEIMouseListener.h"
#include "XSEIRenderWindowListener.h"
#include "XSEIViewport.h"
#include "XSECResourceFileManager.h"
#include "XSECShaderManager.h"
#include "XSECSceneManager.h"
#include "XSECSceneNode.h"
#include "XSECMesh.h"
#include "XSECMeshManager.h"
#include "XSEIVertexBuffer.h"
#include "XSECModel.h"
#include "XSECCamera.h"
#include "XSECVertexData.h"
#include "XSECIndexData.h"
#include "XSEITechnique.h"
#include "XSEIPass.h"
#include "XSEIIndexBuffer.h"
#include "XSECStaticGeometry.h"
#include "XSECStaticGeometryGroup.h"
#include "XSECDynamicGeometry.h"
#include "XSECDynamicGeometryGroup.h"
#include "XSECImageManager.h"
#include "XSEIImage.h"
#include "XSEIImageSystem.h"
#include "XSECTerrainSystem.h"
#include "XSECMipMapPagingTerrain.h"
#include "XSEIMouseListener.h"
#include "XSEIKeyboard.h"
#include "XSEIMouse.h"
#include "XSECLight.h"
//GUI
#include "GUI/XSECGUIManager.h"
#include "GUI/XSECComponent.h"
#include "GUI/XSEIGUIRenderer.h"

XST_API XSE::CEngine* 	XSECreateEngine();
XST_API void 			XSEDestroyEngine(XSE::CEngine** ppEngine);

#endif