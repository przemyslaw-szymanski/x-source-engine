#ifndef XSE_IINPUT_LAYOUT_H
#define XSE_IINPUT_LAYOUT_H

#include "XSERenderSystemCommon.h"
#include "XSEIVertexShader.h"
#include "XSEIPixelShader.h"

namespace XSE
{
	struct SInputLayoutElement
	{
		enum
		{
			UNAVAILABLE = 0xffffffffL
		};

		SInputLayoutElement() : eType( (INPUT_LAYOUT_ELEMENT)UNAVAILABLE ), ulSize( ILESizes::POSITION ), ulOffset( 0 ), uiId( 0xffffffff ) {}
		SInputLayoutElement(INPUT_LAYOUT_ELEMENT eElement, ul32 ulSize, ul32 ulOffset) : eType( eElement ), ulSize( ulSize ), ulOffset( ulOffset ) {}
		INPUT_LAYOUT_ELEMENT	eType; //type of the element e.g. POSITION
		ul32					ulSize; //size of the element e.g. sizeof( Vec3 )
		ul32					ulOffset; //offset in input layout
		u32						uiId; //Id of the element in enum struct, usable for array maps

		bool	operator<(const SInputLayoutElement& Right)
		{
			//Sort from higher to lower, position should be at first place
			return eType > Right.eType;
		}
	};


	class IInputLayout
	{
		public:

													IInputLayout() { xst_zero( m_aAvailableElements, sizeof( bool ) * ILEs::_ENUM_COUNT ); }
			virtual									~IInputLayout() {}

			virtual	i32								Create(cul32& ulElements) = 0;

			virtual ul32							GetHandle() const = 0;

			virtual	xst_fi const bool*				GetAvailableElements() const { return m_aAvailableElements; }
			virtual xst_fi u32						GetAvailableElementCount() const { return ILEs::_ENUM_COUNT; }
			virtual xst_fi bool						IsElementAvailable(cul32& ulElId) const { return m_aAvailableElements[ ulElId ]; }
			
			virtual const	SInputLayoutElement*	GetElements() const = 0;

			virtual u32								GetElementCount() = 0;

			virtual cu32							GetElementCount() const = 0;

			virtual ul32							GetElementSize(cul32& ulElementId) const = 0;

			virtual ul32							GetElementOffset(cul32& ulElementId) const = 0;

			virtual xst_castring&					GetVertexShaderCode() const = 0;

			virtual const VertexShaderPtr			GetVertexShader() const = 0;

			virtual VertexShaderPtr					GetVertexShader() = 0;

			virtual const PixelShaderPtr			GetPixelShader() const = 0;

			virtual PixelShaderPtr					GetPixelShader() = 0;

			virtual cul32							GetVertexSize() const = 0;

			virtual bool							IsPosition() const { return m_aAvailableElements[ ILEIds::POSITION ]; }
			virtual bool							IsNormal() const { return m_aAvailableElements[ ILEIds::NORMAL ]; }
			virtual bool							IsBinormal() const { return m_aAvailableElements[ ILEIds::BINORMAL ]; }
			virtual bool							IsTangent()	const { return m_aAvailableElements[ ILEIds::TANGENT ]; }
			virtual bool							IsColor() const { return m_aAvailableElements[ ILEIds::COLOR ]; }
			virtual bool							IsTexCoord0() const { return m_aAvailableElements[ ILEIds::TEXCOORD0 ]; }
			virtual bool							IsTexCoord1() const { return m_aAvailableElements[ ILEIds::TEXCOORD1 ]; }
			virtual bool							IsTexCoord2() const { return m_aAvailableElements[ ILEIds::TEXCOORD2 ]; }
			virtual bool							IsTexCoord3() const { return m_aAvailableElements[ ILEIds::TEXCOORD3 ]; }
			virtual bool							IsTexCoord4() const { return m_aAvailableElements[ ILEIds::TEXCOORD4 ]; }
			virtual bool							IsTexCoord5() const { return m_aAvailableElements[ ILEIds::TEXCOORD5 ]; }
			virtual bool							IsTexCoord6() const { return m_aAvailableElements[ ILEIds::TEXCOORD6 ]; }
			virtual bool							IsTexCoord7() const { return m_aAvailableElements[ ILEIds::TEXCOORD7 ]; }
			virtual bool							IsDiffuse() const { return m_aAvailableElements[ ILEIds::DIFFUSE ]; }
			virtual bool							IsSpecular() const { return m_aAvailableElements[ ILEIds::SPECULAR ]; }

			static cu32								GetNormalSize() { return ILESizes::NORMAL; }
			static cu32								GetTexCoordSize() { return ILESizes::TEXCOORD; }
			static cu32								GetSpecularSize() { return ILESizes::POSITION; }
			static cu32								GetDiffuseSize() { return ILESizes::POSITION; }
			static cu32								GetColorSize() { return ILESizes::COLOR; }
			static cu32								GetTangentSize() { return ILESizes::TANGENT; }
			static cu32								GetBinormalSize() { return ILESizes::BINORMAL; }
			static cu32								GetPositionSize() { return ILESizes::POSITION; }


		protected:

			bool	m_aAvailableElements[ InputLayoutElements::_ENUM_COUNT ];

	};

}//xse

#endif //XSE_IINPUT_LAYOUT_H