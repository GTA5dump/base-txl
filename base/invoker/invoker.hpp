#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
#include "crossmap.hpp"
namespace base {
    struct native_call_context : public rage::scrNativeCallContext {
        native_call_context();

        std::uint64_t m_return_stack[10];
        std::uint64_t m_arg_stack[100];
    };

    struct invoker {
        void cache_handler();
        void begin_call();
        void end_call(rage::scrNativeHash hash);
        template <typename T>
        T& get_return_value()
        {
            return *m_call_context.get_return_value<T>();
        }
        template <typename Ret, typename ...Args>
        FORCEINLINE Ret invoke(rage::scrNativeHash hash, Args &&...args) {
            begin_call();
            (m_call_context.push_arg(std::forward<Args>(args)), ...);
            end_call(hash);
            if constexpr (!std::is_same_v<Ret, void>) {
                return get_return_value<Ret>();
            }
        }

        std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_cache;
        native_call_context m_call_context;
    };
    inline invoker g_invoker;
}