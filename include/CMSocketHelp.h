#ifndef __CM_SOCKET_HELP_H__
#define __CM_SOCKET_HELP_H__

#include "CMDef.h"

namespace CMLib
{
	namespace Socket
	{
		class CMSocketHelp
		{
		public:
			CMSocketHelp();
			~CMSocketHelp();

		public:
			static int OnAccept(int nFd, std::string& strClientIp, unsigned short& unPort);
			static int OnRecv(int nFd, char* szBuffer, int nMaxLen);
			static int OnSend(int nFd, const char* szBuffer, int nMsgLen);
			static int OnClose(int nFd);
		};

	}
}
#endif
