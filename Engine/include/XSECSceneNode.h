#ifndef XSE_CSCENE_NODE_H
#define XSE_CSCENE_NODE_H

#include "XSECObject.h"
#include "XSEIRenderableObject.h"

//Disable GetObject from winapi
//#if defined (XST_WINDOWS)
//#	if defined GetObject
//#		undef GetObject
//#	endif //if defined GetObject
//#endif //XST_WINDOWS
#pragma push_macro( "GetObject" )
#undef GetObject

namespace XSE
{
	class XST_API CSceneNode : public CObject
	{
		friend class CSceneManager;

		protected:

			typedef CObject*							ObjectPtr;
			typedef xst_vector< CSceneNode* >			NodeList;
			//typedef xst_map< ul32, CSceneNode* >		NodeMap;
			typedef xst_map< ul32, ObjectPtr >			ObjectMap;
			typedef xst_vector< ObjectPtr >				ObjectVector;
			typedef NodeList::iterator					_NodeIterator;
			typedef ObjectMap::iterator					_ObjectIterator;

		public:

			typedef XST::TCIterator< NodeList >			NodeIterator;
			//typedef XST::TCPairIterator< NodeMap >		NodeIterator;
			typedef XST::TCPairIterator< ObjectMap >	ObjectIterator;

			using CObject::SetPosition;
			using CObject::SetDirection;
			using CObject::SetScale;
			using CObject::Rotate;
			using CObject::SetOrientation;

		public:

									CSceneNode(CSceneManager* pCreator, CSceneNode* pParent, xst_castring& strName, ul32 ulHandle);
			virtual					~CSceneNode();

					NodeIterator	GetChildNodeIterator();

					//MapIterator		GetChildNodeMapIterator();

					ObjectIterator	GetObjectIterator();

					CSceneNode*		CreateChildNode(xst_castring& strName, bool bAutoDestroy = true);
					CSceneNode*		CreateChildNode(bool bAutoDestroy = true);

					CSceneNode*		RemoveChildNode(xst_castring& strName);
					CSceneNode*		RemoveChildNode(CSceneNode* pNode);
					CSceneNode*		RemoveChildNode(cul32& ulHandle);

					CSceneNode*		GetChildNode(xst_castring& strName);
					CSceneNode*		GetChildNode(cul32& ulHandle);

					i32				DestroyChildNode(xst_castring& strName);
					i32				DestroyChildNode(cul32& ulHandle);
					i32				DestroyChildNode(CSceneNode* pNode);

					i32				AddObject(RenderableObjectWeakPtr pObject, bool bAddToScenePartition = true);
					i32				AddUniqueObject(RenderableObjectWeakPtr pObject, bool bAddToScenePartition = true);
					i32				AddObject(CObject* pObject, bool bAddToScenePartition = true);
					i32				AddUniqueObject(CObject* pObject, bool bAddToScenePartition = true);

					i32				RemoveObject(RenderableObjectWeakPtr pObject);
					i32				RemoveObject(const CObject* pObject);

					void			Reorganize();

					void			ReorganizeObject(CObject* pObj);

					CObject*		GetObject(xst_castring& strName) const;
					CObject*		GetObject(cul32& ulHandle) const;

					i32				AddChildNode(CSceneNode* pNode);

					void			DestroyChildren();

					void			Destroy(); //destroy itself

					void			ReserveObjects(cu32& uiCount);

			xst_fi	bool			IsEmpty() const
									{ return m_vObjects.size() == 0 && m_lChildNodes.size() == 0; }

			/*xst_fi	CSceneManager*	GetSceneManager() const
									{ return m_pSceneMgr; }

			xst_fi const CSceneManager* GetSceneManager() const
										{ return m_pSceneMgr; }*/

			virtual	void			Update(cf32& fElapsedTime) override;

			virtual	void			SetPosition(cf32& fX, cf32& fY, cf32& fZ);

			xst_fi	bool			IsAutoDestroy()
									{ return m_bAutoDestroy; }

			xst_fi	xst_castring&	GetObjectName() const
									{ return m_strName; }

			xst_fi	ul32			GetObjectHandle() const
									{ return m_ulObjHandle; }

			xst_fi	CSceneManager*	GetSceneManager() const
									{ return m_pSceneMgr; }

		protected:

				_NodeIterator		_FindNode(cul32& ulHandle);

		protected:

			CSceneManager*	m_pSceneMgr;
			//NodeMap			m_mChildNodes;
			NodeList		m_lChildNodes;
			ObjectVector	m_vObjects;
			CSceneNode*		m_pParentNode;
			bool			m_bAutoDestroy;
            u8 m_padding[ 3 ];
			ul32			m_ulObjHandle;
			xst_astring		m_strName;
            u8 m_padding2[ 4 ];
	};

	//typedef XST::TCObjectSmartPointer< CSceneNode >	SceneNodePtr;

}//xse

//#if defined (XST_WINDOWS)
//#	ifndef GetObject
//#		if defined (XST_UNICODE)
//#			define GetObject GetObjectW
//#		else
//#			define GetObject GetObjectA
//#		endif //if defined XST_UNICODE
//#	endif //ifndef GetObject
//#endif //XST_WINDOWS
#pragma pop_macro( "GetObject" )

#endif //XSE_CSCENE_NODE_H