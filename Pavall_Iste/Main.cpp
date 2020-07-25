#include "Include.hpp"

void CheckValidHardwareID()
{
	DWORD MY_HWID = 1234955536;
	DWORD MY_HWID2 = -763434645;
	DWORD MY_HWID3 = -1297420555;
	DWORD dwSerial;
	GetVolumeInformation("C:\\", NULL, 0, &dwSerial, NULL, NULL, NULL, 0);
	if (dwSerial == MY_HWID || dwSerial == MY_HWID2 || dwSerial == MY_HWID3)
	{
		MessageBox(0, "HWID Ok!", "Loaded", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		printf("Your HWID is: %d", dwSerial);
		MessageBox(0, "Wrong HWID", "Error", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}
}

int main(int argc, char** argv)
{
	//初始化CSGO
	std::cout << "[+] 初始化CSGO...." << std::endl;
	if (!memory.initialize("csgo")) return 0;

	//获取网络上最新的地址和偏移
	std::cout << "[+] 尝试从网络上获取最新地址和偏移...." << std::endl;
	if (!memory_config.initialize(argv[0], true)) return 0;

	//调用作弊线程
	std::cout << "[+] 开始作弊线程...." << std::endl;
	std::thread main_thread(hacks::main_thread);
	main_thread.join();

	return 1;
}