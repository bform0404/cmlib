#ifndef __CM_TIMER_QUEUE_H__
#define __CM_TIMER_QUEUE_H__

#include <set>
#include <functional>
#include <stdint.h>

namespace CMLib
{
	namespace Thread
	{
		class CMTimerQueue
		{
		public:
			CMTimerQueue() = default;
			~CMTimerQueue() = default;
			CMTimerQueue(CMTimerQueue&& other) = delete;
			CMTimerQueue& operator=(CMTimerQueue&& other) = delete;
			CMTimerQueue(const CMTimerQueue&) = delete;
			CMTimerQueue& operator=(const CMTimerQueue&) = delete;

			void SetTimer(uint32_t nTimerId, uint32_t nMilliSeconds,
				const std::function<void(uint32_t)>& OnTimer);
			void KillTimer(uint32_t nTimerId);

			void KillAllTimer();
			bool Empty() const;

			bool Peek(uint32_t& nTimerId, std::function<void(uint32_t)>& OnTimer);
			uint64_t GetWaitTime();

		private:
			void Erase(uint32_t nTImerId);
		private:
			struct Timer
			{
				uint32_t nTimerId;  ///< 时钟ID
				uint64_t nMilliSeconds;  ///< 间隔, 单位: 毫秒
				uint64_t nBeginTime;  ///< 开始时间, 单位: 毫秒
				std::function<void(uint32_t)> OnTimer;  ///< 回调函数
				bool operator<(const Timer& right) const;
			};

			std::set<Timer> m_stTimers;  ///< timers
		};
	}
} 

#endif 
