#ifndef XST_CCOLOR_H
#define XST_CCOLOR_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"
#include "XSTCVector4.h"

namespace XST
{
	using namespace Types;

	class XST_API CColor
	{
		public:

			union
			{
				struct
				{
					u32 r : 8;
					u32 g : 8;
					u32 b : 8;
					u32 a : 8;
				};

				u32 rgba;
				u8 channels[4];
			};

		public:

			static const CColor	BLACK;
			static const CColor WHITE;
			static const CColor BLUE;
			static const CColor RED;
			static const CColor GREEN;

		public:

									CColor() : r( 0 ), g( 0 ), b( 0 ), a( 255 ) {}
									CColor(u8 uR, u8 uG, u8 uB, u8 uA) : r( uR ), g( uG ), b( uB ), a( uA ) {}
									CColor(u8 uR, u8 uG, u8 uB) : r( uR ), g( uG ), b( uB ), a( 255 ) {}
									//CColor(f32 fR, f32 fG, f32 fB, f32 fA) : r( (u8)(fR * 255) ), g( (u8)(fG * 255) ), b( (u8)(fB * 255) ), a( (u8)(fA * 255) ) {}
									//CColor(f32 fR, f32 fG, f32 fB) : r( (u8)(fR * 255) ), g( (u8)(fG * 255) ), b( (u8)(fB * 255) ), a( 255 ) {}
									CColor(cu32& uiRGBA);
			virtual					~CColor() {}

			static	u32				ToRGBA(cu8& uR, cu8& uG, cu8& uB, cu8& uA);

			static	CColor			FromRGBA(cu32& uiRGBA);

			static	CColor			FromFloats(cf32& fR, cf32& fG, cf32& fB, cf32& fA);

					u32				ToRGBA();

			static	CColor			Random();

			xst_fi	Math::CVector4	ToVector4() const
									{ return Math::CVector4( (f32)r / 255.0f, (f32)g / 255.0f, (f32)b / 255.0f, (f32)a / 255.0f ); }

			xst_fi	Math::CVector4	ToVector3() const
									{ return Math::CVector3( (f32)r / 255.0f, (f32)g / 255.0f, (f32)b / 255.0f ); }

		
	};

}//xst

#endif