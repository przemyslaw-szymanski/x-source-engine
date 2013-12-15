#ifndef XST_CRANDOM_H
#define XST_CRANDOM_H

#include "XSTCommon.h"
#include "XSTCTime.h"
#include "mtrand.h"

namespace XST
{
	using namespace Types;

	class CRandom
	{
		public:

			xst_fi CRandom() //: m_IRand( MTRand_int32( XST::CTime::GetTickCount() ) ), m_DRand( MTRand( XST::CTime::GetTickCount() ) )
			{
				Randomize();
			}

			xst_fi void Randomize()
			{
				SetSeed( XST::CTime::GetTickCount() );
			}

			xst_fi void SetSeed(cul32& ulSeed)
			{
				m_IRand.seed( ulSeed );
			}

			xst_fi i32 GetNextInt()
			{
				return m_IRand();
			}

			xst_fi i32 GetNextInt(ci32& iMax)
			{
				return GetNextInt() % iMax;
			}

			xst_fi i32 GetNextIntInRange(ci32& iMin, ci32& iMax)
			{
				return ( GetNextInt() % ( iMax -  iMin + 1 ) ) + iMin;
			}

			xst_fi f32 GetNextFloat()
			{
				return (f32)( GetNextInt() % 0xffffffff );
			}

			xst_fi f32 GetNextFloat(cf32& fMax, cu32& uiDiv = 1000)
			{
				return (f32)( GetNextInt() % (i32)( fMax *  uiDiv + 1) ) / uiDiv;
			}

			xst_fi f32 GetNextFloatInRange(cf32& fMin, cf32& fMax, cu32& uiDiv = 1000)
			{
				return (f32)( GetNextInt() % (i32)( ( fMax - fMin ) * uiDiv + 1) ) / uiDiv + fMin;
			}

		private:

			MTRand_int32	m_IRand;
	};

	class Random
	{
		public:

			static void	Randomize()
			{
				srand( XST::CTime::GetTickCount() );
			}

			static i32	GetNextInt()
			{
				return rand() % RAND_MAX;
			}

			static i32	GetNextInt(const i32& _iMax)
			{
				return rand() % _iMax + 1;
			}

			static i32	GetNextIntInRange(const i32& _iMin, const i32& _iMax)
			{
				return (rand() % (_iMax - _iMin + 1 ) ) + _iMin;
			}

			static f32	GetNextFloat()
			{
				return rand() / (f32)RAND_MAX;
			}

			static f32	GetNextFloat(const f32& _fMax, const u32& _uiDiv = 1000)
			{
				return (f32)( rand() % (i32)(_fMax * _uiDiv + 1) ) / _uiDiv;
			}

			static f32	GetNextFloatInRange(const f32& _fMin, const f32& _fMax, const u32& _uiDiv = 1000)
			{
				return (f32)( rand() % (i32)( (_fMax - _fMin) * _uiDiv + 1) ) / _uiDiv + _fMin;
			}
	};

}//XST

#endif