// Scoket.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Socket\SocketInterface.h"
#include "Socket\Tcp_Client\ScoketTcpClient.h"
#include "SSH\SSHshell.h"
using namespace std;
bool b_recvi = false;
int g_int = 0;
CRITICAL_SECTION					m_HeartbeatandSendcritical = { 0 };

void Print1()
{
	for (size_t i = 0; i < 10000; i++)
	{
		
		CriticalSectionLockGuardian heartbeat(m_HeartbeatandSendcritical);
		std::cout << "Print1:" << ++g_int << std::endl;

		
	}
}
void Print2()
{
	for (size_t i = 0; i < 10000; i++)
	{
		CriticalSectionLockGuardian heartbeat(m_HeartbeatandSendcritical);
		std::cout << "Print2:" << ++g_int << std::endl;
	}
	
}

void RecviPrint(char *Recvibuf, int length,int flag)
{

	std::cout << Recvibuf << std::endl;
	b_recvi = true;
}
int main()
{


	/*::InitializeCriticalSection(&m_HeartbeatandSendcritical);
	std::thread  ta(Print1);
	std::thread  tb(Print2);
	ta.join();
	tb.join();*/
	/*CSSHshell ssh("10.166.152.6");
	if (ssh.Connect("EIJPNAE", "EIJPNAE"))
	{
		Channel* channel = ssh.CreateChannel();
		channel->Write("ls -a");
		cout << channel->Read() << endl;
		channel->Write("pwd");
		cout << channel->Read() << endl;
		delete channel;
	}*/
	
	{
		unsigned char senddata[2000] = "This is a TCP client message";
		unsigned char recvi[2000] = {0};
		unsigned char heartbeatpack[10] = {"heartbeat"};
		CScoketTCPClient TcpClientTest;
		TcpClientTest.SetClientHeartbeat(heartbeatpack, 1);
		TcpClientTest.EnableClientHeartbeat();
		TcpClientTest.Connect();
		TcpClientTest.SendData((unsigned char*)senddata, 4);
		TcpClientTest.ReigsterAsyncRecviProcessFunction(RecviPrint);
		while (!(std::getchar() == 'c'))
		{
			TcpClientTest.SendData((unsigned char*)"SendData", 8);
		}
		TcpClientTest.DisableClientHeartbeat();
		while (!(std::getchar() == 'd'))
		{
			;
		}
		TcpClientTest.EnableClientHeartbeat();
		while (!(std::getchar() == 'c'))
		{
			;
		}
		/*while (true)
		{
			int ret =  k.Recvi(recvi, 2000);
			if (SOCKET_ERROR  == ret)
			{
				int error = WSAGetLastError();
				printf("\n");
			}
			int sendtimeout = {0};
			int recvitimeout = { 0 };
			bool retval = k.GetSyncReadAndRecviTimeOut(sendtimeout, recvitimeout);
			int error = WSAGetLastError();
			std::cout << recvi << std::endl;
			ZeroMemory(recvi, 2000);
		}*/
		TcpClientTest.DisConnect();
		system("pause"); 
	}
	system("pause");
	_CrtDumpMemoryLeaks();
    return 0;
}

