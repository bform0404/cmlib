#ifndef __CM_EPOLL_WRAPPER_H__
#define __CM_EPOLL_WRAPPER_H__

#include "CMDef.h"
#include "CMEpoll.h"
#include "CMTaskThread.h"
#include "CMNetHandle.h"
#include "CMResponse.h"
#include "CMListen.h"

namespace CMLib
{
	namespace Socket
	{
		/************************************************************************/
		/*       Epoll包装接口                                                 */
		/************************************************************************/
		class CMEpollWrapper
		{
		public:
			CMEpollWrapper();
			~CMEpollWrapper();

			bool Init(const char* szIp, uint16_t nPort,
				std::shared_ptr<CMNetHandle> stHandle);
			int StartListen();

			bool Stop();

			int SendResponse(std::shared_ptr<CMResponse> pstMsg);
			
		protected:
			std::shared_ptr<CMSession> GetSession(int nFd);

		protected:
			void _SetListening(int nFd);
			void _SetConnected(int nFd);

			bool _IsListening(int nFd);
			bool _IsConnected(int nFd);

			std::shared_ptr<CMSession> _AddFd(int nFd, const std::string strIp, uint16_t unPort);
			void _CloseFd(int nFd);
		public:

			int OnReadHandle(int nFd);
			int OnWriteHandle(int nFd);
			int OnErrorHandle(int nFd);
			void IOThreadMain();
		protected:

			int _CreateEpoll(int nFdSize = MAX_EPOLL_FDZISE);
			int _CreateListen();
			int _CreateConnect();
			int OnSend();
			int32_t _MakeSessionId(int nFd);
		public:
			void Loop();
		private:
			Thread::CMTaskThread	m_stThread;
		private:
			//是否客户端，默认是服务端
			bool						m_bClient;
			uint8_t						m_nFlag;
			CMListen					m_stListen;
			CMEpoll						m_stEpoll;
			std::shared_ptr<CMNetHandle> m_pstNetHandle;
			std::map<int, std::shared_ptr<CMSession> > m_stSessions;
			bool						m_bRunning;
		};

	}
}

#endif
