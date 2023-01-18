#pragma once
#include "../../fiber/fiber.hpp"
#include "../hooking.hpp"
namespace base {
	std::int64_t hooks::script_virtual_machine(void* stack, std::int64_t** globals, std::int64_t* program, std::int64_t* thread_context) {
		static std::uint32_t hook_frame_count;
		if (hook_frame_count != *g_pointers.m_frame_count) {
			g_fibers.tick(&g_main_fiber);
			hook_frame_count = *g_pointers.m_frame_count;
		}
		return g_hooking.m_script_virtual_machine.get_original<decltype(&script_virtual_machine)>()(stack, globals, program, thread_context);
	}
}