// Scoket.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Socket\SocketInterface.h"
#include "Socket\Tcp_Client\ScoketTcpClient.h"
bool b_recvi = false;
bool RecviPrint(char *Recvibuf, int length,int flag)
{
	std::cout << Recvibuf << std::endl;
	b_recvi = true;
	return true;

}
int main()
{
	{
		unsigned char recvi[2000] = { 0 };
		CScoketTCPClient k;
		k.Connect();
		k.SendData((unsigned char*)"TCPCLIENT", 10);
		k.ReigsterAsyncRecviProcessFunction(RecviPrint);

		while (!b_recvi)
		{
			std::cout << "Wait for Recvi Call Back" << std::endl;
		}
		k.DisConnect();

		std::cout << recvi << std::endl;
		system("pause"); 
	}
	_CrtDumpMemoryLeaks();
    return 0;
}

