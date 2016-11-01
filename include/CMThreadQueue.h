#ifndef __CM_THREAD_QUEUE_H__
#define __CM_THREAD_QUEUE_H__

#include <mutex>
#include <condition_variable>
#include "CMTimerQueue.h"

namespace CMLib
{
	namespace Thread
	{
		template<typename TaskT, template<typename> class TaskQueueT>
		class CMThreadQueue
		{
		public:
			CMThreadQueue() = default;
			~CMThreadQueue() = default;
			CMThreadQueue(CMThreadQueue&& other);
			CMThreadQueue& operator=(CMThreadQueue&& other);
			CMThreadQueue(const CMThreadQueue&) = delete;
			CMThreadQueue& operator=(const CMThreadQueue&) = delete;

			void PostTask(const TaskT& task)
			{
				m_stMutex.lock();
				m_stTaskQueue.Push(task);
				m_stMutex.unlock();
				m_stCondition.notify_one();
			}

			void PostTask(TaskT&& task)
			{
				m_stMutex.lock();
				m_stTaskQueue.Push(task);
				m_stMutex.unlock();
				m_stCondition.notify_one();
			}

			int  TaskCount() const
			{
				std::lock_guard<std::mutex> guard(m_stMutex);
				return m_stTaskQueue.Size();
			}

			void SetTimer(
				uint32_t nTimerId, uint32_t nMilliSeconds,
				const std::function<void(uint32_t)>& OnTimer)
			{
				m_stMutex.lock();
				m_stTimerQueue.SetTimer(nTimerId, nMilliSeconds, OnTimer);
				m_stMutex.unlock();
				m_stCondition.notify_one();
			}

			void KillTimer(uint32_t nTimerId)
			{
				std::lock_guard<std::mutex> guard(m_stMutex);
				m_stTimerQueue.KillTimer(nTimerId);
			}
			void KillAllTimer()
			{
				std::lock_guard<std::mutex> guard(m_stMutex);
				m_stTimerQueue.KillAllTimer();
			}
		public:
			void DoWhile()
			{
				std::unique_lock<std::mutex> lock(m_stMutex);
				while (!m_bExit)
				{
					if (m_stTaskQueue.Empty())
					{
						uint64_t t = m_stTimerQueue.GetWaitTime();
						m_stCondition.wait_for(lock, std::chrono::milliseconds(t));
					}
					if (m_bExit) 
						break;
					DoOneTimer(lock);
					if (m_bExit) 
						break;
					DoOneTask(lock);
					if (m_bExit) 
						break;
				}
			}

			void NotifyExit()
			{
				m_stMutex.lock();
				m_bExit = true;
				m_stMutex.unlock();
				m_stCondition.notify_all();
			}

		private:
			void DoOneTimer(std::unique_lock<std::mutex>& lock)
			{
				uint32_t nTimerId;
				std::function<void(uint32_t)> onTimer;
				if (m_stTimerQueue.Peek(nTimerId, onTimer))
				{
					lock.unlock();
					onTimer(nTimerId);
					lock.lock();
				}
			}

			void DoOneTask(std::unique_lock<std::mutex>& lock)
			{
				TaskT task;
				if (m_stTaskQueue.Pop(task))
				{
					lock.unlock();
					task();
					lock.lock();
				}
			}

		private:
			TaskQueueT<TaskT> m_stTaskQueue;
			CMTimerQueue m_stTimerQueue;
			mutable std::mutex m_stMutex;
			std::condition_variable m_stCondition;
			bool m_bExit = false;
		};
	}
}

#endif
