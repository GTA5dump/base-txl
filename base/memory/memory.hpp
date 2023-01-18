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
    struct signature {
        const char* signature;
        const char* name;
        int32_t add = 0;
        bool rip = false;
    };

    class memory {
    public:
        int m_sig_count;
    public:
        unsigned long hex_char_to_int(char c);
        std::vector<int> pattern_to_byte(const char* pattern);
    public: 
        std::uintptr_t scan(signature sig);
        std::vector<std::uintptr_t> scan_multi(const std::vector<signature>& sigs);
    };
    inline memory g_memory;

}