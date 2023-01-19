#include "core.hpp"
#include "../log/log.hpp"
#include "../hooking/hooking.hpp"
#include "../invoker/invoker.hpp"
#include "../script/script.hpp"

namespace base {
	void core::attach() {
		std::thread main_thread([=] {
			g_log.attach();
	     	g_input.initialize();
		    g_pointers.scan();
		    g_hooking.attach();
		    g_invoker.cache_handler();
		    g_script.attach();
			while (g_running) {
				if (GetAsyncKeyState(VK_END)) {
					g_running = false;
				}
				std::this_thread::sleep_for(1ms);
			}
			detach();
			FreeLibraryAndExitThread(m_module, 0);
		});
		main_thread.detach();
	}
	void core::detach() {
		g_hooking.detach();
		g_script.detach();
		g_log.detach();
		std::this_thread::sleep_for(100ms);
	}
}
