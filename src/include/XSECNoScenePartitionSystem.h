#ifndef XSE_CNO_SCENE_PARTITION_SYSTEM_H
#define XSE_CNO_SCENE_PARTITION_SYSTEM_H

#include "XSEIScenePartitionSystem.h"

namespace XSE
{
	class CNoScenePartitionSystem : public IScenePartitionSystem
	{
		public:

								CNoScenePartitionSystem(IRenderSystem* pRS);
			virtual				~CNoScenePartitionSystem();

					i32			Draw();

		protected:

			IRenderSystem*		m_pRS;
	};

}//xse

#endif