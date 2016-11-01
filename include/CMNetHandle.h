#ifndef __APP_NETHANDLE_H__
#define __APP_NETHANDLE_H__

#include "CMDef.h"
#include "CMRequest.h"

namespace CMLib
{
	namespace Socket
	{
		class CMNetHandle
		{
		public:
			virtual ~CMNetHandle(){};
			virtual void OnAccept(std::shared_ptr<CMRequest> stAppMsg) = 0;
			virtual void OnRecv(std::shared_ptr<CMRequest> stAppMsg) = 0;
			virtual void onTimeOut(std::shared_ptr<CMRequest> stAppMsg) = 0;
			virtual void OnClose(std::shared_ptr<CMRequest> stAppMsg) = 0;
		};
	}
}


#endif
