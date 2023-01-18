#include "pointers.hpp"
#include "../hooking/hooking.hpp"
#include "../log/log.hpp"
namespace base {
	void pointers::scan() {
        std::vector<signature> signatures = {
          {"E8 ? ? ? ? 48 85 FF 48 89 1D", "SVM", 1, true},
          {"8B 15 ? ? ? ? 41 FF CF", "FC", 2, true},
          {"48 89 5C 24 18 48 89 7C 24 20 0F", "GNH"},
          {"48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", "NRT", 3, true},
          {"83 79 18 00 48 8B D1 74 4A", "FV"},
          {"48 8B C8 FF 52 30 84 C0 74 05 48", "STHN", 8},
          {"48 8D 15 ? ? ? ? 48 8D 4C 24 ? 89 05 ? ? ? ? E8 ? ? ? ?", "MMR", 3, true},
          {"48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9", "FR"},
          {"44 8B 0D ? ? ? ? 4C 8B 1D ? ? ? ? 48 8B 1D ? ? ? ? 41 83 F8 FF 74 3F 49 63 C0 42 0F B6 0C 18 81 E1", "SS", 7},
          {"4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", "VHP", 3, true}
        };
        std::vector<std::uintptr_t> results = g_memory.scan_multi(signatures);

        m_script_virtual_machine = reinterpret_cast<function_types::script_virtual_machine*>(results[0]);
        m_frame_count = reinterpret_cast<std::uint32_t*>(results[1]);
        m_get_native_handler = reinterpret_cast<function_types::get_native_handler*>(results[2]);
        m_native_registration_table = reinterpret_cast<rage::scrNativeRegistrationTable*>(results[3]);
        m_fix_vector = reinterpret_cast<function_types::fix_vector*>(results[4]);
        m_set_this_thread_networked = reinterpret_cast<PVOID>(results[5]);
        m_match_making_region = reinterpret_cast<std::uint8_t*>(results[6]);
        m_register_file = reinterpret_cast<function_types::file_register>(results[7]);
        m_streamed_scripts = results[8];
        m_vehicle_hash_pool = reinterpret_cast<rage::hash_list*>(results[9]);

        g_log.send("Memory", "Finished scanning {} signatures", g_memory.m_sig_count);
	}
}