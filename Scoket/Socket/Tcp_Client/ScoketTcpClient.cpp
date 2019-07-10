#include "ScoketTCPClient.h"
CScoketTCPClient::CScoketTCPClient(const std::string &IpAddr, const std::string &PortNum)
	:CScoketBase(IpAddr, PortNum)
{
	m_ClientType = CScoketBase::CLIENTTYPE::Client;
	m_SokcetType = CScoketBase::SOCKTTYPE::TCP;
	this->m_sockthandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->m_RemoteAddr.sin_family = AF_INET;
	this->m_RemoteAddr.sin_addr.S_un.S_addr = inet_addr(m_IpAddr.c_str());
	this->m_RemoteAddr.sin_port = htons(std::atoi(m_PortNum.c_str()));

}


CScoketTCPClient::~CScoketTCPClient()
{
	m_RecviProcessThreadRunControl = false;
	if (m_RecviProcessThread!=nullptr && m_RecviProcessThread->joinable())
	{
		m_RecviProcessThread->join();
	}
	DisConnect();
}

bool CScoketTCPClient::Connect()
{
	if (this->m_sockthandle == INVALID_SOCKET)
	{
		return false;
	}
	int ret = connect(this->m_sockthandle, (sockaddr*)(&this->m_RemoteAddr), sizeof(this->m_RemoteAddr));
	if (ret == SOCKET_ERROR)
	{
		std::stringstream debuginfo;
		debuginfo << "Connect TCP Server fail the IPAddress is !!!" << m_IpAddr << "the Port is " << m_PortNum;
		DEBUGMSG(debuginfo.str())
		closesocket(this->m_sockthandle);
		return false;
	}
	this->m_ConnectFlag = true;
}

bool CScoketTCPClient::DisConnect()
{
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

int CScoketTCPClient::SendData(const byte* senddata, int SendNum)
{
	return send(this->m_sockthandle, reinterpret_cast<const char*>(senddata), SendNum, 0);
}

int CScoketTCPClient::Recvi(byte *recvidata, int recvilength)
{
	int Recvilen = { 0 };
	Recvilen = recv(this->m_sockthandle, reinterpret_cast<char*>(recvidata), recvilength, 0);
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
			m_RecviProcessThread = std::make_shared<std::thread>(&CScoketTCPClient::ThreadProcessFunction, this);
			FD_SET(m_sockthandle, &m_Recvifdset);
		}
		return true;
	}
	return false;
}

void CScoketTCPClient::ThreadProcessFunction(CScoketTCPClient *ClassParam)
{
	timeval ReadTimeOut = {0};
	ReadTimeOut.tv_sec = 0;
	ReadTimeOut.tv_usec = 100;
	char *recvibuf = new char[20000];
	if (m_RecviCallBackFunction!=nullptr && m_sockthandle != INVALID_SOCKET)
	{
		while (m_RecviProcessThreadRunControl)
		{
			if (int retcode = ::select(this->m_sockthandle, &m_Recvifdset, nullptr, nullptr, &ReadTimeOut))
			{
				if (retcode == 0)
				{
					continue;
				}
				else if (retcode == SOCKET_ERROR)
				{

					DEBUGMSG("select function occur error!!!")
				}
				
			}
			if (FD_ISSET(this->m_sockthandle, &m_Recvifdset))
			{
				ZeroMemory(recvibuf, 20000);
				int Recvilen = recv(this->m_sockthandle, recvibuf, 20000, 0);
				m_RecviCallBackFunction(recvibuf, Recvilen, 0);
			}
		}
	}
	delete[] recvibuf;


}
