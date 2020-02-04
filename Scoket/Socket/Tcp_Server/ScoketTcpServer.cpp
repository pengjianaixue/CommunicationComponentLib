#include "ScoketTcpServer.h"

CScoketTcpServer::CScoketTcpServer(const std::string &serverIpAddr , const std::string &serverPortNum)
	:CScoketBase(serverIpAddr, serverPortNum,CScoketBase::TCP,CScoketBase::Server)
{
	m_eventbase = event_base_new();
	
}


CScoketTcpServer::~CScoketTcpServer()
{
	DisConnect();
	evconnlistener_free(m_listener);
	event_base_free(m_eventbase);
}

bool CScoketTcpServer::Connect()
{
	memset(&m_sin, 0, sizeof(m_sin));
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = htons(atoi(m_PortNum.c_str()));
	m_listener = evconnlistener_new_bind(m_eventbase, listener_cb, (void *)this,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&m_sin,
		sizeof(m_sin));
	m_isOpen = true;
	event_base_dispatch(m_eventbase);
	return true;
}

void CScoketTcpServer::listener_cb(evconnlistener *listener, evutil_socket_t fd, sockaddr *sa, int socklen, void *user_data)
{

	CScoketTcpServer *instance = static_cast<CScoketTcpServer *>(user_data);
	

	instance->m_bufferevent = bufferevent_socket_new(instance->GetEventBase(), fd, BEV_OPT_CLOSE_ON_FREE);
	if (!instance->m_bufferevent) {
		DEBUGERR("Error constructing bufferevent!");
		event_base_loopbreak(instance->GetEventBase());
		return;
	}
	bufferevent_setcb(instance->m_bufferevent, conn_Readcb, conn_Writecb, conn_Eventcb, user_data);
	bufferevent_enable(instance->m_bufferevent, EV_WRITE);
	bufferevent_enable(instance->m_bufferevent, EV_READ);
	
	
}

void CScoketTcpServer::conn_Readcb(bufferevent *bev, void *user_data)
{
	CScoketTcpServer *instance = static_cast<CScoketTcpServer *>(user_data);
	
	evbuffer *input = bufferevent_get_input(bev);
	int bufferLen = evbuffer_get_length(input);
	if (bufferLen > 0)
	{
		/*std::unique_ptr<char> dataBuffer = std::make_unique<char>(bufferLen);*/
		char *dataBuffer = new char[bufferLen] {0};
		if (bufferevent_read(bev, dataBuffer, bufferLen) > 0)
		{
			instance->m_serverReadCallbackfun(dataBuffer, bev, instance);
		}
	}
}

void CScoketTcpServer::conn_Writecb(bufferevent *bev, void *user_data)
{

	evbuffer *output = bufferevent_get_output(bev);
	if (evbuffer_get_length(output) == 0) 
	{
		printf("flushed answer\n"); 
		//bufferevent_free(bev);
	} 
	
}

void CScoketTcpServer::conn_Eventcb(bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF) {
		DEBUGMSG("Connection closed.\n");
	}
	else if (events & BEV_EVENT_ERROR) {
		std::ostringstream errorinfo;
		errorinfo  << "Got an error on the connection:" << __FILE__ << __FUNCTION__<< std::endl;
		DEBUGMSG(errorinfo.str());
	}
	/* None of the other events can happen here, since we haven't enabled
	* timeouts */
	//bufferevent_free(bev);
}

void CScoketTcpServer::Open()
{
	auto dispatch =  std::async(std::launch::deferred, &CScoketTcpServer::Connect,this);
	dispatch.get();
	return;
	
}

bool CScoketTcpServer::Close()
{
	
	return DisConnect();
}

bool CScoketTcpServer::DisConnect()
{  
	if (m_bufferevent)
	{
		bufferevent_free(m_bufferevent);
	}
	if (m_eventbase)
	{
		struct timeval delay = { 2, 0 };
		event_base_loopexit(m_eventbase, &delay);
	}
	return true;
}

bool CScoketTcpServer::Isopen()
{
	return m_isOpen; 
}



int CScoketTcpServer::SendData(const byte *senddata, size_t SendNum)
{

	return 0;
}

int CScoketTcpServer::Recvi(byte *recvidata, size_t recvilength)
{
	return 0;
}

int CScoketTcpServer::RecviAsync(byte *recvidata, size_t recvilength)
{
	return 0;
}

bool CScoketTcpServer::GetSocktInfor(SOCKTTYPE sockttype, SOCKADDR_IN &clientinforget)
{
	return false;
}


bool CScoketTcpServer::SetSyncReadAndRecviTimeOut(int Sendtimeout, int Recvitimeout)
{
	return false;
}

bool CScoketTcpServer::GetSyncReadAndRecviTimeOut(int &Sendtimeout, int &Recvitimeout) const
{
	return false;
}

bool CScoketTcpServer::SetHeartbeat(const byte *Heartbeatdata, size_t SendTimeIntervalofMillisecond)
{
	return false;
}

bool CScoketTcpServer::EnableClientHeartbeat()
{
	return false;
}

bool CScoketTcpServer::DisableClientHeartbeat()
{
	return false;
}

void CScoketTcpServer::ReigsterServerProcessFunction(SERVERPROCESSCALLBACKFUNCTION SeverProcessFunction)
{
	if (SeverProcessFunction)
	{
		m_serverReadCallbackfun = SeverProcessFunction;
	}
}

event_base* CScoketTcpServer::GetEventBase()
{
	return m_eventbase;
}
