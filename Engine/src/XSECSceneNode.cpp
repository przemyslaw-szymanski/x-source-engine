#include "XSECSceneNode.h"
#include "XSECSceneManager.h"
#include "XSECStaticGeometry.h"
#include "XSEIScenePartitionSystem.h"

#pragma push_macro( "GetObject" )
#undef GetObject

namespace XSE
{
	CSceneNode::CSceneNode(CSceneManager* pCreator, CSceneNode* pParent, xst_castring& strName, ul32 ulHandle) : CObject( OT::SCENE_NODE, strName.c_str() ), 
	m_pSceneMgr( pCreator ), m_pParentNode( pParent ), m_bAutoDestroy( true ), m_strName( strName ), m_ulObjHandle( ulHandle )
	{
	}

	CSceneNode::~CSceneNode()
	{
		DestroyChildren();
	}

	void CSceneNode::DestroyChildren()
	{
		for(_NodeIterator Itr = m_lChildNodes.begin(); Itr != m_lChildNodes.end(); ++Itr)
		{
			xst_delete( (*Itr) );
		}

		m_lChildNodes.clear();
		//m_mChildNodes.clear();
	}

	void CSceneNode::Destroy()
	{
		if( m_pParentNode )
			m_pParentNode->DestroyChildNode( this );
	}

	CSceneNode::NodeIterator CSceneNode::GetChildNodeIterator()
	{
		return NodeIterator( &m_lChildNodes );
	}

	/*CSceneNode::MapIterator CSceneNode::GetChildNodeMapIterator()
	{
		return MapIterator( &m_mChildNodes );
	}*/

	void CSceneNode::Reorganize()
	{
		ObjectVector::iterator Itr;
		CObject* pObj;

		xst_stl_foreach( Itr, m_vObjects )
		{
			pObj = (*Itr);
			if( !pObj->IsDirty() )
				continue;

			//Reorganize object in the scene partition system
			m_pSceneMgr->GetScenePartitionSystem()->ReorganizeObject( pObj );
			
			pObj->IsDirty( false );
		}
	}

	void CSceneNode::ReorganizeObject(CObject* pObj)
	{
		if( !pObj->IsDirty() )
			return;
		m_pSceneMgr->GetScenePartitionSystem()->ReorganizeObject( pObj );
		pObj->IsDirty( false );
	}

	CSceneNode* CSceneNode::CreateChildNode(xst_castring &strName, bool bAutoDestroy)
	{
		//Check if node exists
		ul32 ulHandle = XST::CHash::GetCRC( strName );
		CSceneNode* pNode = GetChildNode( ulHandle );
		if( pNode )
		{
			return pNode;
		}

		pNode = xst_new CSceneNode( m_pSceneMgr, this, strName, ulHandle );
		if( pNode )
		{
			pNode->m_bAutoDestroy = bAutoDestroy;
			//m_mChildNodes.insert( NodeMap::value_type( pNode->GetHandle(), pNode ) );
			m_lChildNodes.push_back( pNode );
		}

		return pNode;
	}

	CSceneNode* CSceneNode::CreateChildNode(bool bAutoDestroy)
	{
		return CreateChildNode( this->m_strName + XST::StringUtil::ToString( XST::CTime::GetQPerfTickCount() ), bAutoDestroy );
	}

	CSceneNode* CSceneNode::RemoveChildNode(xst_castring& strName)
	{
		return RemoveChildNode( XST::CHash::GetCRC( strName ) );
	}

	CSceneNode* CSceneNode::RemoveChildNode(CSceneNode* pNode)
	{
		return RemoveChildNode( pNode->GetObjectHandle() );
	}

	CSceneNode* CSceneNode::RemoveChildNode(cul32& ulHandle)
	{
		//Get child node
		CSceneNode* pNode = GetChildNode( ulHandle );
		if( pNode != xst_null )
		{
			std::remove( m_lChildNodes.begin(), m_lChildNodes.end(), pNode ); 
		}
		else
		{
			XST_LOG_ERR( "Node with handle: " << ulHandle << " does not exists" );
		}

		return pNode;
	}

	i32 CSceneNode::DestroyChildNode(xst_castring& strName)
	{
		return DestroyChildNode( XST::CHash::GetCRC( strName ) );
	}

	i32 CSceneNode::DestroyChildNode(CSceneNode* pNode)
	{
		return DestroyChildNode( pNode->GetObjectHandle() );
	}

	i32 CSceneNode::DestroyChildNode(cul32& ulHandle)
	{
		CSceneNode* pNode = RemoveChildNode( ulHandle );
		if( !pNode )
		{
			return RESULT::FAILED;
		}

		xst_delete( pNode );

		return RESULT::OK;
	}

	CSceneNode::_NodeIterator CSceneNode::_FindNode(cul32& ulHandle)
	{
		for(_NodeIterator Itr = m_lChildNodes.begin(); Itr != m_lChildNodes.end(); ++Itr)
		{
			if( (*Itr)->GetObjectHandle() == ulHandle )
			{
				return Itr;
			}
		}

		return m_lChildNodes.end();
	}

