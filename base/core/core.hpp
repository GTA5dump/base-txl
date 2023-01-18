#pragma once
#include <Windows.h>
namespace base {
	class core {
	public:
		void attach();
		void detach();
	public:
		HMODULE m_module{};
		bool m_detach_was_called;
	};
	inline core g_core; 
};