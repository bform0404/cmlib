#ifndef __CM_STREAM_H__
#define __CM_STREAM_H__

#include <stdint.h>
#include <memory>
#include "CMBuffer.h"

namespace CMLib
{
	namespace Stream
	{
		class CMByteStream
		{
		public:
			CMByteStream(uint8_t* buf, uint32_t len);
			CMByteStream(std::shared_ptr<Socket::CMBuffer> pSimpBuf, uint32_t pos);
			~CMByteStream() {}

			uint8_t* GetBuf() { return m_pBuf; }
			uint32_t GetPos() { return m_pos; }
			uint32_t GetLen() { return m_len; }
			bool Skip(uint32_t len)
			{
				if (m_pos + len > m_len)
					return false;
				m_pos += len;
				return true;
			}

			void operator << (int8_t data);
			void operator << (uint8_t data);
			void operator << (int16_t data);
			void operator << (uint16_t data);
			void operator << (int32_t data);
			void operator << (uint32_t data);
			void operator << (uint64_t data);

			void operator >> (int8_t& data);
			void operator >> (uint8_t& data);
			void operator >> (int16_t& data);
			void operator >> (uint16_t& data);
			void operator >> (int32_t& data);
			void operator >> (uint32_t& data);
			void operator >> (uint64_t& data);

			void WriteString(const char* str);
			void WriteString(const char* str, uint32_t len);
			char* ReadString(uint32_t& len);

			void WriteData(uint8_t* data, uint32_t len);
			uint8_t* ReadData(uint32_t& len);
		private:
			void _WriteByte(void* buf, uint32_t len);
			void _ReadByte(void* buf, uint32_t len);
		private:
			std::shared_ptr<Socket::CMBuffer>	m_pSimpBuf;
			uint8_t*		m_pBuf;
			uint32_t		m_len;
			uint32_t		m_pos;
		};

		class CMStreamHelp
		{
			static int16_t ReadInt16(uint8_t* buf);
			static uint16_t ReadUint16(uint8_t* buf);
			static int32_t ReadInt32(uint8_t* buf);
			static uint32_t ReadUint32(uint8_t* buf);
			static void WriteInt16(uint8_t* buf, int16_t data);
			static void WriteUint16(uint8_t* buf, uint16_t data);
			static void WriteInt32(uint8_t* buf, int32_t data);
			static void WriteUint32(uint8_t* buf, uint32_t data);
			static void WriteUint64(uint8_t* buf, uint64_t data);
		};
	}
}

#endif

