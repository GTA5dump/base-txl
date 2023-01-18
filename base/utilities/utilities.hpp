#pragma once
#include "../common.hpp"
#include "../rage/classes.hpp"
namespace base {
	class input
	{
	public:
		void initialize();
		bool is_key_pressed(int virtual_key);
	public:
		HWND m_hwnd{};
	};
	inline input g_input;

	class utility
	{
	 public:
		std::string get_documents_path();
		void load_ytd(const char* path, const char* file_name);
		const char* fixed_decimel(float number);
	};
	inline utility g_utility;

	class math {
	public:
		float convert_360(float base, float min, float max);
		float deg_to_rad(float degs);
	};
	inline math g_math;

	class gta_utility {
	public:
		const char* draw_keyboard();
		const char* get_vehicle_class_name(int id);
		void request_model(std::uint32_t hash);
		Vehicle spawn_vehicle(std::uint32_t hash);
		rage::scrProgram* get_program_by_hash(std::uint32_t hash);
	};
	inline gta_utility g_gta_utility;
}