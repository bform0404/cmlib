#include "CMResponse.h"
#include <string.h>

namespace CMLib
{
	namespace Socket
	{


		int CMResponse::GetMsgLen() const
		{
			return m_nMsgLen;
		}

		const char* CMResponse::GetRespMsg() const
		{
			return m_szMsg;
		}

		int32_t CMResponse::GetSessionId() const
		{
			return m_nSessionId;
		}

		CMResponse::CMResponse(int32_t nSessionId, const char* szMsg, int nMsgLen)
		{
			m_nSessionId = nSessionId;
			m_szMsg = new char[nMsgLen];
			memcpy(m_szMsg, szMsg, nMsgLen);
			m_nMsgLen = nMsgLen;
		}

		CMResponse::~CMResponse()
		{
			delete[]m_szMsg;
		}
	}
}