	void CSceneNode::Update(cf32& fElapsedTime)
	{
		for(_NodeIterator Itr = m_lChildNodes.begin(); Itr != m_lChildNodes.end(); ++Itr)
		{
			(*Itr)->Update( fElapsedTime );
		}
	}

	CSceneNode* CSceneNode::GetChildNode(xst_castring &strName)
	{
		return GetChildNode( XST::CHash::GetCRC( strName ) );
	}

	CSceneNode* CSceneNode::GetChildNode(cul32 &ulHandle)
	{
		/*NodeMap::iterator Itr = m_mChildNodes.find( ulHandle );
		if( Itr == m_mChildNodes.end() )
		{
			return xst_null;
		}

		return Itr->second;*/
		for(_NodeIterator Itr = m_lChildNodes.begin(); Itr != m_lChildNodes.end(); ++Itr)
		{
			if( (*Itr)->GetObjectHandle() == ulHandle )
			{
				return (*Itr);
			}
		}
		return xst_null;
	}

	CObject* CSceneNode::GetObject(xst_castring& strName) const
	{
		ul32 ulHandle = XST::CHash::GetCRC( strName );
		return GetObject( ulHandle );
	}

	CObject* CSceneNode::GetObject(cul32& ulHandle) const
	{
		ObjectVector::const_iterator Itr;
		xst_stl_foreach( Itr, m_vObjects )
		{
			if( (*Itr)->GetObjectHandle() == ulHandle )
			{
				return (*Itr);
			}
		}
		return xst_null;
	}

	i32 CSceneNode::AddObject(CObject* pObject)
	{
		xst_assert( pObject, "" );
		ObjectMap::iterator Itr;
		ul32 ulId = pObject->GetObjectHandle();//XST::CHash::GetCRC( pObject->GetObjectName() );
		//if( !XST_FAILED( XST::MapUtils::Insert( m_mObjects, ulId, pObject, &Itr ) ) )
		i32 iResult = XST_OK;
		if( GetObject( pObject->GetObjectHandle() ) != xst_null )
		{
			XST_LOG_ERR( "Object: " << XST_GET_DBG_NAME( pObject ) << " is already in scene node: " << XST_GET_DBG_NAME( this ) );
			return XST_FAIL;
		}

		{
			switch( pObject->GetObjectType() )
			{
				case OT::TERRAIN:
				case OT::MESH: 
				case OT::MODEL: 
				case OT::STATIC_GEOMETRY: 
				case OT::DYNAMIC_GEOMETRY:
				case OT::DYNAMIC_GEOMETRY_GROUP:
				case OT::STATIC_GEOMETRY_GROUP:
				{
					IRenderableObject* pObj = (IRenderableObject*)pObject;
					pObj->_SetSceneNode( this );
					m_pSceneMgr->_AddObject( pObj );
				}
				break;

				case OT::CAMERA:
				{

				}
				break;

				case OT::SCENE_NODE:
				{
					return this->AddChildNode( (CSceneNode*)pObject );
				}
				break;

				default: 
				{
					XST_LOG_ERR( "Unknown renderable object type: " << pObject->GetObjectType() );
					return XST_FAIL;
				}
				break;
			}
			m_vObjects.push_back( pObject );
			return XST_OK;
		}
		
		XST_LOG_ERR( "Unable to add renderable object: " << XST_GET_DBG_NAME( pObject ) << " to the scene node: " << XST_GET_DBG_NAME( this ) );
		return XST_FAIL;
	}

	i32 CSceneNode::AddObject(RenderableObjectPtr pObject)
	{
		xst_assert( !pObject.IsNull(), "(CSceneNode::AddObject) Object is nul" );

		IRenderableObject* pObj = pObject.GetPtr();
		pObj->_SetSceneNode( this );;
		return AddObject( pObj );
	}

	i32 CSceneNode::RemoveObject(RenderableObjectPtr pObject)
	{
		return RemoveObject( pObject.GetPtr() );
	}

	i32 CSceneNode::RemoveObject(const CObject* pObject)
	{
		ObjectVector::iterator Itr = std::find( m_vObjects.begin(), m_vObjects.end(), pObject );
		if( Itr == m_vObjects.end() )
		{
			return XST_FAIL;
		}
		m_vObjects.erase( Itr );
		return XST_OK;
	}

	i32 CSceneNode::AddChildNode(XSE::CSceneNode *pNode)
	{
		m_lChildNodes.push_back( pNode );
		pNode->m_pParentNode = this;
		return XST_OK;
	}

	void CSceneNode::SetPosition(cf32& fX, cf32& fY, cf32& fZ)
	{
		CObject::SetPosition( fX, fY, fZ );
		for( auto& pObj : m_vObjects )
		{
			pObj->IsDirty( true );
		}
	}

}//xse

#pragma pop_macro( "GetObject" )