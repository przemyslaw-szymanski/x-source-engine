#ifndef XSE_CRENDER_ARRAY_H
#define XSE_CRENDER_ARRAY_H

#include "XSECommon.h"

namespace XSE
{

	class CRenderObjectArray
	{
		public:

			typedef xst_vector< IRenderableObject* >	ObjectVector;


					i32				AddObject(IRenderableObject* pObj)
									{ m_vObjects.push_back( pObj ); return XST_OK; }	
		
		protected:

			ObjectVector	m_vObjects;
	};

	class CRenderMaterialArray
	{
		public:

			typedef CRenderObjectArray	ChildArray;
			typedef xst_map< Resources::CMaterial*, CRenderObjectArray* >	ObjectArrayMap;

		public:

					i32			AddMaterial(Resources::CMaterial* pMat);

					i32			AddObject(IRenderableObject* pObj);

		protected:

			ObjectArrayMap	m_mObjects;
			
	};

	class CRenderArray
	{
	public:

		typedef CRenderMaterialArray	ChildArray;
		typedef xst_map< const IInputLayout*, ChildArray* >	ILArrayMap;

	public:

						CRenderArray();
		virtual			~CRenderArray();

				i32		AddInputLayout(const IInputLayout* pIL);

				i32		AddMaterial(Resources::CMaterial* pMat);

				i32		AddObject(IRenderableObject* pObj);

				CRenderMaterialArray*	GetMaterialArray(Resources::CMaterial* pMat);

	protected:

		ILArrayMap	m_mInputLayouts;

	};

	class XST_API CRenderTree
	{
		public:

			typedef IRenderableObject*	ObjPtr;
			//typedef RenderableObjectPtr	ObjPtr;
			typedef Resources::IVertexShader*		VSPtr;
			typedef	Resources::IPixelShader*		PSPtr;
			typedef Resources::ITexture*			TexPtr;
			typedef IInputLayout*		ILPtr;

			/*typedef xst_vector< ObjPtr >			ObjVec;
			typedef xst_map< TexPtr, ObjVec* >		TexObjMap;
			typedef xst_map< PSPtr, TexObjMap* >	PSTexMap;
			typedef xst_map< VSPtr, PSTexMap* >		VSPSMap;
			typedef xst_map< ILPtr, VSPSMap* >		ILVSMap;*/
#if( XST_VS_VER < 10 )
			XST_TEMPLATE_CLASS std::allocator< ObjPtr >;
#endif
			xst_tvectorx( ObjPtr )				ObjVec;
			xst_tmapx( TexPtr, ObjVec* )		TexObjMap;
			xst_tmapx( PSPtr, TexObjMap* )		PSTexMap;
			xst_tmapx( VSPtr, PSTexMap* )		VSPSMap;
			xst_tmapx( ILPtr, VSPSMap* )		ILVSMap;

			virtual				~CRenderTree();

					i32			Add(ObjPtr pObj);

					i32			Add(Resources::CModel* pModel);

					i32			Remove(ObjPtr pObj);

					VSPSMap*	GetOrCreateVSPSMap(ILPtr pIL);
					PSTexMap*	GetOrCreatePSTexMap(ILPtr pIL, VSPtr pVS);
					TexObjMap*	GetOrCreateTexObjMap(VSPtr pVS, PSPtr pPS);
					ObjVec*		GetOrCreateObjVec(VSPtr pVS, PSPtr pPS, TexPtr pTex);

					ILVSMap*	GetILVSMap();
					VSPSMap*	GetVSPSMap(ILPtr pIL);
					PSTexMap*	GetPSTexMap(ILPtr pIL, VSPtr pVS);
					TexObjMap*	GetTexObjMap(VSPtr pVS, PSPtr pPS);
					ObjVec*		GetObjVec(VSPtr pVS, PSPtr pPS, TexPtr pTex);

			xst_fi	const ObjVec&	GetObjects() const
									{ return m_vObjs; }

		protected:

			ILVSMap		m_mTree;
			ObjVec		m_vObjs;
	};

}//xse

#endif