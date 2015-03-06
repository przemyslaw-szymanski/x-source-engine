#ifndef XSE_CD3D11_SAMPLER_H
#define XSE_CD3D11_SAMPLER_H

// SAMPLER
namespace XSE
{
	namespace D3D11
	{
		enum class SamplerFilterBits
		{
			MIN_POINT = 0x00000001,
			MAG_POINT = MIN_POINT << 1,
			MIP_POINT = MIN_POINT << 2,
			MIN_LINEAR = MIN_POINT << 3,
			MAG_LINEAR = MIN_POINT << 4,
			MIP_LINEAR = MIN_POINT << 5,
			MIN_ANISO = MIN_POINT << 6,
			MAG_ANISO = MIN_POINT << 7,
			MIP_ANISO = MIN_POINT << 8,
		};

		enum class SamplerWrapBits
		{
			WRAP_U = 0x00000001 << 9,
			WRAP_V = WRAP_U << 10,
			WRAP_W = WRAP_U << 11,
			CLAMP_U = WRAP_U << 12,
			CLAMP_V = WRAP_U << 13,
			CLAMP_W = WRAP_U << 14,
			MIRROR_U = WRAP_U << 15,
			MIRROR_V = WRAP_U << 16,
			MIRRRO_W = WRAP_U << 17,
		};

		enum class SamplerLevelBits
		{
			MIN_LEVEL0 = 0x00000001 << 18,
			MIN_LEVEL1 = MIN_LEVEL0 << 19,
			MIN_LEVEL2 = MIN_LEVEL0 << 20,
			MIN_LEVEL3 = MIN_LEVEL0 << 21,
			MIN_LEVEL4 = MIN_LEVEL0 << 22,
			MIN_LEVEL5 = MIN_LEVEL0 << 23,
			MAX_LEVE05 = MIN_LEVEL0 << 24,
			MAX_LEVEL1 = MIN_LEVEL0 << 25,
			MAX_LEVEL2 = MIN_LEVEL0 << 26,
			MAX_LEVEL3 = MIN_LEVEL0 << 27,
			MAX_LEVEL4 = MIN_LEVEL0 << 28,
			MAX_LEVEL5 = MIN_LEVEL0 << 29,
			MAX_LEVEL_INF = MIN_LEVEL0 << 31
		};

		static const u32 SamplerMinFilterBits[] =
		{
			MIN_POINT,
			MIN_LINEAR,
			MIN_ANISOTROPIC
		};

		static const u32 SamplerMagFilterBits[] =
		{
			MAG_POINT,
			MAG_LINEAR,
			MAG_ANISOTROPIC
		};

		static const u32 SamplerMipFilterBits[] =
		{
			MIP_POINT,
			MIP_LINEAR,
			MIP_ANISOTROPIC
		};

		static const u32 SamplerWrapUBits[] =
		{
			WRAP_U,
			CLAMP_U,
			MIRROR_U
		};

		static const u32 SamplerWrapVBits[] =
		{
			WRAP_V,
			CLAMP_V,
			MIRROR_V
		};

		static const u32 SamplerWrapWBits[] =
		{
			WRAP_W,
			CLAMP_W,
			MIRROR_W
		};

		static const u32 SamplerMinLevelBits[] =
		{
			MIN_LEVEL0,
			MIN_LEVEL1,
			MIN_LEVEL2,
			MIN_LEVEL3,
			MIN_LEVEL4,
			MIN_LEVEL5
		};

		static const u32 SamplerMaxLevelBits[] =
		{
			MAX_LEVEL0,
			MAX_LEVEL1,
			MAX_LEVEL2,
			MAX_LEVEL3,
			MAX_LEVEL4,
			MAX_LEVEL5,
			MAX_INF
		};
	} // D3D11
} // XSE
#endif // XSE_CD3D11_SAMPLER_H