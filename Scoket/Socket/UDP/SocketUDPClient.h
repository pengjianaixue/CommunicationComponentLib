#pragma once
#include "../SocketInterface.h"
#include "../../../Utility/TestFunction/RunTimeMeausre.h"
#include "../../../Utility/synchronizationwrap.hpp"
class CSocketUDP :public CScoketBase
{
public:
	CSocketUDP(const std::string &IpAddr = "127.0.0.1", const std::string &PortNum = "8080");
	virtual ~CSocketUDP();
//API 
public:
	//Interface 
	virtual	bool						Connect();
	virtual	bool						DisConnect();
	virtual	bool						Isopen();
	virtual	bool						IsConnect();
	virtual	int							SendData(const byte* senddata, size_t SendNum);
	virtual	int							Recvi(byte* recvidata, size_t recvilength);
	virtual	int							RecviAsync(byte* recvidata, size_t recvilength);
	virtual	bool						GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget);
	virtual	bool						ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction);
	virtual bool						GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const;
	virtual bool						EnableClientHeartbeat();
	virtual bool						DisableClientHeartbeat();
private:
	void								RecviThreadProcessFunction(CSocketUDP *ClassParam);
	void								HeartbeatThreadProcessFunction(CSocketUDP *ClassParam);
	bool								OPenHeartbeartThread();
	bool								CloseHeartbeartThread();
private:
	SOCKET								m_sockthandle						= { INVALID_SOCKET };
	SOCKADDR_IN							m_RemoteAddr						= { 0 };
	bool								m_ConnectFlag						= { false };
	//Asynchronous function
	RecviCallBackFunction				m_RecviCallBackFunction				= { nullptr };
	std::shared_ptr<std::thread>		m_RecviProcessThread				= { nullptr };
	std::shared_ptr<std::thread>		m_ClientHeartbeatSendThread			= { nullptr };
	fd_set								m_Recvifdset						= { FD_ZERO(&m_Recvifdset) };
	std::atomic<bool>					m_RecviProcessThreadRunControl		= { false };
	CRITICAL_SECTION					m_HeartbeatandSendcritical			= { 0 };


};

