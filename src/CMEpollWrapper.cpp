#include "CMEpollWrapper.h"
#include "CMSocketHelp.h"

namespace CMLib
{
	namespace Socket
	{
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		int CMEpollWrapper::_CreateEpoll(int nFdSize)
		{
			if (m_stEpoll.EpollCreate(nFdSize) < 0)
			{
				//
				return -1;
			}

			m_stEpoll.SetEpollWaitingTime(1);

			m_stEpoll.SetEpollEventHandle(CMEpoll::EEpollEvent_Read, std::bind(&CMEpollWrapper::OnReadHandle, this, std::placeholders::_1));
			m_stEpoll.SetEpollEventHandle(CMEpoll::EEpollEvent_Write, std::bind(&CMEpollWrapper::OnWriteHandle, this, std::placeholders::_1));
			m_stEpoll.SetEpollEventHandle(CMEpoll::EEpollEvent_Error, std::bind(&CMEpollWrapper::OnErrorHandle, this, std::placeholders::_1));

			return 0;
		}

		void CMEpollWrapper::_SetListening(int nFd)
		{
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				iter->second->SetFlag(ESFG_LISTENING);
			}
		}

		void CMEpollWrapper::_SetConnected(int nFd)
		{
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				iter->second->SetFlag(ESFG_CONNECTED);
			}
		}

		bool CMEpollWrapper::_IsListening(int nFd)
		{
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				return iter->second->GetFlag() & ESFG_LISTENING;
			}

			return false;
		}

		bool CMEpollWrapper::_IsConnected(int nFd)
		{
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				return iter->second->GetFlag() & ESFG_CONNECTED;
			}

			return false;
		}

		std::shared_ptr<CMSession> CMEpollWrapper::GetSession(int nFd)
		{
			std::shared_ptr<CMSession> stSession;

			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				stSession = iter->second;
			}

			return stSession;
		}

		bool CMEpollWrapper::Init(const char* szIp, unsigned short nPort,
			std::shared_ptr<CMNetHandle> stHandle)
		{
			m_stListen.Init(szIp, nPort, MAX_EPOLL_FDZISE);
			m_pstNetHandle = stHandle;

			m_stThread.Begin();

			return true;
		}

		bool CMEpollWrapper::Stop()
		{
			m_bRunning = false;
			m_stThread.End();
			return true;
		}

		int CMEpollWrapper::StartListen()
		{
			if (_CreateEpoll() < 0)
			{
				return -1;
			}

			if (_CreateListen() < 0)
			{
				return -1;
			}
			
			m_bRunning = true;
			Loop();
			
			return 0;
		}

		std::shared_ptr<CMSession> CMEpollWrapper::_AddFd(int nFd,
			const std::string strIp, uint16_t unPort)
		{
			std::shared_ptr<CMSession> stSession;
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				iter->second->SetIp(strIp.c_str());
				iter->second->SetPort(unPort);
				stSession = iter->second;
			}
			else
			{
				stSession = std::make_shared<CMSession>(strIp.c_str(), unPort);
				stSession->SetSessionId(_MakeSessionId(nFd));
				m_stSessions[nFd] = stSession;
				m_stEpoll.EpollAdd(nFd);
			}

			return stSession;
		}

		CMEpollWrapper::CMEpollWrapper()
		{
			m_pstNetHandle = nullptr;
			m_nFlag = 0;
			m_bRunning = false;
			m_bClient = false;
		}

		CMEpollWrapper::~CMEpollWrapper()
		{

		}

		int CMEpollWrapper::_CreateListen()
		{
			if (m_stListen.CreateTcpSocket() < 0)
			{
				//
				return -1;
			}
			if (m_stListen.SetForEpoll() < 0)
			{
				//
				return -1;
			}
			if (m_stListen.Bind() < 0)
			{
				//
				return -1;
			}
			if (m_stListen.Listen() < 0)
			{
				//
				return -1;
			}
			int nListenFd = m_stListen.GetFd();
			_AddFd(nListenFd, "", 0);
			_SetListening(nListenFd);

			return 0;
		}


		int CMEpollWrapper::OnReadHandle(int nFd)
		{
			if (_IsListening(nFd))
			{
				std::string strIp;
				uint16_t unPort = 0;
				int nClientFd = CMSocketHelp::OnAccept(nFd, strIp, unPort);
				if (nClientFd > 0)
				{
					std::shared_ptr<CMSession> stSession = _AddFd(nClientFd, strIp, unPort);
					_SetConnected(nClientFd);
					if (m_pstNetHandle)
					{
						m_pstNetHandle->OnAccept(std::make_shared<CMRequest>(stSession));
					}
				}
			}
			else if (_IsConnected(nFd))
			{
				std::shared_ptr<CMSession> stSession = GetSession(nFd);
				if (!stSession.get())
				{
					return -1;
				}
				char* szMsg = new char[MAX_RECV_LEN];
				int nRecvLen = CMSocketHelp::OnRecv(nFd, szMsg, MAX_RECV_LEN);
				if (nRecvLen > 0)
				{
					stSession->GetBuffer()->Write(szMsg, nRecvLen);
				}

				std::shared_ptr<CMRequest> stMsg = std::make_shared<CMRequest>(stSession);
				if (nRecvLen > 0)
				{
					m_pstNetHandle->OnRecv(stMsg);
				}
				else if (nRecvLen == -2)
				{
					m_pstNetHandle->OnClose(stMsg);
					_CloseFd(nFd);
				}
				delete[]szMsg;
			}

			return 0;
		}

		int CMEpollWrapper::OnWriteHandle(int nFd)
		{

			return 0;
		}

		int CMEpollWrapper::OnErrorHandle(int nFd)
		{
			if (_IsListening(nFd))
			{
				//
				m_stListen.SetListenState(ELST_unlisten);
				return -1;
			}
			return 0;
		}

		void CMEpollWrapper::_CloseFd(int nFd)
		{
			auto iter = m_stSessions.find(nFd);
			if (iter != m_stSessions.end())
			{
				m_stSessions.erase(iter);
			}
			m_stEpoll.EpollDelete(nFd);
			CMSocketHelp::OnClose(nFd);
		}

		int32_t CMEpollWrapper::_MakeSessionId(int nFd)
		{
			return CMSessionId(nFd, m_nFlag++).getSid();
		}

		void CMEpollWrapper::Loop()
		{
			m_stThread.PostTask(std::bind(&CMEpollWrapper::IOThreadMain, this));
		}


		void CMEpollWrapper::IOThreadMain()
		{
			while (m_bRunning)
			{
				m_stEpoll.EpollWait();
				usleep(10);
			}
		}

		int CMEpollWrapper::SendResponse(std::shared_ptr<CMResponse> pstMsg)
		{
			int nRet = -1;
			if (!pstMsg)
				return nRet;

			int nFd = 0;
			std::shared_ptr<CMSession> stSession;
			
			int32_t nSessionId = pstMsg->GetSessionId();
			auto iter = m_stSessions.find(CMSessionId(nSessionId).getSocketId());
			if (iter != m_stSessions.end() &&
				iter->second->GetSessionId() == nSessionId)
			{
				nFd = iter->first;
				stSession = iter->second;
			}

			if (nFd > 0)
			{
				nRet = CMSocketHelp::OnSend(nFd, pstMsg->GetRespMsg(), pstMsg->GetMsgLen());
				if (nRet == -2)
				{
					m_pstNetHandle->OnClose(std::make_shared<CMRequest>(stSession));
					_CloseFd(nFd);
				}
			}
			return nRet;
		}
		

	}
}