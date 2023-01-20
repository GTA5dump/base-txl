#pragma once
#include <array>
#include <cstring>
#include <map>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include "../common.hpp"
#include "../log/log.hpp"
#include <future>
namespace base
{
    class memory {
    public:
        int m_sig_count;
    public:
        unsigned long hex_char_to_int(char c);
        std::vector<int> pattern_to_byte(const char* pattern);
        std::uintptr_t scan(const char* signature, const char* name, int32_t add = 0, bool rip = false);
    public: 
    };
    inline memory g_memory;

}