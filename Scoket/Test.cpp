// Scoket.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Socket\SocketInterface.h"
#include "Socket\Tcp_Client\ScoketTcpClient.h"
bool b_recvi = false;
void RecviPrint(char *Recvibuf, int length,int flag)
{
	std::cout << Recvibuf << std::endl;
	b_recvi = true;

}
int main()
{
	{
		unsigned char senddata[2000] = { 0x85,0x75,0x45,0x22 };
		unsigned char recvi[2000] = {0};
		CScoketTCPClient k;
		k.Connect();
		k.SendData((unsigned char*)senddata, 4);
		/*k.ReigsterAsyncRecviProcessFunction(RecviPrint);*/
		while (true)
		{
			k.Recvi(recvi, 2000);
			std::cout << recvi << std::endl;
			ZeroMemory(recvi, 2000);
		}
		k.DisConnect();
		system("pause"); 
	}
	_CrtDumpMemoryLeaks();
    return 0;
}

