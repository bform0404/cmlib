#ifndef __CM_TASK_QUEUE_H__
#define __CM_TASK_QUEUE_H__

#include <queue>

namespace CMLib
{
	namespace Thread
	{
		template<typename TaskT>
		class CMTaskQueue
		{
		public:
			CMTaskQueue() = default;
			~CMTaskQueue() = default;
			CMTaskQueue(CMTaskQueue&& other) = delete;
			CMTaskQueue& operator=(CMTaskQueue&& other) = delete;
			CMTaskQueue(const CMTaskQueue&) = delete;
			CMTaskQueue& operator=(const CMTaskQueue&) = delete;

			void Push(const TaskT& task)
			{
				m_stTasks.push(task);
			}

			void Push(TaskT&& task)
			{
				m_stTasks.push(task);
			}

			bool Pop(TaskT& task)
			{
				if (!m_stTasks.empty())
				{
					task = m_stTasks.front();
					m_stTasks.pop();
					return true;
				}
				else
				{
					return false;
				}
			}

			bool Empty() const
			{
				return m_stTasks.empty();
			}

			int Size() const
			{
				return m_stTasks.size();
			}

		private:
			std::queue<TaskT> m_stTasks;
		};

	}
} 

#endif
