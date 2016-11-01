#include "CMSessionId.h"

namespace CMLib
{
	namespace Socket
	{

		bool CMSessionId::operator!=(const CMSessionId& src) const
		{
			return !(*this == src);
		}

		bool CMSessionId::operator==(const CMSessionId& src) const
		{
			return (m_nSocketId == src.m_nSocketId&&m_nSeqId == src.m_nSeqId);
		}

		const CMSessionId& CMSessionId::operator=(const CMSessionId& src)
		{
			if (this != &src) {
				m_nSeqId = src.m_nSeqId;
				m_nSocketId = src.m_nSocketId;
			}
			return *this;
		}

		CMSessionId::CMSessionId(const CMSessionId& src)
		{
			m_nSeqId = src.m_nSeqId;
			m_nSocketId = src.m_nSocketId;
		}

		CMSessionId::CMSessionId(int nSocketId, uint8_t nSeqId) :m_nSocketId(nSocketId), m_nSeqId(nSeqId)
		{

		}

		CMSessionId::CMSessionId(int nSid)
		{
			m_nSocketId = nSid & 0xFFFFFF;
			m_nSeqId = (nSid & 0xFF000000) >> 24;
		}

		int CMSessionId::getSid() const
		{
			return (m_nSeqId << 24) | (m_nSocketId);
		}

		int CMSessionId::getSocketId() const
		{
			return m_nSocketId;
		}

		uint8_t CMSessionId::getSeqId() const
		{
			return m_nSeqId;
		}

		bool CMSessionId::isEmpty() const
		{
			return m_nSocketId == 0 && m_nSeqId == 0;
		}
	}
}
