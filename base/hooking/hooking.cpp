#include "hooking.hpp"
#include "../pointers/pointers.hpp"
#include "hooks/script_virtual_machine.hpp"
#include "../rage/classes.hpp"
#include "../invoker/invoker.hpp"
namespace base {
	void hooking::attach() {
		MH_Initialize();

		m_script_virtual_machine.create("SVM", g_pointers.m_script_virtual_machine, &hooks::script_virtual_machine);

		MH_EnableHook(MH_ALL_HOOKS);
	}

	void hooking::detach() {
		remove_script_hooks();

		m_script_virtual_machine.remove();

		MH_DisableHook(MH_ALL_HOOKS);
		
		MH_Uninitialize();
	}

	bool hooking::hook_script_native(const char* script_name, std::uint64_t native_hash, void* destination) {
		// Credits vali
		if (auto val = g_invoker.m_cache.find(native_hash); val != g_invoker.m_cache.end()) {
			void* handler = (void*)val->second;
			std::uint64_t* table = *(std::uint64_t**)(g_pointers.m_streamed_scripts);
			if (table == NULL) return false;
			rage::scrProgram* script_program = g_gta_utility.get_program_by_hash(joaat(script_name));
			if (script_program && script_program->is_valid()) {
				void** script_handler = script_program->get_address_of_native_entrypoint(handler);
				if (script_handler) {
					m_hooked_natives.insert({ { script_name, native_hash }, script_handler });
					*script_handler = destination;
					g_log.send("Hooks", "Created {} native hook in {}", script_name, native_hash);
					return true;
				}
			}
		}
		return false;
	}

	void hooking::remove_script_hooks() {
		// Credits vali
		for (auto& hooked_native : m_hooked_natives) {
			if (auto val = g_invoker.m_cache.find(hooked_native.first.second); val != g_invoker.m_cache.end()) {
				void* handler = (void*)val->second;
				*hooked_native.second = handler;
			}
		}
		m_hooked_natives.clear();
	}
}