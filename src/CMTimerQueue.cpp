#include <assert.h>
#include <algorithm>
#include <mutex>
#include "CMTimerQueue.h"

namespace CMLib
{
	namespace Thread
	{

		uint64_t get_tick_count()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::steady_clock::now().time_since_epoch()).count();
		}

		void CMTimerQueue::SetTimer(uint32_t nTimerId, uint32_t nMilliSeconds,
			const std::function<void(uint32_t)>& OnTimer)
		{
			Erase(nTimerId);
			m_stTimers.insert(Timer{ nTimerId, nMilliSeconds, get_tick_count(), OnTimer });
		}

		void CMTimerQueue::KillTimer(uint32_t nTimerId)
		{
			Erase(nTimerId);
		}


		void CMTimerQueue::KillAllTimer()
		{
			for (auto iter = m_stTimers.begin(); iter != m_stTimers.end(); iter++)
			{
				m_stTimers.erase(iter);
			}
		}

		void CMTimerQueue::Erase(uint32_t nTImerId)
		{
			for (auto iter = m_stTimers.begin(); iter != m_stTimers.end(); iter++)
			{
				if (iter->nTimerId == nTImerId)
				{
					m_stTimers.erase(iter);
					break;
				}
			}
		}

		bool CMTimerQueue::Empty()const
		{
			return m_stTimers.empty();
		}

		bool CMTimerQueue::Peek(uint32_t& nTimerId, std::function<void(uint32_t)>& OnTimer)
		{
			if (m_stTimers.empty()) return false;
			Timer first_timer = *m_stTimers.begin();
			uint64_t end_time = first_timer.nBeginTime + first_timer.nMilliSeconds;
			uint64_t current = get_tick_count();
			if (current >= end_time)
			{
				nTimerId = first_timer.nTimerId;
				OnTimer = first_timer.OnTimer;
				m_stTimers.erase(m_stTimers.begin());
				first_timer.nBeginTime += first_timer.nMilliSeconds;
				m_stTimers.insert(first_timer);
				return true;
			}
			else
			{
				return false;
			}
		}

		uint64_t CMTimerQueue::GetWaitTime()
		{
			const uint64_t kWaitInfinite = 60 * 60 * 1000;  // 1 hour
			if (m_stTimers.empty()) return kWaitInfinite;
			Timer first_timer = *m_stTimers.begin();
			uint64_t end_time = first_timer.nBeginTime + first_timer.nMilliSeconds;
			uint64_t current = get_tick_count();
			return current <= end_time ? end_time - current : 0;
		}

		bool CMTimerQueue::Timer::operator<(const Timer& right) const
		{
			auto end_time = nBeginTime + nMilliSeconds;
			auto right_end_time = right.nBeginTime + right.nMilliSeconds;
			return (
				(end_time < right_end_time) ||
				(end_time == right_end_time && nTimerId < right.nTimerId));
		}

	}  // namespace detail
}  // namespace ustd
