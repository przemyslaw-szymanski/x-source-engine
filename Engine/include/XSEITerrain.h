#ifndef XSE_ITERRAIN_H
#define XSE_ITERRAIN_H

#include "XSEIRenderableObject.h"
#include "XSEIImage.h"

namespace XSE
{
	struct STerrainOptions
	{
		STerrainOptions() : Size( 256, 256 ), vecHeightRange( 0, 100 ), PageCount( 1, 1 ), PageVertexCount( 257, 257 ), bStatic( true ),
							uiLODCount( 3 ), TileVertexCount( 65, 65 ), bNormal( true ), bTangent( false ), bBinormal( false ),
							uiMaxLODCount( 1 ), bTexCoord( false )
		{
		}

		CPoint			Size;
		CPoint			PageCount;
		CPoint			PageVertexCount;
		CPoint			TileVertexCount;
		Vec2			vecHeightRange;
		u32				uiLODCount;
		u32				uiMaxLODCount;
		StringVector	vHeightmaps;
		u32 			bStatic : 1;
		u32 			bNormal : 1;
		u32 			bTangent : 1;
		u32 			bBinormal : 1;
		u32 			bColor : 1;
		u32 			bTexCoord : 1;
        u32             bEachTileHasOwnTexCoords : 1;
        u8 m_padding[ 2 ];
	};

	class XST_API ITerrain : public IRenderableObject
	{
		public:

									ITerrain(IInputLayout* pIL, lpcastr strDbgName, bool bManualRendering = true) : IRenderableObject( ObjectTypes::TERRAIN, pIL, strDbgName, bManualRendering ) {}
			virtual					~ITerrain() {}

			virtual void			DestroyData() { return; }
			virtual void			Update() { return; }
			virtual xst_fi const STerrainOptions&	GetOptions() const
													{ return m_Options; }

		protected:

			STerrainOptions		m_Options;
            u8 m_padding[ 56 ];
	};
}//xse

#endif