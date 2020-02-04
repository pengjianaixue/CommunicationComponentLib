#pragma once
#include "../SocketInterface.h"
#include <atomic>
#include <chrono>
#include "../../../Utility/TestFunction/RunTimeMeasure.h"
#include "../../../Utility/Synchronizationwrap/synchronizationwrap.hpp"
using namespace std::chrono;
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
	virtual	int			SendData(const byte* senddata, size_t SendNum) override;
	virtual	int			Recvi(byte *recvidata, size_t recvilength) override;
	virtual	bool		GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget) override;
	virtual	bool		ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction) ;
	virtual bool		GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const override;
	virtual bool		EnableClientHeartbeat() override;
	virtual bool		DisableClientHeartbeat()override;
private:
	void				RecviThreadProcessFunction(CScoketTCPClient *ClassParam);
	void				HeartbeatThreadProcessFunction(CScoketTCPClient *ClassParam);
	bool				OPenHeartbeartThread();
	bool				CloseHeartbeartThread();


private:
	bool								m_ConnectFlag					= { false };
	SOCKET								m_sockthandle					= { INVALID_SOCKET }; //Sockt Resource
	SOCKADDR_IN							m_RemoteAddr					= { 0 };
	SOCKADDR_IN							m_TCPServerAccpet				= { 0 };
	//Asynchronous function
	RecviCallBackFunction				m_RecviCallBackFunction			= { nullptr };
	std::shared_ptr<std::thread>		m_RecviProcessThread			= { nullptr };
	std::shared_ptr<std::thread>		m_ClientHeartbeatSendThread		= { nullptr };
	fd_set								m_Recvifdset					= {FD_ZERO(&m_Recvifdset)};
	std::atomic<bool>					m_RecviProcessThreadRunControl  = { false };
	CRITICAL_SECTION					m_HeartbeatandSendcritical		= {0};

};

