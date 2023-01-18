#define _CRT_SECURE_NO_WARNINGS
#include "log.hpp"
namespace base {
	void log::attach() {
		init_console_handles();
		init_files_handles();
		g_log.send("Log", "Attached");
	}

	void log::detach() {
		g_log.send("Log", "Detached");
		free_console_handles();
		free_file_handles();
	}

	void log::init_console_handles() {
		AttachConsole(GetCurrentProcessId());
		AllocConsole();
		SetConsoleTitleA(m_menu_name);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		std::this_thread::sleep_for(40ms);
		m_console_hwnd = FindWindowA(NULL, m_menu_name);
		SetLayeredWindowAttributes(m_console_hwnd, NULL, 200, LWA_ALPHA);
		SetWindowPos(m_console_hwnd, m_console_hwnd, 0, 0, 600, 480, SWP_FRAMECHANGED | WS_VISIBLE);
	}
	void log::init_files_handles() {
		m_console.open("CONOUT$");
		try {
			m_path.append(g_utility.get_documents_path());
			m_path.append(m_menu_name);
			if (!std::filesystem::exists(m_path))
				std::filesystem::create_directory(m_path);
			m_path.append(std::format("{}.{}",m_menu_name, "log"));
		}
		catch (std::filesystem::filesystem_error const& err) {
			g_log.send("std::filesystem error %s\n", err.what());
		}
		m_file.open(m_path, std::ios_base::out | std::ios_base::app);
		m_file.clear();
	}

	void log::free_console_handles() {
		fclose(stdout);
		FreeConsole();
	}
	
	void log::free_file_handles() {
		m_console.clear();
		m_path.clear();
		m_console.close();
		m_file.close();
	}
}