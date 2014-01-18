#ifndef XST_CPLANE_H
#define XST_CPLANE_H

#include "XSTCVector4.h"
//#include "XSE"

namespace XST
{
	using namespace Types;

	class CPlane
	{
		public:

			Math::CVector4	vecPlane;

		public:

							CPlane() {}
							CPlane(const Math::CVector4& vecPlane) : vecPlane( vecPlane ) {}
			virtual			~CPlane() {}

			xst_fi	void	Normalize()
							{ 
								f32 fLen = Math::CVector3::Length( vecPlane ); 
								if( fLen > 1e-08 ) fLen = 1.0f / fLen;
								vecPlane *= fLen;
							}

			xst_fi	f32		Dot(const Math::CVector4& vecVector) const
							{ return vecPlane.x * vecVector.x + vecPlane.y * vecVector.y + vecPlane.z * vecVector.z + vecVector.w * vecPlane.w; }

			xst_fi	f32		Dot(const Math::CVector3& vecVector) const
							{ return Dot( Math::CVector4( vecVector.x, vecVector.y, vecVector.z, 1.0f ) ); }

			xst_fi	f32		DotCoord(const Math::CVector4& vecVector) const
							{ return vecPlane.x * vecVector.x + vecPlane.y * vecVector.y + vecPlane.z * vecVector.z + vecPlane.w; }

			xst_fi	f32		DotCoord(const Math::CVector3& vecVector) const
							{ return vecPlane.x * vecVector.x + vecPlane.y * vecVector.y + vecPlane.z * vecVector.z + vecPlane.w; }
		
	};
}//xst

#endif