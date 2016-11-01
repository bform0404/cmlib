#ifndef __CM_REQUEST_H__
#define __CM_REQUEST_H__

#include "CMSession.h"

namespace CMLib
{
	namespace Socket
	{
		class CMRequest
		{
		public:
			CMRequest(std::shared_ptr<CMSession> stSession);
			virtual ~CMRequest();
		public:
			std::shared_ptr<CMSession> GetSession() const;
		private:
			std::shared_ptr<CMSession> m_stSession;
		};
	}
}

#endif

