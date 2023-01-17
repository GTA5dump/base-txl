#pragma once
#include "../common.hpp"
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
namespace base {
	struct log {
		void attach() {
			init_console_handles();
			init_files_handles();
		}

		void deattach() {
			free_console_handles();
			free_file_handles();
		}

		void init_console_handles();
		void init_files_handles();
		void free_console_handles();
		void free_file_handles();

		template <typename ...arguments>
		void send(std::string title, std::string message, arguments... args) {
			auto get_time = std::time(nullptr);
			auto time_struct = std::localtime(&get_time);
			std::string messageS = std::format("[{:0>2}:{:0>2}:{:0>2}", time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec) + "] [" + title + "] " + std::vformat(message, std::make_format_args(args...));
			m_console << messageS << std::endl;
			m_file << messageS << '\n';
		}

		std::filesystem::path m_path{};
		std::ofstream m_file{};
		std::ofstream m_console{};
		HWND m_console_hwnd;
	};
	inline log g_log;
}