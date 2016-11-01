#include "CMRequest.h"

namespace CMLib
{
	namespace Socket
	{
		CMRequest::CMRequest(std::shared_ptr<CMSession> stSession)
		{
			m_stSession = stSession;
		}

		CMRequest::~CMRequest()
		{

		}

		std::shared_ptr<CMSession> CMRequest::GetSession() const
		{
			return m_stSession;
		}
	}
}