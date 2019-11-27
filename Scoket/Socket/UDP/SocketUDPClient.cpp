#include "SocketUDPClient.h"
CSocketUDP::CSocketUDP(const std::string &IpAddr, const std::string &PortNum):CScoketBase(IpAddr, PortNum)
{

	m_ClientType = CScoketBase::CLIENTTYPE::Client;
	m_SokcetType = CScoketBase::SOCKTTYPE::UDP;
	this->m_sockthandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	this->m_RemoteAddr.sin_family = AF_INET;
	this->m_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(m_IpAddr.c_str());
	this->m_RemoteAddr.sin_port = htons((atoi(m_PortNum.c_str())));
	::setsockopt(this->m_sockthandle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&m_Sendtimeout), sizeof(m_Sendtimeout));
	::setsockopt(this->m_sockthandle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&m_Recvitimeout), sizeof(m_Recvitimeout));
	::InitializeCriticalSection(&m_HeartbeatandSendcritical);
}


CSocketUDP::~CSocketUDP()
{
	::DeleteCriticalSection(&m_HeartbeatandSendcritical);
	DisConnect();
}

bool CSocketUDP::Connect()
{
	if (this->m_sockthandle!=INVALID_SOCKET)
	{
		m_ConnectFlag = true;
		return true;
	}
	else
	{
		return false;
	}
	
}

bool CSocketUDP::DisConnect()
{
	if (this->m_sockthandle != INVALID_SOCKET)
	{
		m_ConnectFlag = false;
		if (CScoketBase::m_HeartbeatEnable)
		{
			DisableClientHeartbeat();
		}
		m_RecviProcessThreadRunControl = false;
		if (m_RecviProcessThread != nullptr && m_RecviProcessThread->joinable())
		{
			m_RecviProcessThread->join();
		}
		closesocket(this->m_sockthandle);
		return true;
	}
	return false;
}

bool CSocketUDP::Isopen()
{
	return m_ConnectFlag;
}

bool CSocketUDP::IsConnect()
{
	return m_ConnectFlag;
}

int CSocketUDP::SendData(const byte * senddata, size_t SendNum)
{
	CriticalSectionLockGuardian heartbeat(m_HeartbeatandSendcritical);
	return ::sendto(this->m_sockthandle, reinterpret_cast<const char*>(senddata),SendNum, 0, (SOCKADDR*)&this->m_RemoteAddr, sizeof(this->m_RemoteAddr));
}

int CSocketUDP::Recvi(byte * recvidata, size_t recvilength)
{
	int Addrlen = sizeof(this->m_RemoteAddr);
	return ::recvfrom(this->m_sockthandle, reinterpret_cast<char*>(recvidata), recvilength,0, (SOCKADDR*)&this->m_RemoteAddr, &Addrlen);
}

int CSocketUDP::RecviAsync(byte * recvidata, size_t recvilength)
{

	return 0;
}

bool CSocketUDP::GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN & clientinforget)
{

	return false;
}

bool CSocketUDP::ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction)
{

	if (CallBackfunction && IsConnect())
	{
		m_RecviCallBackFunction = CallBackfunction;
		if (!m_RecviProcessThread)
		{
			m_RecviProcessThreadRunControl = true;
			m_RecviProcessThread = std::make_shared<std::thread>(&CSocketUDP::RecviThreadProcessFunction, this, this);
			FD_SET(m_sockthandle, &m_Recvifdset);
		}
		return true;
	}
	return false;
}


bool CSocketUDP::GetSyncReadAndRecviTimeOut(int & Sendtimeout, int & Recvitimeout) const
{
	int oplen = sizeof(int);
	int retsend = ::getsockopt(this->m_sockthandle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&Sendtimeout), &oplen);
	int retrecv = ::getsockopt(this->m_sockthandle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&Recvitimeout), &oplen);
	return ((retsend == 0) && (retrecv == 0));
}

bool CSocketUDP::EnableClientHeartbeat()
{
	CScoketBase::m_HeartbeatEnable = true;
	return OPenHeartbeartThread();
}

bool CSocketUDP::DisableClientHeartbeat()
{
	CScoketBase::m_HeartbeatEnable = false;
	return CloseHeartbeartThread();
}

void CSocketUDP::RecviThreadProcessFunction(CSocketUDP * ClassParam)
{
	timeval ReadTimeOut = { 0 };
	ReadTimeOut.tv_sec = 0;
	ReadTimeOut.tv_usec = 1000;//1 ms
	char recvibuf[20000] = { 0 };
	int Addrlen = sizeof(this->m_RemoteAddr);
	if (m_RecviCallBackFunction != nullptr && m_sockthandle != INVALID_SOCKET)
	{
		while (m_RecviProcessThreadRunControl)
		{
			/*auto start = system_clock::now();*/
			int retcode = ::select(0, &m_Recvifdset, nullptr, nullptr, &ReadTimeOut);
			if (retcode == 0)
			{
				/*auto end = system_clock::now();
				std::cout << "cost time(milliseconds): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;*/
				FD_SET(m_sockthandle, &m_Recvifdset);//the timeout will clean the FD_Set struct
				DEBUGMSG("select is time out")
					continue;
			}
			else if (retcode == SOCKET_ERROR)
			{
#ifdef _DEBUG
				int errorcode = GetLastError();
				std::stringstream debuginfo;
				debuginfo << "select function occur error!!!" << std::endl << "error code is " << errorcode;
				DEBUGMSG(debuginfo.str())
#endif // _DEBUG
					FD_SET(m_sockthandle, &m_Recvifdset);
				continue;
			}
			if (FD_ISSET(this->m_sockthandle, &m_Recvifdset))
			{
				ZeroMemory(recvibuf, 20000);
				int Recvilen = ::recvfrom(this->m_sockthandle, recvibuf, 20000, 0, (SOCKADDR*)&this->m_RemoteAddr, &Addrlen);
				if (m_RecviCallBackFunction)
				{
					m_RecviCallBackFunction(recvibuf, Recvilen, 0);
				}
				
			}
		}
	}

}

void CSocketUDP::HeartbeatThreadProcessFunction(CSocketUDP * ClassParam)
{
	while (CScoketBase::m_HeartbeatEnable)
	{
		RunTimeMeasure timestart;
		std::this_thread::sleep_for(std::chrono::milliseconds(m_SendTimeInterval));
		CriticalSectionLockGuardian heartbeat(ClassParam->m_HeartbeatandSendcritical);
		::sendto(this->m_sockthandle, m_HeartbeatData.c_str(), m_HeartbeatData.length(), 0, (SOCKADDR*)&this->m_RemoteAddr, sizeof(this->m_RemoteAddr));
		timestart.PrintTimePassed(RunTimeMeasure::TIMEUNIT::milliseconds, "cost time(milliseconds):");
	}
}

bool CSocketUDP::OPenHeartbeartThread()
{
	if (m_ConnectFlag && (!m_ClientHeartbeatSendThread))
	{
		m_ClientHeartbeatSendThread = std::make_shared<std::thread>(&CSocketUDP::HeartbeatThreadProcessFunction, this, this);
		return true;
	}
	return false;
}

bool CSocketUDP::CloseHeartbeartThread()
{
	if (m_ClientHeartbeatSendThread && m_ClientHeartbeatSendThread->joinable())
	{
		m_ClientHeartbeatSendThread->join();
		m_ClientHeartbeatSendThread = nullptr;
		return true;
	}
	return false;
}
