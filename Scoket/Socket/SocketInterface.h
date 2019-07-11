#pragma once
#include <WinSock2.h>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#ifdef _DEBUG
#define DEBUGMSG(msg) std::cout<<(msg)<<std::endl;
#else
#define DEBUGMSG(msg)	 
#endif // _DEBUG
class CScoketBase
{
public:
	typedef enum SocktType
	{
		TCP,
		UDP
	} SOCKTTYPE;
	typedef enum ClientType
	{
		Client,
		Server
	} CLIENTTYPE;
	using   RecviCallBackFunction = std::function<void(char* Recvibuf, int length, int flag)>;
	using   byte = unsigned char;
public:
	CScoketBase(const std::string &IpAddr = "127.0.0.1", const std::string &PortNum = "8080",
		SOCKTTYPE socktype = SOCKTTYPE::TCP, CLIENTTYPE clienttype = CLIENTTYPE::Client);
	virtual								~CScoketBase();
	//Interface 
	virtual	bool						Connect();
	virtual	bool						DisConnect();
	virtual	bool						Isopen();
	virtual	bool						IsConnect();
	virtual	int							SendData(const byte* senddata, int SendNum);
	virtual	int							Recvi(byte* recvidata, int recvilength);
	virtual	int							RecviAsync(byte* recvidata, int recvilength);
	virtual	bool						GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget);
	virtual	bool						ReigsterAsyncRecviProcessFunction(RecviCallBackFunction CallBackfunction);
	virtual bool						SetSyncReadAndRecviTimeOut(int Sendtimeout, int Recvitimeout);
	virtual bool						GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const;

private:
	static bool SokctLoadinit();
protected:
	CLIENTTYPE							m_ClientType = {};
	SOCKTTYPE							m_SokcetType = {};
	std::string							m_IpAddr = {};
	std::string							m_PortNum = {};
	int									m_Sendtimeout =  { 30000 };
	int									m_Recvitimeout = { 60000 };
	
};