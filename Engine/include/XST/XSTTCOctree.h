#ifndef XST_TCOCTREE_H
#define XST_TCOCTREE_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"
#include "XSTCAABB.h"

namespace XST
{
	using namespace Types;

	class COctBounds
	{
		public:

			Vec4	vecPosition;
			Vec4	vecCenter;
			f32		fSize;
            u8 m_padding[ 12 ];
	};

	//template<class _T_>
	class TCOctree
	{
		public:

			typedef TCOctree	Node;

			enum
			{
				LEFT_TOP_FRONT = 0,
				RIGHT_TOP_FRONT,
				RIGHT_BOTTOM_FRONT,
				LEFT_BOTTOM_FRONT,

				LEFT_TOP_BACK,
				RIGHT_TOP_BACK,
				RIGHT_BOTTOM_BACK,
				LEFT_BOTTOM_BACK
			};

		public:

								TCOctree();
			virtual				~TCOctree();

			virtual void		DestroyChildren();

			virtual i32			Create(cu16& usDepth, cf32& fSize, const Vec3& vecPosition);
			virtual i32			Create(cf32& fMinSize, cf32& fSize, const Vec3& vecPosition);

		protected:

			virtual i32			_Create(cu16& usDepth, cu16& usCurrentDepth, cf32& fCurrentSize, const Vec3& vecPosition, Node* pParent);
			virtual i32			_Create(cf32& fMinSize, cf32& fCurrentSize, const Vec3& vecPosition, Node* pParent);

		protected:

			Node		*m_apChildren[ 8 ];
			Node*		m_pParent;
			CAABB		m_Bounds;
            u8 m_padding[ 12 ];
			u16			m_usDepth;
            u8 m_padding2[ 14 ];
	};


	

}//xst

#endif