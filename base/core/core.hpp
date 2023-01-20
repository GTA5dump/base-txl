#pragma once
#include <Windows.h>
namespace base {
	class core {
	public:
		void attach();
		void detach();
	public:
		HMODULE m_module{};
		HANDLE m_main_thread;
		bool m_steam;
		DWORD m_main_thread_id;
	};
	inline core g_core; 
};