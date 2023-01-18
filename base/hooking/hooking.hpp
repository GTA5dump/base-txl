#pragma once
#include "detour/detour.hpp"
namespace base {
	class hooks {
	public:
		static std::int64_t script_virtual_machine(void* stack, std::int64_t** globals, std::int64_t* program, std::int64_t* thread_context);
    };
	class hooking {
	public:
		void attach();
		void detach();
	public:
		bool hook_script_native(const char* script_name, std::uint64_t native_hash, void* destination);
		void remove_script_hooks();
	public:
		detour_hook m_script_virtual_machine;
		std::map<std::pair<const char*, uint64_t>, void**> m_hooked_natives;
	};
	inline hooking g_hooking;
}