#include "CMSession.h"

namespace CMLib
{
	namespace Socket
	{
		CMSession::CMSession(const char* pszIp, uint16_t unPort)
		{
			m_strIp = pszIp;
			m_nPort = unPort;
			m_pstBuffer = std::make_shared<CMBuffer>();
			m_pstBuffer->Extend(MAX_RECV_BUFFER_LEN);
			m_nFlag = 0;
		}

		CMSession::~CMSession()
		{

		}

		const char* CMSession::GetIp() const
		{
			return m_strIp.c_str();
		}

		int CMSession::GetPort() const
		{
			return m_nPort;
		}

		int8_t CMSession::GetFlag() const
		{
			return m_nFlag;
		}

		int32_t CMSession::GetSessionId() const
		{
			return m_nSessionId;
		}


		std::shared_ptr<CMBuffer> CMSession::GetBuffer()
		{
			return m_pstBuffer;
		}

		void CMSession::SetIp(const char* pszIp)
		{
			m_strIp = pszIp;
		}

		void CMSession::SetPort(int nPort)
		{
			m_nPort = nPort;
		}

		void CMSession::SetFlag(int8_t nFlag)
		{
			m_nFlag = nFlag;
		}

		void CMSession::SetSessionId(int32_t nSessionId)
		{
			m_nSessionId = nSessionId;
		}

	}
}