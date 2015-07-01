#ifndef XST_PARAMETER_SUB_SYSTEM_H
#define XST_PARAMETER_SUB_SYSTEM_H

#include "XSTCommonInclude.h"
#include "XSTCommonTypes.h"
#include "XSTCString.h"

namespace XST
{
	template<typename _KEY_, typename _VALUE_>
	class TIParameterSubSystem
	{
		public:

			typedef xst_map< _KEY_, _VALUE_ >	ParamMap;

		public:

			virtual			~TIParameterSubSystem()
			{
				m_mParams.clear();
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, const _VALUE_& _tValue)
			{
				m_mParams[ _tKey ] = _tValue;
			}

			xst_fi	const _VALUE_&	GetParameter(const _KEY_& _tKey, const _VALUE_& _tNotFoundValue)
			{
				typename ParamMap::iterator Itr = m_mParams.find( _tKey );
				if( Itr == m_mParams.end() )
					return _tNotFoundValue;
				return Itr->second;
			}

			xst_fi void	ClearParameters()
						{ m_mParams.clear(); }


		protected:

			ParamMap	m_mParams;
	};

	struct SSubSystemParam
	{
		SSubSystemParam() : IsNull( true ) {}
		SSubSystemParam(xst_unknown _value) : pParam( _value ), IsNull( false ) { if( !_value ) IsNull = true; }
		SSubSystemParam(cf32& _value) : fParam( _value ), IsNull( false ) {}
		SSubSystemParam(ci32& _value) : iParam( _value ), IsNull( false ) {}
		SSubSystemParam(const bool& _value) : bParam( _value ), IsNull( false ) {}
		SSubSystemParam(lpcastr _value) : IsNull( false ) { strParam.Set( _value ); }

		union
		{
			xst_unknown			pParam;
			f32					fParam;
			i32					iParam;
			bool				bParam;
			TSSimpleString<32>	strParam;
		};

		bool IsNull;
        u8 m_padding[ 3 ];
	};

	template<u32 _STRING_LENGTH_ = 32>
	struct TSSubSystemParam
	{
		TSSubSystemParam() : IsNull( true ) {}
		TSSubSystemParam(xst_unknown _value) : pParam( _value ), IsNull( false ) { if( !_value ) IsNull = true; }
		TSSubSystemParam(cf32& _value) : fParam( _value ), IsNull( false ) {}
		TSSubSystemParam(ci32& _value) : iParam( _value ), IsNull( false ) {}
		TSSubSystemParam(const bool& _value) : bParam( _value ), IsNull( false ) {}
		TSSubSystemParam(lpcastr _value) : IsNull( false ) { strParam.Set( _value ); }

		union
		{
			xst_unknown			pParam;
			f32					fParam;
			i32					iParam;
			bool				bParam;
			TSSimpleString<_STRING_LENGTH_>	strParam;
		};

		bool IsNull;
	};

	template<typename _KEY_, u32 _STRING_LENGTH_ = 32>
	class TCParameterSubSystem : public virtual TIParameterSubSystem< _KEY_, TSSubSystemParam< _STRING_LENGTH_ > >
	{
		public:

			typedef TSSubSystemParam< _STRING_LENGTH_ >	Param;
			typedef TIParameterSubSystem< _KEY_, TSSubSystemParam< _STRING_LENGTH_ > >  BaseParamSubSystem;

		public:

			TCParameterSubSystem() : m_uNullParam( Param() ) {}

			xst_fi	const Param&	GetParameter(const _KEY_& _tKey)
			{
				return BaseParamSubSystem::GetParameter( _tKey, m_uNullParam );
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, xst_unknown _value)
			{
				BaseParamSubSystem::SetParameter( _tKey, Param( _value ) );
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, cf32& _value)
			{
				BaseParamSubSystem::SetParameter( _tKey, Param( _value ) );
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, ci32& _value)
			{
				BaseParamSubSystem::SetParameter( _tKey, Param( _value ) );
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, const bool& _value)
			{
				BaseParamSubSystem::SetParameter( _tKey, Param( _value ) );
			}

			xst_fi	void	SetParameter(const _KEY_& _tKey, lpcastr _value)
			{
				BaseParamSubSystem::SetParameter( _tKey, Param( _value ) );
			}

		private:

			Param	m_uNullParam;
	};


	template<u32 _STRING_LENGTH_ = 32>
	class TIParameterSubSystemInt : public TCParameterSubSystem< i32, _STRING_LENGTH_ >
	{
	};

}//xst

#endif
