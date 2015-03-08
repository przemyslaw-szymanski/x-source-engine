#ifndef XSE_CD3D11_SAMPLER_H
#define XSE_CD3D11_SAMPLER_H

#include "XSERenderSystemCommon.h"

// SAMPLER
namespace XSE
{
	namespace D3D11
	{
		enum class SamplerFilterBits : u32
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

		enum class SamplerWrapBits : u32
		{
			WRAP_U = 0x00000001 << 9,
			WRAP_V = WRAP_U << 10,
			WRAP_W = WRAP_U << 11,
			CLAMP_U = WRAP_U << 12,
			CLAMP_V = WRAP_U << 13,
			CLAMP_W = WRAP_U << 14,
			MIRROR_U = WRAP_U << 15,
			MIRROR_V = WRAP_U << 16,
			MIRROR_W = WRAP_U << 17,
		};

		enum class SamplerLODBits : u32
		{
			MIN_LEVEL0 = 0x00000001 << 18,
			MIN_LEVEL1 = MIN_LEVEL0 << 19,
			MIN_LEVEL2 = MIN_LEVEL0 << 20,
			MIN_LEVEL3 = MIN_LEVEL0 << 21,
			MIN_LEVEL4 = MIN_LEVEL0 << 22,
			MIN_LEVEL5 = MIN_LEVEL0 << 23,
			MAX_LEVEL0 = MIN_LEVEL0 << 24,
			MAX_LEVEL1 = MIN_LEVEL0 << 25,
			MAX_LEVEL2 = MIN_LEVEL0 << 26,
			MAX_LEVEL3 = MIN_LEVEL0 << 27,
			MAX_LEVEL4 = MIN_LEVEL0 << 28,
			MAX_LEVEL5 = MIN_LEVEL0 << 29,
			MAX_LEVEL_INF = MIN_LEVEL0 << 31
		};

		static const u32 SamplerMinFilterBits[] =
		{
			(u32)SamplerFilterBits::MIN_POINT,
			(u32)SamplerFilterBits::MIN_LINEAR,
			(u32)SamplerFilterBits::MIN_ANISO
		};

		static const u32 SamplerMagFilterBits[] =
		{
			(u32)SamplerFilterBits::MAG_POINT,
			(u32)SamplerFilterBits::MAG_LINEAR,
			(u32)SamplerFilterBits::MAG_ANISO
		};

		static const u32 SamplerMipFilterBits[] =
		{
			(u32)SamplerFilterBits::MIP_POINT,
			(u32)SamplerFilterBits::MIP_LINEAR,
			(u32)SamplerFilterBits::MIP_ANISO
		};

		static const u32 SamplerWrapUBits[] =
		{
			(u32)SamplerWrapBits::WRAP_U,
			(u32)SamplerWrapBits::CLAMP_U,
			(u32)SamplerWrapBits::MIRROR_U
		};

		static const u32 SamplerWrapVBits[] =
		{
			(u32)SamplerWrapBits::WRAP_V,
			(u32)SamplerWrapBits::CLAMP_V,
			(u32)SamplerWrapBits::MIRROR_V
		};

		static const u32 SamplerWrapWBits[] =
		{
			(u32)SamplerWrapBits::WRAP_W,
			(u32)SamplerWrapBits::CLAMP_W,
			(u32)SamplerWrapBits::MIRROR_W
		};

		static const u32 SamplerMinLevelBits[] =
		{
			(u32)SamplerLODBits::MIN_LEVEL0,
			(u32)SamplerLODBits::MIN_LEVEL1,
			(u32)SamplerLODBits::MIN_LEVEL2,
			(u32)SamplerLODBits::MIN_LEVEL3,
			(u32)SamplerLODBits::MIN_LEVEL4,
			(u32)SamplerLODBits::MIN_LEVEL5
		};

		static const u32 SamplerMaxLevelBits[] =
		{
			(u32)SamplerLODBits::MAX_LEVEL0,
			(u32)SamplerLODBits::MAX_LEVEL1,
			(u32)SamplerLODBits::MAX_LEVEL2,
			(u32)SamplerLODBits::MAX_LEVEL3,
			(u32)SamplerLODBits::MAX_LEVEL4,
			(u32)SamplerLODBits::MAX_LEVEL5,
			(u32)SamplerLODBits::MAX_LEVEL_INF
		};

		static u32 CalcSamplerId(const STextureSamplingMode& Mode)
		{
			u32 uId = 0;
			u32 uMinFilter = 0;
			u32 uMagFilter = 0;
			u32 uMipFilter = 0;
			u32 uAddrU = 0;
			u32 uAddrV = 0;
			u32 uAddrW = 0;
			u32 uMinLOD = 0;
			u32 uMaxLOD = 0;
			u32 uCount = 0;

			for( u32 mif = 0; mif < TextureFilters::_ENUM_COUNT; ++mif )
			{
				for( u32 maf = 0; maf < TextureFilters::_ENUM_COUNT; ++maf )
				{
					for( u32 mipf = 0; mipf < TextureFilters::_ENUM_COUNT; ++mipf )
					{
						for( u32 au = 0; au < TextureAddresses::_ENUM_COUNT; ++au )
						{
							for( u32 av = 0; av < TextureAddresses::_ENUM_COUNT; ++av )
							{
								for( u32 aw = 0; aw < TextureAddresses::_ENUM_COUNT; ++aw )
								{
									for( u32 lmin = 0; lmin < TextureLODs::_ENUM_COUNT; ++lmin )
									{
										for( u32 lmax = 0; lmax < TextureLODs::_ENUM_COUNT; ++lmax )
										{
											uCount++;
										}
									}
								}
							}
						}
					}
				}
			}

			return uId;
		}

	} // D3D11
} // XSE
#endif // XSE_CD3D11_SAMPLER_H