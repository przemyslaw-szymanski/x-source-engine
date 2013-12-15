#ifndef XSE_ISCENE_PARTITION_SYSTEM_H
#define XSE_ISCENE_PARTITION_SYSTEM_H

#include "XSECommon.h"
#include "XSECRenderQueue.h"

namespace XSE
{
	class IScenePartitionListener
	{
		public:

			virtual				~IScenePartitionListener() {}

			virtual void		OnProcess(IScenePartitionListener* pSystem) {}
	};

	class IScenePartitionSystem
	{
		friend class CSceneManager;

		public:

			typedef xst_vector< CObject* >	ObjectVector;
		
		public:

									IScenePartitionSystem(CSceneManager* pSceneMgr) : m_pSceneMgr( pSceneMgr )
									{
										m_vObjects.reserve( 1000 );
									}

			virtual					~IScenePartitionSystem() 
									{
										xst_vector_clear( m_vObjects, ObjectVector::value_type );
									}

			virtual i32				Init() 
									{ return XST_OK; }

			virtual	void			Update() 
									{ return; }
			
			virtual void			AddObject(CObject* pObj) 
									{ m_vObjects.push_back( pObj ); }
			
			virtual void			RemoveObject(CObject* pObj) 
									{ m_vObjects.erase( xst_find( m_vObjects.begin(), m_vObjects.end(), pObj ) ); }

			virtual void			ReorganizeObject(CObject* pObj)
									{ return; }

			virtual 
			const	ObjectVector&	GetObjects() const
									{ return m_vObjects; }

			virtual	void			AddObjects(const ObjectVector& vObjects)
									{ m_vObjects.insert( m_vObjects.end(), vObjects.begin(), vObjects.end() ); }

			virtual void			StartProcessing() {}

			virtual void			StopProcessing() {}

			virtual void			SkipProcess() {}

		protected:


		protected:

			CSceneManager*	m_pSceneMgr;
			ObjectVector	m_vObjects;
			
	};

}//xse

#endif