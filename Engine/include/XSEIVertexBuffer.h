#ifndef XSE_IVERTEX_BUFFER_H
#define XSE_IVERTEX_BUFFER_H

#include "XSECommon.h"
#include "XSECVertexData.h"
#include "XSERenderSystemCommon.h"

namespace XSE
{

	class IVertexBuffer : public XST::IObject
	{
		public:

			virtual						~IVertexBuffer() {}

			virtual i32					Lock() = 0;
			virtual bool				IsLocked() = 0;
			virtual bool				IsLocked() const = 0;
			//virtual i32				Lock(ul32 ulVertexCount, TOPOLOGY_TYPE eType, BUFFER_USAGE eUsage, IInputLayout* pLayout) = 0;
			virtual i32					Unlock(bool bCreate = true) = 0;
			virtual void				SetTopologyType(TOPOLOGY_TYPE eType) = 0;
			virtual TOPOLOGY_TYPE		GetTopologyType() = 0;
			virtual void				SetUsage(BUFFER_USAGE eUsage) = 0;
			virtual BUFFER_USAGE		GetUsage() = 0;
			virtual i32					Create() = 0;
			//virtual Vertex*			AddVertices(ul32 ulVertexCount) = 0;
			//virtual Vertex*			AddVertex() = 0;
			virtual i32					SetVertexCount(ul32 ulCount) = 0;
			virtual void				SetInputLayout(const IInputLayout* pIL) = 0;
			virtual void				SetInputLayout(ul32 uElements) = 0;
			virtual const IInputLayout*	GetInputLayout() const = 0;
			virtual ul32				GetVertexCount() const = 0;
			virtual void				Update() = 0;
			virtual void				Update(cu32& uiStartVertex, cu32& uiVertexCount) = 0;

			virtual	CVertexData&		GetVertexData()
										{ return m_VertexData; }
			virtual const CVertexData&	GetVertexData() const
										{ return m_VertexData; }

		protected:

			CVertexData	m_VertexData;
	};

	typedef XST::TCObjectSmartPointer< IVertexBuffer >	VertexBufferPtr;
	XST_TEMPLATE_CLASS XST::TCObjectSmartPointer< IVertexBuffer >;
	typedef XST::TCWeakPointer< IVertexBuffer >	VertexBufferWeakPtr;
	XST_TEMPLATE_CLASS XST::TCWeakPointer< IVertexBuffer >;

}//xse

#endif