#include "CMTaskThread.h"

namespace CMLib
{
	namespace Thread
	{

		CMTaskThread::CMTaskThread()
		{
		}

		CMTaskThread::~CMTaskThread()
		{
		}

		void CMTaskThread::Begin(int nThreadCount)
		{
			for (int i = 0; i < nThreadCount; i++)
			{
				m_stThreads.emplace_back(&CMTaskThread::ThreadMain, this);
			}
		}

		void CMTaskThread::End()
		{
			m_stQueue.NotifyExit();
			for (auto& thread : m_stThreads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}
		}

		void CMTaskThread::PostTask(const std::function<void()>& task)
		{
			m_stQueue.PostTask(std::function<void()>(task));
		}

		void CMTaskThread::PostTask(const std::function<void()>&& task)
		{
			m_stQueue.PostTask(task);
		}

		uint32_t CMTaskThread::GetTaskCount() const
		{
			return m_stQueue.TaskCount();
		}

		void CMTaskThread::SetTimer(uint32_t nTimerId, uint32_t nMilliSeconds,
			const std::function<void(uint32_t)>& OnTimer)
		{
			m_stQueue.SetTimer(nTimerId, nMilliSeconds, OnTimer);
		}

		void CMTaskThread::KillTimer(uint32_t nTimerId)
		{
			m_stQueue.KillTimer(nTimerId);
		}


		void CMTaskThread::KillAllTimer()
		{
			m_stQueue.KillAllTimer();
		}

		void CMTaskThread::ThreadMain()
		{
			m_stQueue.DoWhile();
		}
	}
}
