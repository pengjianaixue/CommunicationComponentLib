#pragma once
#include "../SocketInterface.h"
#include <sstream>
#include <future>
#include <event.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
class CScoketTcpServer :public CScoketBase
{
public:

	using SERVERPROCESSCALLBACKFUNCTION = std::function<void(const char*, bufferevent *serverBufferEvent, void*)>;
	CScoketTcpServer(const std::string &serverIpAddr = "127.0.0.1", const std::string &serverPortNum = "8080");
	virtual ~CScoketTcpServer();
//API
public:
	
	void								Open();
	bool								Close();
	virtual	bool						Isopen() override;
	virtual	int							SendData(const byte* senddata, size_t SendNum) override;
	virtual	int							Recvi(byte* recvidata, size_t recvilength) override;
	virtual	int							RecviAsync(byte* recvidata, size_t recvilength) override;
	virtual	bool						GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget) override;
	virtual bool						SetSyncReadAndRecviTimeOut(int Sendtimeout, int Recvitimeout) override;
	virtual bool						GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const override;
	virtual bool						SetHeartbeat(const byte* Heartbeatdata, size_t SendTimeIntervalofMillisecond) override;
	virtual bool						EnableClientHeartbeat() override;
	virtual bool						DisableClientHeartbeat()override;
	void								ReigsterServerProcessFunction(SERVERPROCESSCALLBACKFUNCTION SeverProcessFunction);
	event_base*							GetEventBase();
	
private:
	event_base							*m_eventbase{ nullptr };
	evconnlistener						*m_listener{ nullptr };
	event								*m_signal_event { nullptr };
	sockaddr_in							m_sin;
	SERVERPROCESSCALLBACKFUNCTION	    m_serverReadCallbackfun;
	evconnlistener_cb					m_listencb{nullptr};
	bool								m_isOpen{false};
	bufferevent							*m_bufferevent{ nullptr };
	std::unique_ptr<std::thread>		m_closeEventListenThread;


	virtual	bool						Connect() override;
	virtual	bool						DisConnect() override;
	static void	 listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
		struct sockaddr *sa, int socklen, void *user_data);
	static void conn_Writecb(struct bufferevent *bev, void *user_data);
	static void conn_Readcb(struct bufferevent *bev, void *user_data);
	static void conn_Eventcb(struct bufferevent *bev, short events, void *user_data);

};

