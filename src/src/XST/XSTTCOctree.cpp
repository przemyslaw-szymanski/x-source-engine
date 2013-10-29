#include "../../include/xst/XSTTCOctree.h"

namespace XST
{
	using namespace Types;
	using namespace Math;

	TCOctree::TCOctree() : 
		m_pParent( xst_null ) 
	{
		m_apChildren[ 0 ] = xst_null;
		m_apChildren[ 1 ] = xst_null;
		m_apChildren[ 2 ] = xst_null;
		m_apChildren[ 3 ] = xst_null;
		m_apChildren[ 4 ] = xst_null;
		m_apChildren[ 5 ] = xst_null;
		m_apChildren[ 6 ] = xst_null;
		m_apChildren[ 7 ] = xst_null;
	}

	TCOctree::~TCOctree()
	{
		DestroyChildren();
	}

	void TCOctree::DestroyChildren()
	{
		for(u32 i = 8; i --> 0;)
		{
			xst_delete( m_apChildren[ i ] );
		}
	}

	i32 TCOctree::Create(cu16& usDepth, cf32& fSize, const Vec3& vecPosition)
	{
		if( m_apChildren[ 0 ] != xst_null )
		{
			DestroyChildren();
		}

		m_usDepth = usDepth;
		m_Bounds.Create( fSize, vecPosition );
		m_pParent = xst_null; 
		
		Vec3 vecCenter = m_Bounds.CalcCenter();

		//FRONT
		Vec3 vecPos = Vec3( vecCenter.x - fSize, vecCenter.y - fSize, vecCenter.z - fSize );
		m_apChildren[ LEFT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_FRONT ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y - fSize, vecCenter.z - fSize );
		m_apChildren[ RIGHT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_FRONT ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y + fSize, vecCenter.z - fSize );
		m_apChildren[ LEFT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_FRONT ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y + fSize, vecCenter.z - fSize );
		m_apChildren[ RIGHT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_FRONT ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		//BACK
		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y - fSize, vecCenter.z + fSize );
		m_apChildren[ LEFT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_BACK ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y - fSize, vecCenter.z + fSize );
		m_apChildren[ RIGHT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_BACK ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y + fSize, vecCenter.z + fSize );
		m_apChildren[ LEFT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_BACK ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y + fSize, vecCenter.z + fSize );
		m_apChildren[ RIGHT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_BACK ]->_Create( usDepth, usDepth - 1, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;
		
		return XST_OK;
	}

	i32 TCOctree::_Create(cu16& usDepth, cu16& usCurrentDepth, cf32& fCurrentSize, const Vec3& vecPosition, Node* pParent)
	{
		m_pParent = pParent;
		m_Bounds.Create( fCurrentSize, vecPosition );
		m_usDepth = usCurrentDepth;

		if( usCurrentDepth > usDepth )
		{
			return XST_OK;
		}

		Vec3 vecCenter = m_Bounds.CalcCenter();
		//FRONT
		Vec3 vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ LEFT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_FRONT ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ RIGHT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_FRONT ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ LEFT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_FRONT ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ RIGHT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_FRONT ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		//BACK
		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ LEFT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_BACK ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ RIGHT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_BACK ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ LEFT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_BACK ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ RIGHT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_BACK ]->_Create( usDepth, m_usDepth - 1, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		return XST_OK;
	}

	i32 TCOctree::Create(cf32& fMinSize, cf32& fSize, const Vec3& vecPosition)
	{
		/*if( m_apChildren[ 0 ] != xst_null )
		{
			DestroyChildren();
		}*/

		m_usDepth = 0;
		m_Bounds.Create( fSize, vecPosition );
		m_pParent = xst_null; 
		
		if( fSize < fMinSize )
		{
			return XST_OK;
		}

		Vec3 vecCenter = m_Bounds.CalcCenter();

		//FRONT
		cf32 fHalfSize = fSize * 0.5f;
		Vec3 vecPos = Vec3( vecCenter.x - fHalfSize, vecCenter.y - fHalfSize, vecCenter.z - fHalfSize );
		m_apChildren[ LEFT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_FRONT ]->_Create( fMinSize, fHalfSize, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y - fSize, vecCenter.z - fSize );
		m_apChildren[ RIGHT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_FRONT ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y + fSize, vecCenter.z - fSize );
		m_apChildren[ LEFT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_FRONT ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y + fSize, vecCenter.z - fSize );
		m_apChildren[ RIGHT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_FRONT ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		//BACK
		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y - fSize, vecCenter.z + fSize );
		m_apChildren[ LEFT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_BACK ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y - fSize, vecCenter.z + fSize );
		m_apChildren[ RIGHT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_BACK ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fSize, vecCenter.y + fSize, vecCenter.z + fSize );
		m_apChildren[ LEFT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_BACK ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fSize, vecCenter.y + fSize, vecCenter.z + fSize );
		m_apChildren[ RIGHT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_BACK ]->_Create( fMinSize, fSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;
		
		return XST_OK;
	}

	i32 TCOctree::_Create(cf32& fMinSize, cf32& fCurrentSize, const Vec3& vecPosition, Node* pParent)
	{
		m_pParent = pParent;
		m_Bounds.Create( fCurrentSize, vecPosition );
		if( m_pParent )
		{
			m_usDepth = m_pParent->m_usDepth + 1;
		}
		else
		{
			m_usDepth = 0;
		}

		if( fCurrentSize < fMinSize )
		{
			return XST_OK;
		}

		Vec3 vecCenter = m_Bounds.CalcCenter();

		//FRONT
		cf32 fHalfSize = fCurrentSize * 0.5f;
		Vec3 vecPos = Vec3( vecCenter.x - fHalfSize, vecCenter.y - fHalfSize, vecCenter.z - fHalfSize );
		m_apChildren[ LEFT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_FRONT ]->_Create( fMinSize, fHalfSize, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ RIGHT_BOTTOM_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_FRONT ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ LEFT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_FRONT ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z - fCurrentSize );
		m_apChildren[ RIGHT_TOP_FRONT ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_FRONT ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_FRONT ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		//BACK
		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ LEFT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_BOTTOM_BACK ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y - fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ RIGHT_BOTTOM_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_BOTTOM_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_BOTTOM_BACK ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x - fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ LEFT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ LEFT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ LEFT_TOP_BACK ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;

		vecPos = Vec3( vecCenter.x + fCurrentSize, vecCenter.y + fCurrentSize, vecCenter.z + fCurrentSize );
		m_apChildren[ RIGHT_TOP_BACK ] = xst_new Node(); if( m_apChildren[ RIGHT_TOP_BACK ] == xst_null ) return XST_FAIL;
		if( XST_FAILED( m_apChildren[ RIGHT_TOP_BACK ]->_Create( fMinSize, fCurrentSize * 0.5f, vecPos, this ) ) ) return XST_FAIL;
		
		return XST_OK;
	}
}//xst