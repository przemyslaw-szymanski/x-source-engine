#ifndef XSE_GUI_CCOMPONENT_H
#define XSE_GUI_CCOMPONENT_H

#include "XSEGUICommon.h"
#include "XSECMaterial.h"

namespace XSE
{
	namespace GUI
	{
		class IGUIRenderer;

		class XST_API CComponent
		{
			friend class IGUIRenderer;
			friend class CGUIManager;

			public:

				typedef	xst_vector< CComponent* >		CompVector;
				typedef XST::TCIterator< CompVector >	ComponentIterator;

			public:

										CComponent(ul32 ulStyles, COMPONENT_TYPE eType, IGUIRenderer* pRenderer, CMaterialManager* pMatMgr, CComponent* pParent);
				virtual					~CComponent();

				virtual	i32				AddChild(CComponent* pComp);

				virtual CComponent*		CreateComponent();

				xst_fi	COMPONENT_TYPE	GetComponentType() const
										{ return m_eCompType; }

				virtual	void			SetPosition(cu32& uiX, cu32& uiY);

				virtual	const Vec3		GetPosition() const
										{ return m_vecPosition; }

				virtual	void			SetSize(cu32& uiWidth, cu32& uiHeight);

				virtual const Vec2		GetSize() const
										{ return m_vecSize; }

				virtual	CComponent*		GetParent() const
										{ return m_pParent; }

				virtual	CComponent*		GetTopParent();

				virtual	HANDLE			GetHandle() const
										{ return (HANDLE)&m_lHandle; }

				virtual	void			Update();

				virtual	void			UpdateSize();

				virtual void			UpdatePosition();

				virtual void			UpdateChildren();

				virtual void			UpdateChild();

				virtual	void			DestroyChildren();

				virtual	void			SetMetricsType(COMPONENT_METRICS_TYPE eType);

				virtual const MaterialPtr&	GetMaterial() const
											{ return m_pMaterial; }

				virtual xst_fi void		SetMaterial(const MaterialPtr& pMat)
										{ m_pMaterial = pMat; }

				virtual	void			SetText(xst_cwstring& strText)
										{ m_strText = strText; }

				virtual xst_cwstring&	GetText() const
										{ return m_strText; }

				virtual	ul32			GetStyles() const
										{ return m_ulStyles; }

				virtual ComponentIterator	GetChildIterator();

			protected:

				Vec3				m_vecPosition;
				Vec2				m_vecSize;
				CComponent*			m_pParent;
				xst_wstring			m_strText;
				l32					m_lHandle;
				COMPONENT_TYPE		m_eCompType;
				CompVector			m_vChildren;
				CMT					m_eMetricsType;
				MaterialPtr			m_pMaterial;
				ul32				m_ulStyles;
				//Index range in buffer
				u16					m_usBeginIndex;
				u16					m_usBeginVertex;
				u16					m_usIndexCount;
                u8 m_padding[ 2 ];
				ul32				m_ulGroupId;
				ul32				m_ulSubGroupId;
				IGUIRenderer*		m_pGUIRenderer;
		};
	}//gui
}//xse

#endif