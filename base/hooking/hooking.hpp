#pragma once
#include "detour/detour.hpp"
namespace base {
	struct hooks {
		static std::int64_t script_virtual_machine(void* stack, std::int64_t** globals, std::int64_t* program, std::int64_t* thread_context);
    };
	struct hooking {
		void attach();
		void detach();

		detour_hook m_script_virtual_machine;

		bool hook_script_native(const char* script_name, std::uint64_t native_hash, void* destination);
		void remove_script_hooks();
		std::map<std::pair<const char*, uint64_t>, void**> m_hooked_natives;
	};
	inline hooking g_hooking;
}