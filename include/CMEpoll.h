#ifndef __CM_EPOLL_H__
#define __CM_EPOLL_H__

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <functional>
#include <memory>
#include "CMDef.h"

using namespace std;

namespace CMLib
{
	namespace Socket
	{
		enum ESOCKETFLAG
		{
			//¼àÌý×´Ì¬
			ESFG_LISTENING = 0x00000001,
			//Á¬½Ó×´Ì¬
			ESFG_CONNECTED = 0x00000002,
		};

		class CMEpoll
		{
		public:
			enum EEpollEvent
			{
				EEpollEvent_Read = 0,
				EEpollEvent_Write = 1,
				EEpollEvent_Error = 2,
				EEpollEvent_MAX
			};
			CMEpoll();
			~CMEpoll();
			int SetEpollEventHandle(EEpollEvent nEvent, std::function<int(int)> stEventHandle);
		public:
			//0--succ, -1----fail
			int EpollCreate(int nFdSize);
			int EpollWait();

			int EpollAdd(int nFd);
			int EpollDelete(int nFd);

			int SetNonBlock(int nFd);
			int SetNagleOff(int nFd);

			void SetEpollWaitingTime(int nWaitingTime);

		private:

			bool IsReadEvent(uint32_t unEvent);
			bool IsWriteEvent(uint32_t unEvent);
			bool IsErrorEvent(uint32_t unEvent);

			int OnEventHandle(EEpollEvent nEvent, int nFd);

		private:
			std::function<int(int)> m_pstEpollEventHandle[EEpollEvent_MAX];

		private:
			int m_nEpollFd;
			int m_nEpollEventSize;
			int m_nEpollWaitingTime;
			std::shared_ptr<epoll_event> m_pstEpollEvent;
			epoll_event m_stOneEpollEvent;
		};

	}
}
#endif 
