#include "core/core.hpp"
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID) {
	using namespace base;
	DisableThreadLibraryCalls(hInstance);
	g_core.m_module = hInstance;
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		g_core.attach();
	} break;
	case DLL_PROCESS_DETACH: {
		g_core.detach();
	} break;
	}
	return TRUE;
}
