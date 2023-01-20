#include "pointers.hpp"
#include "../hooking/hooking.hpp"
#include "../log/log.hpp"
namespace base {
	void pointers::scan() {
        auto start = std::chrono::steady_clock::now();

        m_script_virtual_machine = reinterpret_cast<types::script_virtual_machine*>(g_memory.scan("E8 ? ? ? ? 48 85 FF 48 89 1D", "SVM", 1, true));
        m_frame_count = reinterpret_cast<std::uint32_t*>(g_memory.scan("8B 15 ? ? ? ? 41 FF CF", "FC", 2, true));
        m_get_native_handler = reinterpret_cast<types::get_native_handler*>(g_memory.scan("48 89 5C 24 18 48 89 7C 24 20 0F", "GNH"));
        m_native_registration_table = reinterpret_cast<rage::scrNativeRegistrationTable*>(g_memory.scan("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", "NRT", 3, true));
        m_fix_vector = reinterpret_cast<types::fix_vector*>(g_memory.scan("83 79 18 00 48 8B D1 74 4A", "FV"));
        m_set_this_thread_networked = reinterpret_cast<PVOID>(g_memory.scan("48 8B C8 FF 52 30 84 C0 74 05 48", "STHN", 8));
        m_match_making_region = reinterpret_cast<std::uint8_t*>(g_memory.scan("48 8D 15 ? ? ? ? 48 8D 4C 24 ? 89 05 ? ? ? ? E8 ? ? ? ?", "MMR", 3, true));
        m_register_file = reinterpret_cast<types::file_register>(g_memory.scan("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9", "FR"));
        m_streamed_scripts = g_memory.scan("44 8B 0D ? ? ? ? 4C 8B 1D ? ? ? ? 48 8B 1D ? ? ? ? 41 83 F8 FF 74 3F 49 63 C0 42 0F B6 0C 18 81 E1", "SS", 0, false);
        m_vehicle_hash_pool = reinterpret_cast<rage::hash_list*>(g_memory.scan("4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", "VHP", 3, true));

        auto end = std::chrono::steady_clock::now();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        g_log.send("Memory", "Finished scanning {} signatures in {}ms", g_memory.m_sig_count, elapsed_time);
	}
}