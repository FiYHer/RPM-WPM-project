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
	//CheckValidHardwareID();

	/////
	//Initalize the config
	if (!config.initialize(argv[0]))
		return 0;

	auto fetch_from_web = true;
	
	//Kinda lazy and ugly method of getting user input, not very good practice
	std::string answer;
	std::cout << "Fetch offsets from the internet? (y/n)\n";
	std::cin >> answer;
	if (answer.length() > 0)
	{
		if (answer == "y" || answer == "Y")
			fetch_from_web = true;
		else if (answer == "n" || answer == "N")
			fetch_from_web = false;
	}

	//Initialize the memory class, which gets the base address of the client and engine.dll in csgo to read form
	//so you can get information like enemy position, etc.
	if (!memory.initialize("csgo"))
		return 0;

	//Initialize the memory config for getting offsets from internet or file
	if (!memory_config.initialize(argv[0], fetch_from_web))
		return 0;

	//Start the main thread that does all the hacks
	std::thread main_thread(hacks::main_thread);
	main_thread.detach();

	//If you press delete, close program
	while(!GetAsyncKeyState(VK_DELETE)&1)
	{
		//If you press insert then load config
		if (GetAsyncKeyState(VK_INSERT) & 1)
			config.load_config();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	return 1;
}
