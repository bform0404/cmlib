#ifndef __CM_SESSION_ID_H__
#define __CM_SESSION_ID_H__

#include <stdint.h>

namespace CMLib
{
	namespace Socket
	{
		class CMSessionId
		{
		public:
			CMSessionId(int nSocketId, uint8_t nSeqId);

			CMSessionId(int nSid);

			CMSessionId(const CMSessionId& src);

			const CMSessionId& operator=(const CMSessionId& src);

			bool operator==(const CMSessionId& src) const;

			bool operator!=(const CMSessionId& src) const;

			int getSid() const;

			int getSocketId() const;

			uint8_t getSeqId() const;

			bool isEmpty() const;

		private:
			int m_nSocketId;
			int m_nSeqId;

		};
	}
}
#endif


