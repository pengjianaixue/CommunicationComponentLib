#include "SocketInterface.h"
#pragma comment(lib,"ws2_32.lib")
CScoketBase::CScoketBase(const std::string & IpAddr, const std::string & PortNum, SOCKTTYPE socktype, CLIENTTYPE clienttype)
	:m_IpAddr(IpAddr), m_PortNum(PortNum), m_ClientType(clienttype), m_SokcetType(socktype)
{
	
	SokctLoadinit();

}

CScoketBase::~CScoketBase()
{
	WSACleanup();
}

bool CScoketBase::Connect()
{
	return false;
}

bool CScoketBase::DisConnect()
{
	return false;
}

bool CScoketBase::Isopen()
{
	return false;
}

bool CScoketBase::IsConnect()
{
	return false;
}

int CScoketBase::SendData(const byte* senddata, size_t SendNum)
{
	return 0;
}

int CScoketBase::Recvi(byte *recvidata, size_t recvilength)
{
	return 0;
}

int CScoketBase::RecviAsync(byte * recvidata, size_t recvilength)
{

	return 0;
}

bool CScoketBase::GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN & clientinforget)
{
	return false;
}


bool CScoketBase::SetSyncReadAndRecviTimeOut(int Sendtimeout, int Recvitimeout)
{
	m_Sendtimeout = Sendtimeout;
	m_Recvitimeout = Recvitimeout;
	return true;
}

bool CScoketBase::GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const
{
	return true;
}

bool CScoketBase::SetHeartbeat(const byte *Heartbeatdata, size_t SendTimeIntervalofMillisecond)
{
	m_HeartbeatData = (const char*)Heartbeatdata;
	m_SendTimeInterval = SendTimeIntervalofMillisecond;
	return true;
}

bool CScoketBase::EnableClientHeartbeat()
{
	return m_HeartbeatEnable=true;
}

bool CScoketBase::DisableClientHeartbeat()
{
	return m_HeartbeatEnable = false;
}

bool CScoketBase::SokctLoadinit()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}
	return true;
}

//bool CScoketBase::SetUDPRemoteAddr(const std::string & Ipaddr, const std::string & Port)
//{
//	return false;
//}
