#pragma once
#include <array>
#include <cstring>
#include <map>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include "../common.hpp"
#include "../log/log.hpp"
namespace base
{
 
    struct memory {
        int m_sig_count;
        std::uintptr_t scan(const char* signature, const char* name, int32_t add = 0);
        std::vector<std::uintptr_t> scan_multi(const std::vector<std::pair<const char*, const char*>>& signatures);
        std::uintptr_t rip(std::uintptr_t address, int offset = 4) {
            return (address + *(std::int32_t*)address) + offset;
        }
        std::uintptr_t add(std::uintptr_t address, int offset) {
            return address + offset;
        }
        std::uintptr_t sub(std::uintptr_t address, int offset) {
            return address - offset;
        }
    };
    inline memory g_memory;

}