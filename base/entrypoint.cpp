#include "log/log.hpp"
#include "hooking/hooking.hpp"
#include "invoker/invoker.hpp"
#include "script/script.hpp"
auto detach_dll(void* thread_param) {
	using namespace base;

	std::this_thread::sleep_for(250ms);

	g_hooking.detach();

	g_script.detach();

	g_log.send("Log", "Detached");

	g_log.deattach();

	FreeLibraryAndExitThread(static_cast<HMODULE>(thread_param), EXIT_SUCCESS);
}


BOOL entry_dll(void* thread_param)  {
	using namespace base;

	g_running = true; 

	g_log.attach();

	g_log.send("Log", "Attached");

	g_input.initialize();

	g_pointers.scan();

	g_hooking.attach();

	g_invoker.cache_handler();

	g_log.send("Invoker", "Natives cached");

	g_script.attach();

	g_log.send("Credits", "Base made by TxL#1337");

	while (g_running) {
		if (GetAsyncKeyState(VK_END) & 0x8000)
			g_running = false;
		std::this_thread::sleep_for(1ms);
	}
	
	detach_dll(thread_param);
}


BOOL __stdcall DllMain(const HMODULE module, const DWORD reason, LPVOID reserved) 
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(module);

		if (auto* handle = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_dll), module, NULL, nullptr))
		{
			CloseHandle(handle);
		}
	}
	
    return true;
}

