#ifndef __CM_BUFFER_H__
#define __CM_BUFFER_H__

#include "CMDef.h"

namespace CMLib
{
	namespace Socket
	{
		class CMBuffer
		{
		public:
			CMBuffer(bool bAutoIncrease = false);
			~CMBuffer();
			uint8_t*  GetBuffer() { return m_buffer; }
			uint32_t GetAllocSize() { return m_alloc_size; }
			uint32_t GetBufferLen() { return m_write_offset; }
			void IncWriteOffset(uint32_t len) { m_write_offset += len; }

			void Extend(uint32_t len);
			uint32_t Write(void* buf, uint32_t len);
			uint32_t Read(void* buf, uint32_t len);
		private:
			bool		m_bAutoIncrease;
			uint8_t*	m_buffer;
			uint32_t	m_alloc_size;
			uint32_t	m_write_offset;
		};

	}
}


#endif /* UTILPDU_H_ */
