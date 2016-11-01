#include "CMListen.h"
#include <arpa/inet.h>

namespace CMLib
{
	namespace Socket
	{
		int CMListen::Init(const std::string& strIp, uint16_t unPort, 
			uint32_t unSendBufferSize)
		{
			m_strIp = strIp;
			m_nPort = unPort;
			return 0;
		}

		int CMListen::CreateTcpSocket()
		{
			m_nListenFd = socket(AF_INET, SOCK_STREAM, 0);
			if (m_nListenFd < 0)
			{
				return -1;
			}
			return 0;
		}

		int CMListen::Bind()
		{
			struct sockaddr_in stSocketAddr;
			memset(&stSocketAddr, 0, sizeof(stSocketAddr));
			stSocketAddr.sin_family = AF_INET;
			stSocketAddr.sin_port = htons(m_nPort);
			stSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);

			int nRet = ::bind(m_nListenFd, (struct sockaddr *)&stSocketAddr, sizeof(stSocketAddr));
			if (nRet < 0)
			{
				//		TLogError("bind %d failed, %s ", ushPort, strerror(errno));
				return -1;
			}
			//	TLogTrace("bind %d OK ", ushPort);

			return 0;
		}

		int CMListen::Listen()
		{
			int nRet = ::listen(m_nListenFd, 5);
			if (nRet < 0)
			{
				//		TLogError("listen %d connection error! ", 1024);
				return -1;
			}
			m_nState = ELST_listen;

			return 0;
		}

		int CMListen::SetForEpoll()
		{
			int nFlag = 1;
			setsockopt(m_nListenFd, SOL_SOCKET, SO_REUSEADDR, &nFlag, sizeof(nFlag));
			setsockopt(m_nListenFd, SOL_SOCKET, SO_KEEPALIVE, &nFlag, sizeof(nFlag));

			struct linger stLinger;
			stLinger.l_onoff = 0;
			stLinger.l_linger = 0;
			setsockopt(m_nListenFd, SOL_SOCKET, SO_LINGER, &stLinger, sizeof(stLinger));

			return 0;
		}

		int CMListen::GetFd() const
		{
			return m_nListenFd;
		}

		void CMListen::SetListenState(int nState)
		{
			m_nState = nState;
		}

		int CMListen::GetListenState() const
		{
			return m_nState;
		}

		int CMListen::Connect(uint32_t nTimeOut)
		{
			struct sockaddr_in serv_addr;
			bzero(&serv_addr, sizeof(struct sockaddr_in));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(m_nPort);
			serv_addr.sin_addr.s_addr = inet_addr(m_strIp.c_str());

			time_t nBegin = time(NULL);
			while (connect(m_nListenFd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
			{
				usleep(100);
				if (time(NULL)-nBegin >= nTimeOut)
				{
					printf("connect %s error!\n", m_strIp.c_str());
					return -1;
				}
			}
			return 0;
		}

		const char* CMListen::GetStrIp() const
		{
			return m_strIp.c_str();
		}

		uint16_t CMListen::GetPort() const
		{
			return m_nPort;
		}

	}
}
