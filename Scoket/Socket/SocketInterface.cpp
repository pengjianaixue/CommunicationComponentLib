#include "SocketInterface.h"
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

int CScoketBase::SendData(const byte* senddata, int SendNum)
{
	return 0;
}

int CScoketBase::Recvi(byte *recvidata, int recvilength)
{
	return 0;
}

int CScoketBase::RecviAsync(byte * recvidata, int recvilength)
{

	return 0;
}

bool CScoketBase::GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN & clientinforget)
{
	return false;
}

bool CScoketBase::SokctLoadinit()
{
	DWORD sockVersion = MAKEWORD(2, 2);
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
