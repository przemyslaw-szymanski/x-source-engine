#ifndef XSE_COCTREE_H
#define XSE_COCTREE_H

#include "../include/XSECommon.h"
#include "../include/XSECObject.h"

namespace XSE
{
	class COctree;

	class IOctreeListener
	{
		public:

			virtual				~IOctreeListener() {}

			virtual	void		OnAddNode(const COctree* pNode) {}

			virtual void		OnAddObject(const COctree* pNode, const CObject* pObj) {}

			virtual void		OnVisitNode(const COctree* pNode) {}
	};

	class COctree
	{
		friend class IScenePartitionSystem;
		friend class CSceneManager;

		public:

			typedef xst_vector< CObject* >	ObjectVector;

		public:

			struct Nodes
			{
				enum NODE
				{
					LEFT_BOTTOM_BACK,
					RIGHT_BOTTOM_BACK,
					LEFT_TOP_BACK,
					RIGHT_TOP_BACK,
					LEFT_BOTTOM_FRONT,
					RIGHT_BOTTOM_FRONT,
					LEFT_TOP_FRONT,
					RIGHT_TOP_FRONT,
					_ENUM_COUNT
				};
			};
			typedef Nodes::NODE		NODE;

			struct Results
			{
				enum RESULT
				{
					NOT_IN_NODE = 2,
					MAX_DEPTH_REACHED,
					MEMORY_ERROR,
					_ENUM_COUNT
				};
			};
			typedef Results::RESULT	RESULT;

		public:

									COctree();

			virtual					~COctree();

					void			Init(f32 fSize, const Vec3& vecCenterPos, IOctreeListener* pListener = xst_null);
					void			Init(f32 fSize, const Vec3& vecMinCorner, cu8& byCurrDepth, COctree* pParent);

					i32				AddObject(CObject* pObj, cu8& byMaxDepth);

					void			RemoveObject(CObject* pObj);

					i32				ReorganizeObject(CObject* pObj, cu8& byMaxDepth);

					COctree*		GetNode(CObject* pObj);

			xst_fi	const Vec3&		GetLeftTopBackCorner() const
									{ return m_vecMinCorner; }

			xst_fi	f32				GetSize() const
									{ return m_fSize; }

			xst_fi	const COctree*	GetParent() const
									{ return m_pParent; }

			xst_fi	
			const COctree* const*	GetChildren() const
									{ return m_apChildren; }

			xst_fi	COctree**		GetChildren()
									{ return m_apChildren; }

			xst_fi	
			const COctree* const*	GetNeighbours() const
									{ return m_apNeighbours; }

			xst_fi 
			const	ObjectVector&	GetObjects() const
									{ return m_vObjects; }

			xst_fi ObjectVector&	GetObjects()
									{ return m_vObjects; }

			xst_fi Vec3				CalcCenter() const
									{ 
										cf32 fHalfSize = m_fSize * 0.5f;
										return m_vecMinCorner + fHalfSize; 
									}

			xst_fi CBoundingVolume	CalcBoundingVolume() const
									{ return CBoundingVolume( CalcBoundingSphere(), CalcAABB() ); }

			XST::CAABB				CalcAABB() const;
			void					CalcAABB(XST::CAABB* pOut) const;

			XSE::CBoundingSphere	CalcBoundingSphere() const;

			xst_fi	void			IsVisible(bool bIs)
									{ m_bIsVisible = bIs; }

			xst_fi	bool			IsVisible() const
									{ return m_bIsVisible; }

		protected:

									COctree(f32 fSize, const Vec3& vecMinCorner, cu8& byCurrDepth, COctree* pParent);

					void			_Init();

					i32				_AddObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth);

					i32				_ReorganizeObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth);

					COctree*		_CalcNodeForObject(COctree* pNode, CObject* pObj, cu8& byMaxDepth);

					COctree*		_CreateNode(COctree* pParent, f32 fSize, const Vec3& vecCenterPos, cu8& byMaxDepth);

					COctree*		_GetNode(COctree* pNode, CObject* pObj, ObjectVector::iterator* pItrOut);

					i32				_CalcNodeIDForObject(const COctree* pNode, CObject* pObj, XST::CAABB* pChildAABB);

					void			_RemoveObject(ObjectVector::iterator& Itr);

					void			_AddObject(CObject* pObj);

		protected:

			COctree*			m_apChildren[ 8 ];
			COctree*			m_apNeighbours[ 4 ];
			COctree*			m_pParent = xst_null;
			Vec3				m_vecMinCorner;
			f32					m_fSize = 0.0f;
			IOctreeListener*	m_pListener = xst_null;
			ObjectVector		m_vObjects;
			u8					m_byCurrDepth = 0;
			bool				m_bIsVisible = true; // Indicates if this node is visible by the culling system
	};
}//xse

#endif //XSE_COCTREE_H