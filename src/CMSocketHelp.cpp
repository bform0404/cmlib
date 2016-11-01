#include "CMSocketHelp.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

namespace CMLib
{
	namespace Socket
	{

		CMSocketHelp::CMSocketHelp()
		{

		}

		CMSocketHelp::~CMSocketHelp()
		{

		}

		int CMSocketHelp::OnAccept(int nFd, std::string& strClientIp, unsigned short& unPort)
		{
			struct sockaddr_in stSocketAddress;
			int nSocketAddressSize = sizeof(stSocketAddress);;
			int nClientFd = accept(nFd, (struct sockaddr *)&stSocketAddress, (socklen_t *)&nSocketAddressSize);
			if ((nClientFd < 0))
			{
				return -1;
			}

			strClientIp = inet_ntoa(stSocketAddress.sin_addr);
			unPort = ntohs(stSocketAddress.sin_port);

			return nClientFd;
		}
		//-1 错误 -2 关闭 0 缓冲区满 >0 接收字节数
		int CMSocketHelp::OnRecv(int nFd, char* szBuffer, int nMaxLen)
		{
			if (nFd < 0 || szBuffer == NULL)
			{
				return -1;
			}
			unsigned int nRecvOffset = 0;
			int nCanRecvLen = nMaxLen;
			bool bClose = false;
			int nRecvLen = 0;
			do
			{
				if (nCanRecvLen <= 0)
				{
					bClose = false;
					break;
				}
				nRecvLen = recv(nFd, szBuffer + nRecvLen, nCanRecvLen, 0);
				if (nRecvLen > 0)
				{
					nRecvOffset += nRecvLen;
					nCanRecvLen -= nRecvLen;
				}
				else if (nRecvLen < 0)
				{
					//错误
					int nErrNo = errno;
					if (nErrNo != EAGAIN)
					{
						bClose = true;
						break;
					}
				}
				else if (nRecvLen == 0)
				{
					bClose = true;
					break;
				}
			} while (nRecvLen > 0);

			if (bClose)
			{
				if (nRecvOffset == 0)
				{
					return -2;
				}
				else
				{
					return -1;
				}
			}
			return nRecvOffset;
		}
		//-1 错误 -2 关闭 0 缓冲区满 >0 发送字节数
		int CMSocketHelp::OnSend(int nFd, const char* szBuffer, int nMsgLen)
		{
			int nAllSendLen = 0;
			int nCanSendLen = nMsgLen;
			bool bClose = false;

			while (nCanSendLen > 0)
			{
				int nSendLen = send(nFd, szBuffer + nAllSendLen, nCanSendLen, 0);
				if (nSendLen <= 0)
				{
					int nErrNo = errno;
					if (nErrNo == EINTR)
					{
						continue;
					}
					//缓冲不足以发报文
					else if (nErrNo == EAGAIN)
					{
						bClose = false;
						break;
					}
					bClose = true;
					break;
				}
				nAllSendLen += nSendLen;
				nCanSendLen -= nSendLen;
			}
			if (nCanSendLen > 0)
			{
				//mei fawan 
			}
			if (bClose)
			{
				return -2;
			}

			return nAllSendLen;
		}

		int CMSocketHelp::OnClose(int nFd)
		{

			return close(nFd);
		}

	}
}