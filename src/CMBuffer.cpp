#include "CMBuffer.h"
namespace CMLib
{
	namespace Socket
	{
		CMBuffer::CMBuffer(bool bAutoIncrease)
		{
			m_buffer = NULL;
			m_alloc_size = 0;
			m_write_offset = 0;
			m_bAutoIncrease = bAutoIncrease;
		}

		CMBuffer::~CMBuffer()
		{
			m_alloc_size = 0;
			m_write_offset = 0;
			if (m_buffer)
			{
				free(m_buffer);
				m_buffer = NULL;
			}
		}

		void CMBuffer::Extend(uint32_t len)
		{
			m_alloc_size = m_write_offset + len;
			m_alloc_size += m_alloc_size >> 2;	// increase by 1/4 allocate size
			uint8_t* new_buf = (uint8_t*)realloc(m_buffer, m_alloc_size);
			m_buffer = new_buf;
		}

		uint32_t CMBuffer::Write(void* buf, uint32_t len)
		{
			if (m_write_offset + len > m_alloc_size)
			{
				if (m_bAutoIncrease)
				{
					Extend(len);
				}
				else
				{
					return 0;
				}
			}

			if (buf)
			{
				memcpy(m_buffer + m_write_offset, buf, len);
			}

			m_write_offset += len;

			return len;
		}

		uint32_t CMBuffer::Read(void* buf, uint32_t len)
		{
			if (len > m_write_offset)
				len = m_write_offset;

			if (buf)
				memcpy(buf, m_buffer, len);

			m_write_offset -= len;
			memmove(m_buffer, m_buffer + len, m_write_offset);
			return len;
		}
	}
}
