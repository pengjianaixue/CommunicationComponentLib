// Scoket.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Socket\SocketInterface.h"
#include "Socket\Tcp_Client\ScoketTcpClient.h"
#include "SSH\SSHshell.h"
using namespace std;
bool b_recvi = false;
void RecviPrint(char *Recvibuf, int length,int flag)
{
	std::cout << Recvibuf << std::endl;
	b_recvi = true;
}
int main()
{
	CSSHshell ssh("10.166.152.6");
	if (ssh.Connect("EIJPNAE", "EIJPNAE"))
	{
		Channel* channel = ssh.CreateChannel();
		channel->Write("ls -a");
		cout << channel->Read() << endl;
		channel->Write("pwd");
		cout << channel->Read() << endl;
		delete channel;
	}
	
	

	//{
	//	unsigned char senddata[2000] = "This is a TCP client message";
	//	unsigned char recvi[2000] = {0};
	//	CScoketTCPClient k;
	//	k.Connect();
	//	k.SendData((unsigned char*)senddata, 4);
	//	k.ReigsterAsyncRecviProcessFunction(RecviPrint);
	//	while (!(std::getchar()=='c'))
	//	{
	//		;
	//	}
	//	/*while (true)
	//	{
	//		int ret =  k.Recvi(recvi, 2000);
	//		if (SOCKET_ERROR  == ret)
	//		{
	//			int error = WSAGetLastError();
	//			printf("\n");
	//		}
	//		int sendtimeout = {0};
	//		int recvitimeout = { 0 };
	//		bool retval = k.GetSyncReadAndRecviTimeOut(sendtimeout, recvitimeout);
	//		int error = WSAGetLastError();
	//		std::cout << recvi << std::endl;
	//		ZeroMemory(recvi, 2000);
	//	}*/
	//	k.DisConnect();
	//	system("pause"); 
	//}
	system("pause");
	_CrtDumpMemoryLeaks();
    return 0;
}

