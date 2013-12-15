#include "XSECOctree.h"

namespace XSE
{
	class CEmptyOctListener : public IOctreeListener
	{

	};

	COctree::COctree(f32 fSize, const Vec3& vecCenterPos, IOctreeListener* pListener) : 
		m_pListener( pListener ), m_fSize( fSize ), m_byCurrDepth( 0 ), m_pParent( xst_null )
	{
		cf32 fHalfSize = m_fSize * 0.5f;
		m_vecMinCorner = vecCenterPos - fHalfSize;
		//m_vecMinCorner.y = vecCenterPos.y + fHalfSize;
		//m_vecMinCorner.z = vecCenterPos.z + fHalfSize;
		_Init();
	}

	COctree::COctree(f32 fSize, const Vec3& vecMinCorner, cu8& byCurrDepth, COctree* pParent) : 
		m_pListener( xst_null ), m_fSize( fSize ), m_byCurrDepth( byCurrDepth ), m_pParent( pParent ),
		m_vecMinCorner( vecMinCorner )
	{
		_Init();
	}

	void COctree::_Init()
	{
		xst_zero( m_apChildren, sizeof( COctree* ) * 8 );
		xst_zero( m_apNeighbours, sizeof( COctree* ) * 4 );
		
		if( m_pParent == xst_null )
		{
			if( m_pListener == xst_null ) //If user passed no his own listener create a default one
				m_pListener = xst_new CEmptyOctListener();
		}
		else
		{
			m_pListener = m_pParent->m_pListener;
		}
	}

	COctree::~COctree()
	{
		xst_delete( m_apChildren[ 0 ] );
		xst_delete( m_apChildren[ 1 ] );
		xst_delete( m_apChildren[ 2 ] );
		xst_delete( m_apChildren[ 3 ] );
		xst_delete( m_apChildren[ 4 ] );
		xst_delete( m_apChildren[ 5 ] );
		xst_delete( m_apChildren[ 6 ] );
		xst_delete( m_apChildren[ 7 ] );

		xst_delete( m_apNeighbours[ 0 ] );
		xst_delete( m_apNeighbours[ 1 ] );
		xst_delete( m_apNeighbours[ 2 ] );
		xst_delete( m_apNeighbours[ 3 ] );

		m_pParent = xst_null;
	}

	i32 COctree::AddObject(CObject* pObj, cu8& byMaxDepth)
	{
		//Create child nodes
		return _AddObject( this, pObj, byMaxDepth );
	}

	void COctree::RemoveObject(CObject* pObj)
	{
		m_vObjects.erase( xst_find( m_vObjects.begin(), m_vObjects.end(), pObj ) );
	}

	i32 COctree::ReorganizeObject(CObject* pObj, cu8& byMaxDepth)
	{
		return _ReorganizeObject( this, pObj, byMaxDepth );
	}

	COctree* COctree::GetNode(CObject* pObj)
	{
		ObjectVector::iterator Itr;
		return _GetNode( this, pObj, &Itr );
	}

	XST::CAABB	COctree::CalcAABB() const
	{
		XST::CAABB AABB;
		AABB.CreateFromLeftBottomFrontCorner( m_vecMinCorner, Vec3( m_fSize ) );
		return AABB;
	}

	XSE::CBoundingSphere COctree::CalcBoundingSphere() const
	{
		Vec3 vecCenter( CalcCenter() );
		cf32 fRadius = vecCenter.Distance( m_vecMinCorner );
		return CBoundingSphere( fRadius, vecCenter );
	}

	COctree* COctree::_CreateNode(COctree* pParent, f32 fSize, const Vec3& vecMinCorner, cu8& byMaxDepth)
	{
		if( pParent->m_byCurrDepth + 1 >= byMaxDepth )
			return xst_null;

		COctree* pNode = xst_new COctree( fSize, vecMinCorner, pParent->m_byCurrDepth + 1, pParent );
		pParent->m_pListener->OnAddNode( pNode );
		return pNode;
	}

	i32 COctree::_CalcNodeIDForObject(const COctree* pNode, CObject* pObj, XST::CAABB* pChildAABB)
	{
		//Calc center
		const Vec3 vecCenter( pNode->CalcCenter() );
		//Determine the node part based on object position
		//const Vec3& vecObjPos = pObj->GetObjectPosition();
		const CBoundingVolume& ObjVolume = pObj->GetObjectBoundingVolume();
		const Vec4& vecObjPos = ObjVolume.GetSphere().vecCenter;

		//bool bRightPart = ( vecObjPos.x > vecCenter.x );
		//bool bTopPart = ( vecObjPos.y > vecCenter.y );
		//bool bFarPart = ( vecObjPos.z > vecCenter.z );

		Vec3 vecMin;
		Vec3 vecMax;
		f32 fChildSize = pNode->m_fSize * 0.5f;
		u32 uiChildID = 0; //child position in the array
		
		/*
		0 - left & bottom & front / left bottom front
		1 - right & bottom & front / right bottom front
		2 - top & left & front / left top front 
		3 - right & top & front / right top front
		4 - back & left & bottom / left bottom back
		5 - right & back & bottom / right bottom back
		6 - top & back & left / left top back
		7 - right & top & back / right top back
		
		1 - right
		2 - top
		4 - back
		3 = 1 | 2 | 0
		5 = 1 | 4 | 0
		6 = 2 | 4 | 0
		*/

		//If right part of this node
		if( vecObjPos.x > vecCenter.x )
		{
			vecMin.x = vecCenter.x;
			vecMax.x = vecMin.x + fChildSize;
			uiChildID |= 1;
		}
		else //left part
		{
			vecMax.x = vecCenter.x;
			vecMin.x = vecMax.x - fChildSize;
		}

		//If top part of this node
		if( vecObjPos.y > vecCenter.y )
		{
			vecMin.y = vecCenter.y;
			vecMax.y = vecMin.y + fChildSize;
			uiChildID |= 2;
		}
		else //bottom part
		{
			vecMax.y = vecCenter.y;
			vecMin.y = vecMax.y - fChildSize;
		}

		//If back part
		if( vecObjPos.z > vecCenter.z )
		{
			vecMin.z = vecCenter.z;
			vecMax.z = vecCenter.z + fChildSize;
			uiChildID |= 4;
		}
		else //front part
		{
			vecMax.z = vecCenter.z;
			vecMin.z = vecCenter.z - fChildSize;
		}

		//Check if object fits to this part (by bounding sphere, then by AABB if needed)
		//If not fits add it to this node
		//Build AABB for this child node
		//XST::CAABB ChildAABB( vecMin, vecMax );
		pChildAABB->vecMin = vecMin;
		pChildAABB->vecMax = vecMax;

		if( !pChildAABB->Includes( ObjVolume.GetAABB() ) )
			return -1;

		return uiChildID;
	}

