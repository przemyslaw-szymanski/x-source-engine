#ifndef XST_ALGORITHM_H
#define XST_ALGORITHM_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"

namespace XST
{
	using namespace Types;

	class Algorithm
	{
		public:

			template< class _T_ >
			static void QuickSort(_T_* input, i32 iLeft, i32 iRight)
			{ return QuickSort1( input, iLeft, iRight, _DefaultCompare< _T_ > ); }

			template< class _T_, class _CMP_FUNC_ >
			static void QuickSort1(_T_* input, i32 iLeft, i32 iRight, const _CMP_FUNC_& Cmp);

			template< class _T_, class _CMP_FUNC_ >
			static i32 QuickSortPartition1(_T_* input, i32 iLeft, i32 iRight, const _CMP_FUNC_& Cmp);

			template< class _T_ >
			static i32 QuickSortPartition(_T_* input, i32 iLeft, i32 iRight)
			{ return QuickSortPartition1( input, iLeft, iRight, _DefaultCompare< _T_ > ); }

		private:

			template< class _T_ >
			static xst_fi i32	_DefaultCompare(const _T_& tLeft, const _T_& tRight)
			{
				if( tLeft == tRight ) return 0;
				else if( tLeft < tRight ) return -1;
				else return 1;
			}

	};

	//// The quicksort recursive function
	//template< class _T_ >
	//void Algorithm::QuickSort(_T_* input, i32 iLeft, i32 iRight)
	//{
	//	if ( iLeft < iRight )
	//	{
	//		i32 j = QuickSortPartition< _T_, _CMP_FUNC_ >( input, iLeft, iRight );        
	//		QuickSort1(input, iLeft, j-1, Cmp);
	//		QuickSort1(input, j+1, iRight, Cmp);
	//	}
	//}	

	//// The partition function
	//template< class _T_ >
	//i32 Algorithm::QuickSortPartition(_T_* input, i32 iLeft, i32 iRight)
	//{
	//	_T_& pivot = input[iRight];

	//	while ( iLeft < iRight )
	//	{
	//		while ( input[iLeft] < pivot ) iLeft++;

	//		while ( input[iRight] > pivot ) iRight--;

	//		if ( input[iLeft] == input[iRight] ) iLeft++;
	//		else if ( iLeft < iRight )
	//		{
	//			_T_ tmp = input[iLeft];
	//			input[iLeft] = input[iRight];
	//			input[iRight] = tmp;
	//		}
	//	}

	//	return iRight;
	//}


	// The quicksort recursive function
	template< class _T_, class _CMP_FUNC_ >
	void Algorithm::QuickSort1(_T_* input, i32 iLeft, i32 iRight, const _CMP_FUNC_& Cmp)
	{
		if ( iLeft < iRight )
		{
			i32 j = QuickSortPartition1< _T_, _CMP_FUNC_ >(input, iLeft, iRight, Cmp);        
			QuickSort1(input, iLeft, j-1, Cmp);
			QuickSort1(input, j+1, iRight, Cmp);
		}
	}	

	// The partition function
	template< class _T_, class _CMP_FUNC_ >
	i32 Algorithm::QuickSortPartition1(_T_* input, i32 iLeft, i32 iRight, const _CMP_FUNC_& Cmp)
	{
		_T_& pivot = input[iRight];

		while ( iLeft < iRight )
		{
			while( Cmp( input[ iLeft ], pivot ) < 0 )
				iLeft++;

			while( Cmp( input[ iRight ], pivot ) > 0 )
				iRight--;

			if( Cmp( input[ iLeft ], input[ iRight ] ) == 0 )
				iLeft++;
			else if ( iLeft < iRight )
			{
				_T_ tmp = input[iLeft];
				input[iLeft] = input[iRight];
				input[iRight] = tmp;
			}
		}

		return iRight;
	}

}//xst

#endif