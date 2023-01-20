#include "core.hpp"
#include "../log/log.hpp"
#include "../hooking/hooking.hpp"
#include "../invoker/invoker.hpp"
#include "../script/script.hpp"

namespace base {
        void core::attach() { 
        auto threadProc = [](LPVOID lpParam) -> DWORD {
            g_core.m_steam= (uint64_t)GetModuleHandleA("steam_api64.dll") > 0;
            g_log.attach();
            g_input.initialize();
            g_pointers.scan();
            g_hooking.attach();
            g_invoker.cache_handler();
            g_script.attach();
            while (g_running) {
                if (GetAsyncKeyState(VK_END) & 0x8000) {
                    g_running = false;
                }
                std::this_thread::sleep_for(1ms);
            }
            g_core.detach();
            CloseHandle(g_core.m_main_thread);
            FreeLibraryAndExitThread(g_core.m_module, 0);
        };
        m_main_thread = CreateThread(NULL, 0, threadProc, this, 0, &m_main_thread_id);
	}
	void core::detach() {
		g_hooking.detach();
        std::this_thread::sleep_for(500ms);
		g_script.detach();
		g_log.detach();
	}
}
