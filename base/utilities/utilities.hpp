#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
namespace base {
	struct input
	{
		HWND m_hwnd{};

		void initialize();

		bool is_key_pressed(int virtual_key);
	};
	inline input g_input;

	struct utility
	{
		std::string get_documents_path();
		float convert_360(float base, float min, float max);
		void load_ytd(const char* path, const char* file_name);
		const char* fixed_decimel(float number);
		const char* draw_keyboard();
		const char* get_vehicle_class_name(int id);
		void request_model(std::uint32_t hash);
		Vehicle spawn_vehicle(std::uint32_t hash);
		rage::scrProgram* get_program_by_hash(std::uint32_t hash);
		float deg_to_rad(float degs);
	};
	inline utility g_utility;
}