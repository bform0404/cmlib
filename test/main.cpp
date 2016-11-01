#include <stdio.h>
#include "CMEpollWrapper.h"
#include "CMDef.h"
#include <memory>

using namespace CMLib::Socket;

CMEpollWrapper stEpollWrapper;


class TestHandle : public CMNetHandle
{
public:
	virtual void OnAccept(std::shared_ptr<CMRequest> stAppMsg)
	{
		printf("OnAccept\n");
	}
	virtual void OnRecv(std::shared_ptr<CMRequest> stAppMsg)
	{
		std::shared_ptr<CMSession> pstSession = stAppMsg->GetSession();
		std::shared_ptr<CMBuffer> pstBuffer = pstSession->GetBuffer();
		uint32_t nLen = pstBuffer->GetBufferLen();
		if (nLen < 4)
		{
			//不是完整包
			return;
		}

		uint8_t* szMsg = new uint8_t[nLen];
		if (nLen != pstBuffer->Read(szMsg, nLen))
		{
			delete[]szMsg;
			return;
		}

		printf("recv %d:%s\n", pstSession->GetSessionId(), szMsg);

		delete[]szMsg;
		std::shared_ptr<CMResponse> pstMsg = std::make_shared<CMResponse>(pstSession->GetSessionId(), "shit,gun", 8);
		stEpollWrapper.SendResponse(pstMsg);
		
	}
	virtual void onTimeOut(std::shared_ptr<CMRequest> stAppMsg)
	{
		printf("onTimeOut\n");
	}
	virtual void OnClose(std::shared_ptr<CMRequest> stAppMsg)
	{
		printf("OnClose\n");
	}
};

int main()
{
	
	stEpollWrapper.Init("115.28.28.25", 9999, std::make_shared<TestHandle>());
	stEpollWrapper.StartListen();

	while (true)
	{
		sleep(5);
	}
	return 0;
}