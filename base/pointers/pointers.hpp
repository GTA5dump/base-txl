#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
namespace base {
	namespace types {
		using script_virtual_machine = std::int64_t(void* stack, std::int64_t** globals, std::int64_t* program, std::int64_t* thread_context);

		using get_native_handler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, std::uint64_t);

		using fix_vector = void(rage::scrNativeCallContext*);

		using file_register = uint32_t(*)(int* p1, const char* p2, bool p3, const char* p4, bool p5);
	};
	class pointers
	{
	public:
		void scan();
	public:
		types::script_virtual_machine* m_script_virtual_machine;
		std::uint32_t* m_frame_count;
		types::get_native_handler* m_get_native_handler{};
		rage::scrNativeRegistrationTable* m_native_registration_table;
		types::fix_vector* m_fix_vector;
		PVOID m_set_this_thread_networked;
		std::uint8_t* m_match_making_region;
		types::file_register m_register_file;
		std::uintptr_t m_streamed_scripts;
		rage::hash_list* m_vehicle_hash_pool;
	}; inline pointers g_pointers;
}