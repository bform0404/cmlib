#ifndef __CM_RESPONSE_H__
#define __CM_RESPONSE_H__

#include "CMSessionId.h"
#include <stdlib.h>
namespace CMLib
{
	namespace Socket
	{
		class CMResponse
		{
		public:
			virtual ~CMResponse();
			CMResponse(int32_t nSessionId, const char* szMsg, int nMsgLen);
			int32_t		GetSessionId() const;
			const char* GetRespMsg() const;
			int			GetMsgLen() const;
		private:
			int32_t	m_nSessionId;
			char*	m_szMsg;
			int		m_nMsgLen;
		};
	}
}


#endif
