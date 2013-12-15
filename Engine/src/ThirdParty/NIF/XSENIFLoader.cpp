#include "XSENIFLoader.h"
#include "include/niflib.h"
#include "include/obj/NiObject.h"

namespace XSE
{
	class CBinaryStreamBuf : public std::streambuf
	{
		public:

			CBinaryStreamBuf(const XST::CBinaryData& Data)
			{
				char* pBegin = (char*)Data.GetData();
				this->setg( pBegin, pBegin, pBegin + Data.GetSize() );
			}
	};

	namespace Resources
	{
		CNIFLoader::CNIFLoader()
		{
		}

		CNIFLoader::~CNIFLoader()
		{
		}

		i32 CNIFLoader::Init()
		{
			return XST_OK;
		}

		IResource* CNIFLoader::Load(const XST::CBinaryData& Data)
		{
			IResource* pRes = xst_null;
			Niflib::NifInfo Info;
			CBinaryStreamBuf DataStream( Data );
			std::istream is( &DataStream );
			Niflib::HeaderString HeaderStr;
			Niflib::NifStream Header( HeaderStr, is, Info );
			
			if( !Niflib::IsSupportedVersion( Info.version ) )
			{
				XST_SET_ERR( "Invalid NIF file version: " << Info.version );
				return NULL;
			}
			
			Niflib::Ref< Niflib::NiObject > pObj = Niflib::ReadNifTree( is, &Info );
			
			if( pObj == NULL )
			{
				return NULL;
			}

			Niflib::NiNodeRef pNiNode = Niflib::DynamicCast< Niflib::NiNode >( pObj );
			if( pNiNode == NULL )
			{
				return NULL;
			}



			return pRes;
		}

		void CNIFLoader::Destroy(IResource* pRes, xst_castring& strResGrpName)
		{
		}

	}//resources
}//xse