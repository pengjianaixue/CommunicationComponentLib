#pragma once
#include "SocketInterface.h"

class CScoketTCPClient :public CScoketBase
{
public:
	CScoketTCPClient(const std::string &IpAddr = "127.0.0.1", const std::string &PortNum = "8080");
	virtual ~CScoketTCPClient();

	//API
public:
	virtual	bool		Connect()  override;
	virtual	bool		DisConnect() override;
	virtual	bool		Isopen() override;
	virtual	bool		IsConnect() override;
	virtual	int			SendData(const byte* senddata, int SendNum) override;
	virtual	int			Recvi(byte *recvidata, int recvilength) override;
	virtual	bool		GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget) override;
	virtual	bool		ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction) override;
private:
	void				ThreadProcessFunction(CScoketTCPClient *ClassParam);

private:
	bool								m_ConnectFlag					= { false };
	SOCKET								m_sockthandle					= { INVALID_SOCKET }; //Sockt Resource
	SOCKADDR_IN							m_RemoteAddr					= { 0 };
	SOCKADDR_IN							m_TCPServerAccpet				= { 0 };
	//Asynchronous function
	RecviCallBackFunction				m_RecviCallBackFunction			= { nullptr };
	std::shared_ptr<std::thread>		m_RecviProcessThread			= { nullptr };
	fd_set								m_Recvifdset					= {FD_ZERO(&m_Recvifdset)};
	bool								m_RecviProcessThreadRunControl  = { false };
	
};

