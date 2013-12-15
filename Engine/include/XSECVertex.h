#ifndef XSE_CVERTEX_H
#define XSE_CVERTEX_H

#include "XSECommon.h"

namespace XSE
{
	class CVertexF3
	{
		public:

			Vec3	vecPosition;
	};

	class CVertex
	{
		public:

		public:

			Vec3	vecPosition;
			Vec3	vecNormal;
			Vec2	avecTexCoords[7];
	};

	class CVertexB : public virtual CVertex
	{
		public:

			Vec3	vecBinormal;
	};

	class CVertexT : public virtual CVertex
	{
		public:

			Vec3	vecTangent;
	};

	class CVertexC : public virtual CVertex
	{
		public:

			XST::CColor	Color;
	};

	class CVertexS : public virtual CVertex
	{
		public:

			Vec3	vecSpecular;
	};

	class CVertexD : public virtual CVertex
	{
		public:

			Vec3	vecDiffuse;
	};

	class CVertexBTCSD :	public virtual CVertex, public virtual CVertexB, public virtual CVertexT, 
							public virtual CVertexC, public virtual CVertexS, public virtual CVertexD
	{
	};

	typedef CVertexF3	Vertex;

}//xse

#endif //XSE_CVERTEX_H