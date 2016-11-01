#include "CMEpoll.h"

namespace CMLib
{
	namespace Socket
	{

		int CMEpoll::EpollCreate(int nFdSize)
		{
			m_nEpollEventSize = nFdSize;
			m_pstEpollEvent = std::shared_ptr<epoll_event>(new epoll_event[m_nEpollEventSize], [](const epoll_event* p){delete[]p; });

			m_nEpollFd = epoll_create(m_nEpollEventSize);
			if (m_nEpollFd < 0)
			{
				return -1;
			}

			memset(&m_stOneEpollEvent, 0, sizeof(m_stOneEpollEvent));

			m_stOneEpollEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP;
			m_stOneEpollEvent.data.ptr = NULL;
			m_stOneEpollEvent.data.fd = -1;

			return 0;
		}
		int CMEpoll::EpollWait()
		{
			int nEpollEventNumber = 0;
			nEpollEventNumber = epoll_wait(m_nEpollFd, m_pstEpollEvent.get(), m_nEpollEventSize, m_nEpollWaitingTime);
			if (nEpollEventNumber < 0)
			{
				if (errno != EINTR)
				{
					//wait error
				}
				return -1;
			}

			int nFd = 0;
			unsigned int unEvent;
			for (int i = 0; i < nEpollEventNumber; ++i)
			{
				nFd = (m_pstEpollEvent.get()+i)->data.fd;
				unEvent = (m_pstEpollEvent.get() + i)->events;

				EEpollEvent nEpollEvent = EEpollEvent_MAX;
				if (IsReadEvent(unEvent))
				{
					nEpollEvent = EEpollEvent_Read;
				}
				else if (IsWriteEvent(unEvent))
				{
					nEpollEvent = EEpollEvent_Write;
				}
				else if (IsErrorEvent(unEvent))
				{
					nEpollEvent = EEpollEvent_Error;
				}
				else
				{
					continue;
				}
				OnEventHandle(nEpollEvent, nFd);
			}

			return 0;
		}

		int CMEpoll::EpollAdd(int nFd)
		{
			m_stOneEpollEvent.data.fd = nFd;
			if (epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, nFd, &m_stOneEpollEvent) < 0)
			{
				return -1;
			}
			SetNonBlock(nFd);
			return 0;
		}


		int CMEpoll::EpollDelete(int nFd)
		{
			m_stOneEpollEvent.data.fd = nFd;
			if (epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, nFd, &m_stOneEpollEvent) < 0)
			{
				return -1;
			}

			return 0;
		}

		bool CMEpoll::IsErrorEvent(uint32_t unEvent)
		{
			return (EPOLLERR | EPOLLHUP) & unEvent;
		}

		bool CMEpoll::IsReadEvent(uint32_t unEvent)
		{
			return EPOLLIN & unEvent;
		}

		bool CMEpoll::IsWriteEvent(uint32_t unEvent)
		{
			return EPOLLOUT & unEvent;
		}

		int CMEpoll::OnEventHandle(EEpollEvent nEvent, int nFd)
		{
			if (nEvent >= EEpollEvent_MAX)
			{
				return -1;
			}
			(m_pstEpollEventHandle[nEvent])(nFd);

			return 0;
		}
		int CMEpoll::SetNonBlock(int nFd)
		{
			int nFlag = fcntl(nFd, F_GETFL, 0);
			nFlag |= O_NONBLOCK;
			fcntl(nFd, F_SETFL, nFlag);

			return 0;
		}

		void CMEpoll::SetEpollWaitingTime(int nWaitingTime)
		{
			m_nEpollWaitingTime = nWaitingTime;
		}

		CMEpoll::CMEpoll()
		{

		}

		CMEpoll::~CMEpoll()
		{

		}

		int CMEpoll::SetEpollEventHandle(EEpollEvent nEvent, std::function<int(int)> stEventHandle)
		{
			if (nEvent >= EEpollEvent_MAX)
			{
				return -1;
			}
			m_pstEpollEventHandle[nEvent] = stEventHandle;

			return 0;
		}
	}
}