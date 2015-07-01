#ifndef XSE_ITERRAIN_SYSTEM_H
#define XSE_ITERRAIN_SYSTEM_H

#include "XSECommon.h"
#include "XST/XSTCBinaryData.h"
#include "XSEITerrain.h"

namespace XSE
{
	class XST_API ITerrainSystem
	{
		public:

			virtual				~ITerrainSystem() {}

			virtual ITerrain*	CreateTerrain(xst_castring& strName, STerrainOptions& Options, bool bLockOnly) = 0;
			virtual ITerrain*	CreateTerrain(xst_castring& strName, STerrainOptions& Options) = 0;
			virtual ITerrain*	CreateFromImage(xst_castring& strName, const Resources::IImage* pImg, STerrainOptions& Options) = 0;
			virtual ITerrain*	CreateFromBinary(xst_castring& strName, const XST::CBinaryData& Data, STerrainOptions& Options) = 0;
			virtual i32			LockTerrain(ITerrain* pTerrain) = 0;
			virtual i32			SaveToImage(xst_castring& strImg, const ITerrain* pTerrain) = 0;
			virtual i32			SaveToBinary(xst_castring& strBinaryName, const ITerrain* pTerrain) = 0;
			virtual void		DestroyTerrain(ITerrain* pTerrain) = 0;
			virtual void		DestroyTerrain(xst_castring& strName) = 0;
			virtual bool		ValidateOptions(STerrainOptions& Options) = 0;
			virtual void		DestroyTerrains() = 0;
			virtual bool		IsAutoDestroy() const = 0;
			virtual void		Update() = 0;

		protected:
	};

}//xse

#endif