#pragma once
namespace base {
	extern void home_submenu();
	extern void settings_submenu();
	extern void settings_options_submenu();
	extern void settings_header_submenu();
	extern void settings_submenu_submenu();
	extern void settings_options_text_color();
	extern void settings_header_background_color();
	extern void spawner_submenu();
	extern void spawner_vehicle_submenu();
	extern void demo_submenu();
	void main_script();
	struct script {
		void attach();
		void detach();
	};
	inline script g_script;
	inline int m_selected_vehicle_class;
}