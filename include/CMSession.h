#ifndef __CM_SESSION_H__
#define __CM_SESSION_H__

#include "CMDef.h"
#include "CMBuffer.h"

namespace CMLib
{
	namespace Socket
	{
		class CMSession
		{
		public:
			CMSession(const char* pszIp, uint16_t unPort);
			~CMSession();

			const char* GetIp() const;
			int			GetPort() const;
			int8_t		GetFlag() const;
			int32_t		GetSessionId() const;
			std::shared_ptr<CMBuffer>	GetBuffer();

			void		SetIp(const char* pszIp);
			void		SetPort(int nPort);
			void		SetFlag(int8_t nFlag);
			void		SetSessionId(int32_t nSessionId);
		private:
			int32_t m_nSessionId;
			uint16_t m_nPort;
			int8_t m_nFlag;
			std::string m_strIp;
			std::shared_ptr<CMBuffer> m_pstBuffer;
		};
	}
}

#endif
