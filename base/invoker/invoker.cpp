#include "invoker.hpp"
#include "../log/log.hpp"
namespace base {
	native_call_context::native_call_context() {
		m_return_value = &m_return_stack[0];
		m_args = &m_arg_stack[0];
	}
	void invoker::cache_handler() {
		for (const rage::scrNativeMapping& mapping : g_crossmap) {
		   rage::scrNativeHandler handler = g_pointers.m_get_native_handler(g_pointers.m_native_registration_table, mapping.second);
		   m_cache.emplace(mapping.first, handler);	
		}
		g_log.send("Invoker", "Natives cached");
	}

	void invoker::begin_call() {
		m_call_context.reset();
	}

	void invoker::end_call(rage::scrNativeHash hash) {
		if (auto it = m_cache.find(hash); it != m_cache.end()) {
			rage::scrNativeHandler handler = it->second;
			handler(&m_call_context);
			g_pointers.m_fix_vector(&m_call_context);		
		}
		else {
			g_log.send("invoker", "failed to find {} in crossmap", hash);
		}
	}
}