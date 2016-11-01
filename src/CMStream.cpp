#include "CMStream.h"

namespace CMLib
{
	namespace Stream
	{
		CMByteStream::CMByteStream(uint8_t* buf, uint32_t len)
		{
			m_pBuf = buf;
			m_len = len;
			m_pSimpBuf = NULL;
			m_pos = 0;
		}

		CMByteStream::CMByteStream(std::shared_ptr<Socket::CMBuffer> pSimpBuf, uint32_t pos)
		{
			m_pSimpBuf = pSimpBuf;
			m_pos = pos;
			m_pBuf = NULL;
			m_len = 0;
		}

		void CMByteStream::operator << (int8_t data)
		{
			_WriteByte(&data, 1);
		}

		void CMByteStream::operator << (uint8_t data)
		{
			_WriteByte(&data, 1);
		}

		void CMByteStream::operator << (int16_t data)
		{
			unsigned char buf[2];
			buf[0] = static_cast<uint8_t>(data >> 8);
			buf[1] = static_cast<uint8_t>(data & 0xFF);
			_WriteByte(buf, 2);
		}

		void CMByteStream::operator << (uint16_t data)
		{
			unsigned char buf[2];
			buf[0] = static_cast<uint8_t>(data >> 8);
			buf[1] = static_cast<uint8_t>(data & 0xFF);
			_WriteByte(buf, 2);
		}

		void CMByteStream::operator << (int32_t data)
		{
			unsigned char buf[4];
			buf[0] = static_cast<uint8_t>(data >> 24);
			buf[1] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[3] = static_cast<uint8_t>(data & 0xFF);
			_WriteByte(buf, 4);
		}

		void CMByteStream::operator << (uint32_t data)
		{
			unsigned char buf[4];
			buf[0] = static_cast<uint8_t>(data >> 24);
			buf[1] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[3] = static_cast<uint8_t>(data & 0xFF);
			_WriteByte(buf, 4);
		}


		void CMByteStream::operator<<(uint64_t data)
		{
			unsigned char buf[8];
			buf[0] = static_cast<uint8_t>((data >> 56) & 0xFF);
			buf[1] = static_cast<uint8_t>((data >> 48) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 40) & 0xFF);
			buf[3] = static_cast<uint8_t>((data >> 32) & 0xFF);
			buf[4] = static_cast<uint8_t>((data >> 24) & 0xFF);
			buf[5] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[6] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[7] = static_cast<uint8_t>(data & 0xFF);
			_WriteByte(buf, 8);
		}

		void CMByteStream::operator >> (int8_t& data)
		{
			_ReadByte(&data, 1);
		}

		void CMByteStream::operator >> (uint8_t& data)
		{
			_ReadByte(&data, 1);
		}

		void CMByteStream::operator >> (int16_t& data)
		{
			unsigned char buf[2];

			_ReadByte(buf, 2);

			data = (buf[0] << 8) | buf[1];
		}

		void CMByteStream::operator >> (uint16_t& data)
		{
			unsigned char buf[2];

			_ReadByte(buf, 2);

			data = (buf[0] << 8) | buf[1];
		}

		void CMByteStream::operator >> (int32_t& data)
		{
			unsigned char buf[4];

			_ReadByte(buf, 4);

			data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
		}

		void CMByteStream::operator >> (uint32_t& data)
		{
			unsigned char buf[4];

			_ReadByte(buf, 4);

			data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
		}


		void CMByteStream::operator >> (uint64_t& data)
		{
			unsigned char buf[8];

			_ReadByte(buf, 8);

			data = ((uint64_t)buf[0] << 56) | ((uint64_t)buf[1] << 48) | ((uint64_t)buf[2] << 40) | ((uint64_t)buf[3] << 32) | (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
		}

		void CMByteStream::WriteString(const char *str)
		{
			uint32_t size = str ? (uint32_t)strlen(str) : 0;

			*this << size;
			_WriteByte((void*)str, size);
		}

		void CMByteStream::WriteString(const char *str, uint32_t len)
		{
			*this << len;
			_WriteByte((void*)str, len);
		}

		char* CMByteStream::ReadString(uint32_t& len)
		{
			*this >> len;
			char* pStr = (char*)GetBuf() + GetPos();
			Skip(len);
			return pStr;
		}

		void CMByteStream::WriteData(uint8_t *data, uint32_t len)
		{
			*this << len;
			_WriteByte(data, len);
		}

		uint8_t* CMByteStream::ReadData(uint32_t &len)
		{
			*this >> len;
			uint8_t* pData = (uint8_t*)GetBuf() + GetPos();
			Skip(len);
			return pData;
		}

		void CMByteStream::_ReadByte(void* buf, uint32_t len)
		{
			if (m_pos + len > m_len)
			{

			}

			if (m_pSimpBuf)
				m_pSimpBuf->Read((char*)buf, len);
			else
				memcpy(buf, m_pBuf + m_pos, len);

			m_pos += len;
		}

		void CMByteStream::_WriteByte(void* buf, uint32_t len)
		{
			if (m_pBuf && (m_pos + len > m_len))
				return;

			if (m_pSimpBuf)
				m_pSimpBuf->Write((char*)buf, len);
			else
				memcpy(m_pBuf + m_pos, buf, len);

			m_pos += len;
		}

		/************************************************************************/
		/*                                                                      */
		/************************************************************************/

		int16_t CMStreamHelp::ReadInt16(uint8_t *buf)
		{
			int16_t data = (buf[0] << 8) | buf[1];
			return data;
		}

		uint16_t CMStreamHelp::ReadUint16(uint8_t* buf)
		{
			uint16_t data = (buf[0] << 8) | buf[1];
			return data;
		}

		int32_t CMStreamHelp::ReadInt32(uint8_t *buf)
		{
			int32_t data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
			return data;
		}

		uint32_t CMStreamHelp::ReadUint32(uint8_t *buf)
		{
			uint32_t data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
			return data;
		}

		void CMStreamHelp::WriteInt16(uint8_t *buf, int16_t data)
		{
			buf[0] = static_cast<uint8_t>(data >> 8);
			buf[1] = static_cast<uint8_t>(data & 0xFF);
		}

		void CMStreamHelp::WriteUint16(uint8_t *buf, uint16_t data)
		{
			buf[0] = static_cast<uint8_t>(data >> 8);
			buf[1] = static_cast<uint8_t>(data & 0xFF);
		}

		void CMStreamHelp::WriteInt32(uint8_t *buf, int32_t data)
		{
			buf[0] = static_cast<uint8_t>(data >> 24);
			buf[1] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[3] = static_cast<uint8_t>(data & 0xFF);
		}

		void CMStreamHelp::WriteUint32(uint8_t *buf, uint32_t data)
		{
			buf[0] = static_cast<uint8_t>(data >> 24);
			buf[1] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[3] = static_cast<uint8_t>(data & 0xFF);
		}


		void CMStreamHelp::WriteUint64(uint8_t* buf, uint64_t data)
		{
			buf[0] = static_cast<uint8_t>((data >> 56) & 0xFF);
			buf[1] = static_cast<uint8_t>((data >> 48) & 0xFF);
			buf[2] = static_cast<uint8_t>((data >> 40) & 0xFF);
			buf[3] = static_cast<uint8_t>((data >> 32) & 0xFF);
			buf[4] = static_cast<uint8_t>((data >> 24) & 0xFF);
			buf[5] = static_cast<uint8_t>((data >> 16) & 0xFF);
			buf[6] = static_cast<uint8_t>((data >> 8) & 0xFF);
			buf[7] = static_cast<uint8_t>(data & 0xFF);
		}
	}
}