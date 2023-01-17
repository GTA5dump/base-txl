#include "pointers.hpp"
#include "../hooking/hooking.hpp"
#include "../log/log.hpp"
namespace base {
	void pointers::scan() {
		auto start = std::chrono::steady_clock::now();

        std::vector<std::pair<const char*, const char*>> signatures = {
          {"E8 ? ? ? ? 48 85 FF 48 89 1D", "SVM"},
          {"8B 15 ? ? ? ? 41 FF CF", "FC"},
          {"48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A", "GNH"},
          {"83 79 18 00 48 8B D1 74 4A", "FV"},
          {"48 8B C8 FF 52 30 84 C0 74 05 48", "SMALN"},
          {"48 8D 15 ? ? ? ? 48 8D 4C 24 ? 89 05 ? ? ? ? E8 ? ? ? ?", "MMR"},
          {"48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9", "FR"},
          {"44 8B 0D ? ? ? ? 4C 8B 1D ? ? ? ? 48 8B 1D ? ? ? ? 41 83 F8 FF 74 3F 49 63 C0 42 0F B6 0C 18 81 E1", "SS"},
          {"4C 03 05 ? ? ? ? EB 09 49 83 C2 04 EB 8B", "VHP"}
        };
        std::vector<std::uintptr_t> results = g_memory.scan_multi(signatures);

        m_script_virtual_machine = reinterpret_cast<decltype(m_script_virtual_machine)>(g_memory.rip(g_memory.add(results[0], 1)));
        m_frame_count = reinterpret_cast<decltype(m_frame_count)>(g_memory.rip(g_memory.add(results[1], 2)));
        m_get_native_handler = reinterpret_cast<decltype(m_get_native_handler)>(g_memory.rip(g_memory.add(results[2], 12)));
        m_native_registration_table = reinterpret_cast<decltype(m_native_registration_table)>(g_memory.rip(g_memory.add(results[2], 3)));
        m_fix_vector = reinterpret_cast<decltype(m_fix_vector)>(results[3]);
        m_set_this_thread_networked = reinterpret_cast<decltype(m_set_this_thread_networked)>(g_memory.add(results[4], 8));
        m_match_making_region = reinterpret_cast<decltype(m_match_making_region)>(g_memory.rip(g_memory.add(results[5], 3)));
        m_register_file = reinterpret_cast<decltype(m_register_file)>(results[6]);
        m_streamed_scripts = g_memory.add(results[7], 7);
        m_vehicle_hash_pool = reinterpret_cast<decltype(m_vehicle_hash_pool)>(g_memory.rip(g_memory.add(results[8], 3)));

        auto end = std::chrono::steady_clock::now();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        g_log.send("Memory", "Finished scanning {} signatures -> {}ms", g_memory.m_sig_count, elapsed_time);
	}
}