#ifndef __CM_LISTEN_H__
#define __CM_LISTEN_H__

#include "CMDef.h"
#include <sys/types.h>
#include <sys/socket.h>

namespace CMLib
{
	namespace Socket
	{
		enum ELISTENSTATE
		{
			ELST_unlisten = 0,
			ELST_listen = 1,
			ELST_connect = 2
		};

		class CMListen
		{
		public:
			CMListen() { m_nState = ELST_unlisten; }
			~CMListen() {}
			int Init(const std::string& strIp, uint16_t unPort,
				uint32_t unSendBufferSize);

			int CreateTcpSocket();
			int Bind();
			int Listen();

			int SetForEpoll();
			int GetFd() const;

			void	SetListenState(int nState);
			int		GetListenState() const;

			int Connect(uint32_t nTimeOut);

			const char* GetStrIp() const;
			uint16_t	GetPort() const;
		private:
			int			m_nListenFd;
			uint16_t	m_nPort;
			int			m_nState;
			uint32_t	m_unSendBufferSize;
			std::string m_strIp;
		};

	}
}

#endif
