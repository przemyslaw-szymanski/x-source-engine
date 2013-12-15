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
		bool			bStatic;
		bool			bNormal;
		bool			bTangent;
		bool			bBinormal;
		bool			bColor;
		bool			bTexCoord;
	};

	class ITerrain : public IRenderableObject
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
	};
}//xse

#endif