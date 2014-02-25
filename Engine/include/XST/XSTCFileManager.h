#ifndef XST_CFILEMANAGER_H
#define XST_CFILEMANAGER_H

#include "XSTCommonTypes.h"
#include "XSTCommonInclude.h"
#include "XSTTCData.h"
#include "XSTTCSmartPointer.h"
#include "XSTTCResourceManager.h"
#include "XSTIFileLoader.h"
#include "XSTTCIterator.h"

namespace XST
{
	
	class CFileLocation
	{
		public:

							CFileLocation() : pLoader( xst_null ), bRecursive( false ) {}
							CFileLocation(xst_castring& strLoc, IFileLoader* pFileLoader, bool bRecur) : strLocation( strLoc ), pLoader( pFileLoader ), bRecursive( bRecur ) {}

			xst_astring		strLocation;
			xst_astring		strLocationPath;
			IFileLoader*	pLoader;
			bool			bRecursive;
			bool			bPhysical;
	};

	class IFileListener 
	{
		public:

			virtual	i32		OnBeforeLoad(xst_castring& strFileName, xst_castring& strGroupName) { return 0; }
			virtual void	OnAfterLoad(Resources::FilePtr pFile) { }
	};

	class CFileLocationManager : public XST::TCBaseResourceManager< xst_vector< CFileLocation >*, 
																	xst_astring, 
																	xst_astring, 
																	TCNoResourceNameHasher< xst_astring > >
	{
		friend class CFileManager;

        public:

			typedef XST::TCBaseResourceManager< xst_vector< CFileLocation >*, 
																	xst_astring, 
																	xst_astring, 
																	TCNoResourceNameHasher< xst_astring > > BaseManager;
            typedef BaseManager::_ResourceIterator LocationIterator;

		public:

								CFileLocationManager(CFileManager* pMgr);
			virtual				~CFileLocationManager();

					void		DestroyResources();

			xst_fi	void		DestroyLocations()
								{ DestroyResources(); }

					i32			AddLocation(xst_castring& strLocation, xst_castring& strGroupName, IFileLoader* pLoader, bool bRecursive);

					i32			AddLocation(const CFileLocation& Loc, xst_castring& strGroupName);
		protected:

			CFileManager*		m_pMgr;
	};

	struct SFileManagerFile
	{
		IFileLoader*		pLoader;
		Resources::FilePtr	pFile;
	};

	class CFileManager : public XST::TCSingleton< CFileManager >, public XST::TCResourceManager< SFileManagerFile, 
																								xst_astring,
																								xst_astring,
																								XST::TCNoResourceNameHasher< xst_astring > >
	{
		friend class CFileLocationManager;

		public:

			typedef SFileManagerFile	Res;
			typedef XST::TCResourceManager< SFileManagerFile, 
											xst_astring,
											xst_astring,
											XST::TCNoResourceNameHasher< xst_astring > > BaseManager;
			typedef BaseManager::Resource					Resource;
			typedef BaseManager::ResourceGroup				ResourceGroup;
			typedef BaseManager::GroupPtr					GroupPtr;
			//typedef BaseManager::GroupIterator				GroupIterator;
			typedef BaseManager::ResourceIterator			ResourceIterator;
			typedef xst_map< xst_astring, IFileLoader* >	LoaderMap;

			typedef xst_vector< IFileListener* >			ListenerVector;
			typedef BaseManager::Delegate					Delegate;

		public:

										CFileManager();
			virtual						~CFileManager();


					i32					AddLocation(xst_castring& strDirectory, xst_castring& strGroupName, xst_castring& strLoaderName, bool bRecursive = false);

					i32					AddFileExtension(xst_castring& strExt);

					i32					PrepareAllGroups();

					i32					PrepareGroup(xst_castring& strGroupName);

					i32					RegisterLoader(xst_castring& strLoaderName, IFileLoader* pLoader);

					Resources::FilePtr	LoadFile(xst_castring& strFileName, xst_castring& strGroupName);

					Resources::FilePtr	LoadFile(xst_castring& strFileName);

					Resources::FilePtr	LoadFile(xst_castring& strDirPath, xst_castring& strFileName, xst_castring& strLoaderName, xst_castring& strGroupName);

					void				AddListener(IFileListener* pListener) 
										{ m_vListeners.push_back( pListener ); }

					i32					RemoveListener(IFileListener* pListener);

					IFileLoader*		GetFileLoader(xst_castring& strName) const;

					bool				IsExtensionSupported(xst_castring& strExt);

					bool				IsFileTypeSupported(xst_castring& strFileName);

					void				IsExtensionFilter(bool bIs)
										{ m_bExtensionFilter = bIs; }

					bool				IsExtensionFilter() const
										{ return m_bExtensionFilter; }


		protected:

				//void				_OnDestroyFile(ResourcePtr pRes);
				//void				_OnRemoveFile(ResourcePtr pRes) {}
				IFileLoader*		_GetLoader(xst_castring& strName);
				i32					_GetDefaultDir(xst_castring& strLocation, xst_astring* strDirOut, xst_astring* strPathOut, bool* bDirOut);

		private:

			xst_map< xst_astring, i32 >	m_mExts;
			xst_vector<xst_astring>		m_vLocations;
			CFileLocationManager*		m_pLocMgr;
			LoaderMap					m_mLoaders;
			LoaderMap					m_mLoaderInstances;

			ListenerVector				m_vListeners;

			Delegate					m_OnResRemove;
			Delegate					m_OnResDestroy;

            bool						m_bAllFilesSupported;
			bool						m_bExtensionFilter;
	};

}//xst

#endif
