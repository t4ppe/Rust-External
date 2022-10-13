#include "rust/rust.hpp"
#include "Iterations/BaseNetworkable.hpp"
#include "Entities/config.hpp"
#include <thread>
#include <Windows.h>
#include "rust/Menu.hpp"


int main()
{
	/*I do this so every 3 seconds it checks if rust was opened*/
	while (!rust::init());

	/*I create a thread for basenet since its alot of entities and needs to run on its own*/
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)basenet::basenetworkable_loop, NULL, NULL, NULL);

	/*I do this so every if localplayer isn't null we don't get a read acess violation, you can prob find a better way to check im lazy*/
	menu::IntializeMainCanvasMenuOverlay();


	/*Close Handle to prevent memory leak*/
	CloseHandle(kiface::procHandle);
}