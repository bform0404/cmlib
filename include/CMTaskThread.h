#ifndef __CM_TASK_THREAD_H__
#define __CM_TASK_THREAD_H__

#include <thread>
#include "CMThreadQueue.h"
#include "CMTaskQueue.h"
#include <vector>

namespace CMLib
{
	namespace Thread
	{
		class CMTaskThread
		{
		public:
			CMTaskThread();
			~CMTaskThread();

			void Begin(int nThreadCount = 1);
			void End();
			
			void PostTask(const std::function<void()>& stTask);
			void PostTask(const std::function<void()>&& stTask);
			uint32_t GetTaskCount() const;
			void SetTimer(uint32_t nTimerId, uint32_t nMilliSeconds,
				const std::function<void(uint32_t)>& OnTimer);
			void KillTimer(uint32_t nTimerId);
			void KillAllTimer();
		private:
			void ThreadMain();

		private:
			std::vector<std::thread> m_stThreads;
			CMThreadQueue<std::function<void()>, CMTaskQueue> m_stQueue;
		};
	}
}

#endif