	COctree* COctree::_CalcNodeForObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth)
	{
		XST::CAABB ChildAABB;
		i32 iChildID = _CalcNodeIDForObject( pNode, pObj, &ChildAABB );
		if( iChildID >= 0 )
		{
			COctree* pChildNode = pNode->m_apChildren[ iChildID ];
			if( pChildNode == xst_null )
			{
				cf32 fChildSize = pNode->m_fSize * 0.5f;
				pChildNode = pNode->_CreateNode( pNode, fChildSize, ChildAABB.vecMin, byMaxDepth );
				pNode->m_apChildren[ iChildID ] = pChildNode;
			}

			return pChildNode;
		}
		else
		{
			return xst_null;
		}

		return xst_null;
	}

	i32 COctree::_AddObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth)
	{
		if( strcmp( pObj->_GetDbgName(), "Terrain_1_1" ) == 0 )
		{
			int i = 0;
		}

		if( pNode->m_byCurrDepth >= byMaxDepth )
		{
			pNode->_AddObject( pObj );
			return Results::MAX_DEPTH_REACHED;
		}

		//Check if this object fits to this node
		//const Vec3& vecObjPos = pObj->GetObjectPosition();
		const CBoundingVolume& ObjVolume = pObj->GetObjectBoundingVolume();
		const Vec3& vecObjPos = ObjVolume.GetSphere().GetCenterPoint();

		//Specific case, AABB has size == 0, add to this node
		if( ObjVolume.GetAABB().IsZero() )
		{
			return Results::NOT_IN_NODE;
		}

		//Check whether this object fits to this node
		const XST::CAABB NodeAABB( pNode->CalcAABB() );
		if( !NodeAABB.Includes( ObjVolume.GetAABB() ) )
		{
			return Results::NOT_IN_NODE; //if not fits do not add any child nodes
		}

		COctree* pChildNode = _CalcNodeForObject( pNode, pObj, byMaxDepth );
		//If node is null put object in this node, otherwise put it in child node
		if( pChildNode == xst_null )
		{
			pNode->_AddObject( pObj );
		}
		else
		{
			i32 iResult = _AddObject( pChildNode, pObj, byMaxDepth );
			if( iResult == Results::NOT_IN_NODE ) //if this object does not fit to this child node add to the parent node
			{
				pNode->_AddObject( pObj );
				return XST_OK;
			}

			return iResult;
		}

		return XST_OK;
	}

	COctree* COctree::_GetNode(COctree* pNode, CObject* pObj, ObjectVector::iterator* pItrOut)
	{
		if( pNode == xst_null )
			return xst_null; //this node does not exists so no object found here

		XST::CAABB ChildAABB;
		i32 iNodeID = _CalcNodeIDForObject( pNode, pObj, &ChildAABB );
		if( iNodeID < 0 )
		{
			(*pItrOut) = xst_find_ex( pNode->m_vObjects, pObj );
			if( (*pItrOut) == pNode->m_vObjects.end() )
				return xst_null; //not found anywhere
			return pNode; //found in this node
		}
		//If not found in main node, try to find in child node
		COctree* pChildNode = pNode->m_apChildren[ iNodeID ];
		return _GetNode( pChildNode, pObj, pItrOut );
	}

	void COctree::_RemoveObject(ObjectVector::iterator& Itr)
	{
		(*Itr) = m_vObjects.back();
		m_vObjects.pop_back();
	}

	i32 COctree::_ReorganizeObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth)
	{
		ObjectVector::iterator ObjItr;
		COctree* pDstNode = _GetNode( pNode, pObj, &ObjItr );
		//If node not found/obj not exists in any node add it
		if( pDstNode == xst_null )
		{
			return _AddObject( pDstNode, pObj, byMaxDepth );
		}

		//If node found so obj exists in this node, check if it still fits
		if( !pDstNode->CalcAABB().Includes( pObj->GetObjectBoundingVolume().GetAABB() ) )
		{
			//If not fits remove it from this node
			pDstNode->_RemoveObject( ObjItr ); 
		}

		return XST_OK;
	}

	void COctree::_AddObject(CObject* pObj)
	{
		m_vObjects.push_back( pObj );
		m_pListener->OnAddObject( this, pObj );
	}

}//xse