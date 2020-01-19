#include "ScoketTCPClient.h"
CScoketTCPClient::CScoketTCPClient(const std::string &IpAddr, const std::string &PortNum)
	:CScoketBase(IpAddr, PortNum)
{
	m_ClientType = CScoketBase::CLIENTTYPE::Client;
	m_SokcetType = CScoketBase::SOCKTTYPE::TCP;
	this->m_sockthandle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->m_RemoteAddr.sin_family = AF_INET;
	this->m_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(m_IpAddr.c_str());
	this->m_RemoteAddr.sin_port = htons(std::atoi(m_PortNum.c_str()));
	::setsockopt(this->m_sockthandle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&m_Sendtimeout),  sizeof(m_Sendtimeout));
	::setsockopt(this->m_sockthandle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&m_Recvitimeout), sizeof(m_Recvitimeout));
	::InitializeCriticalSection(&m_HeartbeatandSendcritical);

}


CScoketTCPClient::~CScoketTCPClient()
{

	::DeleteCriticalSection(&m_HeartbeatandSendcritical);
	DisConnect();
}

bool CScoketTCPClient::Connect()
{
	if (this->m_sockthandle == INVALID_SOCKET)
	{
		return false;
	}
	int ret = ::connect(this->m_sockthandle, (sockaddr*)(&this->m_RemoteAddr), sizeof(this->m_RemoteAddr));
	if (ret == SOCKET_ERROR)
	{
#ifdef _DEBUG
		int errorcode = ::WSAGetLastError();
		std::stringstream debuginfo;
		debuginfo << "Connect TCP Server fail the IPAddress is !!!" << m_IpAddr << "the Port is " << m_PortNum << "Error code :" << errorcode;
		DEBUGMSG(debuginfo.str())
#endif // _DEBUG
		closesocket(this->m_sockthandle);
		return false;
	}
	this->m_ConnectFlag = true;
	if (m_HeartbeatEnable)
	{
		OPenHeartbeartThread();
	}
	return true;
}

bool CScoketTCPClient::DisConnect()
{
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
	return false;
}

bool CScoketTCPClient::Isopen()
{
	return this->m_ConnectFlag;
}

bool CScoketTCPClient::IsConnect()
{
	return this->m_ConnectFlag;
}

int CScoketTCPClient::SendData(const byte* senddata, size_t SendNum)
{
	CriticalSectionLockGuardian heartbeat(this->m_HeartbeatandSendcritical);
	return ::send(this->m_sockthandle, reinterpret_cast<const char*>(senddata), SendNum, 0);
}

int CScoketTCPClient::Recvi(byte *recvidata, size_t recvilength)
{
	int Recvilen = { 0 };
	Recvilen = ::recv(this->m_sockthandle, reinterpret_cast<char*>(recvidata), recvilength, 0);
	return Recvilen;
}

bool CScoketTCPClient::GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget)
{

	return false;
}

bool CScoketTCPClient::ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction)
{
	if (CallBackfunction && IsConnect())
	{
		m_RecviCallBackFunction = CallBackfunction;
		if (!m_RecviProcessThread)
		{
			m_RecviProcessThreadRunControl = true;
			m_RecviProcessThread = std::make_shared<std::thread>(&CScoketTCPClient::RecviThreadProcessFunction, this,this);
			FD_SET(m_sockthandle, &m_Recvifdset);
		}
		return true;
	}
	return false;
}
bool CScoketTCPClient::GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const
{
	int oplen = sizeof(int);
	int retsend = ::getsockopt(this->m_sockthandle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char*>(&Sendtimeout), &oplen);
	int retrecv = ::getsockopt(this->m_sockthandle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&Recvitimeout), &oplen);
	return ((retsend == 0) && (retrecv == 0));
}
bool CScoketTCPClient::EnableClientHeartbeat()
{
	CScoketBase::m_HeartbeatEnable = true;
	return OPenHeartbeartThread();
}
bool CScoketTCPClient::DisableClientHeartbeat()
{
	CScoketBase::m_HeartbeatEnable = false;
	return CloseHeartbeartThread();
	
}
void CScoketTCPClient::RecviThreadProcessFunction(CScoketTCPClient *ClassParam)
{
	timeval ReadTimeOut = {0};
	ReadTimeOut.tv_sec = 0;
	ReadTimeOut.tv_usec = 1000;//1 ms
	char recvibuf[20000] = {0};
	if (m_RecviCallBackFunction!=nullptr && m_sockthandle != INVALID_SOCKET)
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
				int Recvilen = recv(this->m_sockthandle, recvibuf, 20000, 0);
				if (m_RecviCallBackFunction)
				{
					m_RecviCallBackFunction(recvibuf, Recvilen, 0);
				}
			}
		}
	}


}

void CScoketTCPClient::HeartbeatThreadProcessFunction(CScoketTCPClient * ClassParam)
{
	while (CScoketBase::m_HeartbeatEnable)
	{
		RunTimeMeasure timestart;
		std::this_thread::sleep_for(std::chrono::milliseconds(m_SendTimeInterval));
		CriticalSectionLockGuardian heartbeat(ClassParam->m_HeartbeatandSendcritical);
		::send(this->m_sockthandle, m_HeartbeatData.c_str(), m_HeartbeatData.length(), 0);
		timestart.printTimePassed(RunTimeMeasure::TIMEUNIT::milliseconds, "cost time(milliseconds):");
	}
}

bool CScoketTCPClient::OPenHeartbeartThread()
{
	if (m_ConnectFlag && (!m_ClientHeartbeatSendThread))
	{
		m_ClientHeartbeatSendThread = std::make_shared<std::thread>(&CScoketTCPClient::HeartbeatThreadProcessFunction, this, this);
		return true;
	}
	return false;
}

bool CScoketTCPClient::CloseHeartbeartThread()
{
	if (m_ClientHeartbeatSendThread && m_ClientHeartbeatSendThread->joinable())
	{
		m_ClientHeartbeatSendThread->join();
		m_ClientHeartbeatSendThread = nullptr;
		return true;
	}
	return false;
	
}
