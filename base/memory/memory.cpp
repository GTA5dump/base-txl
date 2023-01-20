#include "memory.hpp"
#include <immintrin.h>
#include <xmmintrin.h>
#include "../log/log.hpp"
#include <future>
namespace base
{
    unsigned long memory::hex_char_to_int(char c) {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        return -1;
    }

    std::vector<int> memory::pattern_to_byte(const char* pattern) {
        std::vector<int> bytes;
        const char* end = pattern;
        while (*end != '\0') {
            ++end;
        }
        for (const char* current = pattern; current < end; ) {
            if (*current == '?') {
                bytes.emplace_back(-1);
                ++current;
                if (*current == '?') {
                    ++current;
                }
                continue;
            }
            char hex_chars[3] = { *(current++), *(current++), '\0' };
            int value = std::strtoul(hex_chars, nullptr, 16);
            bytes.emplace_back(value);
        }
        return bytes;
    }
    std::uintptr_t memory::scan(const char* signature, const char* name, int32_t add, bool rip) {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<std::pair<int, bool>>{};
            const char* end = pattern;
            while (*end != '\0') ++end;
            for (auto* current = pattern; current < end; ++current) {
                if (*current == '?') {
                    bytes.emplace_back(-1, true);
                    ++current;
                    if (*current == '?') {
                        ++current;
                    }
                    continue;
                }
                unsigned long value = 0;
                while (isxdigit(*current)) {
                    value = (value << 4) | g_memory.hex_char_to_int(*current);
                    ++current;
                }
                bytes.emplace_back(value, false);
            }
            return bytes;
        };

        auto* const module = GetModuleHandle(nullptr);

        auto* const dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        auto* const nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

        const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
        auto pattern_bytes = pattern_to_byte(signature);
        auto* scan_bytes = reinterpret_cast<std::uint8_t*>(module);

        const auto s = pattern_bytes.size();
        std::uintptr_t result = 0;
        std::atomic<int> completed_threads = 0;

        auto boyer_moore_search = [&](int start_index, int end_index) {
            std::uint8_t* p = scan_bytes + start_index;
            std::uint8_t* e = scan_bytes + end_index;
            while (p < e) {
                bool match = true;
                for (std::size_t i = 0; i < s; ++i) {
                    if (pattern_bytes[i].second) {
                        continue;
                    }
                    if (p[i] != pattern_bytes[i].first) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    result = reinterpret_cast<std::uintptr_t>(p + add);
                    if (rip) {
                        result = (result + *(std::int32_t*)result) + 4;
                    }
                    std::stringstream result_as_hex;
                    result_as_hex << std::hex << std::uppercase << result;
                    g_log.send("Memory", "Found {} at {}", name, result_as_hex.str());
                    g_memory.m_sig_count++;
                    break;
                }
                ++p;
            }
            ++completed_threads;
        };
        constexpr int num_threads = 10;
        const int range_size = size_of_image / num_threads;
        std::thread threads[num_threads];
        for (int t = 0; t < num_threads; ++t) {
            int start_index = t * range_size;
            int end_index = start_index + range_size;
            if (t == num_threads - 1) {
                end_index = size_of_image - s;
            }
            threads[t] = std::thread(boyer_moore_search, start_index, end_index);
        }

        for (auto& t : threads) {
            t.join();
        }
        return result;
    }
 }





