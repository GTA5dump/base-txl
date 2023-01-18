#pragma once
namespace base {
	class script {
	public:
		void attach();
		void detach();
	};
	inline script g_script;

	inline const char* array_example[] = {
	"Place Holder 1",
	"Place Holder 2",
	"Place Holder 3"
	};
	inline int array_position_example;
	inline int number_example;
	inline bool bool_example;
	inline int m_selected_vehicle_class;

	void main_script();

	void home_submenu();
	void settings_submenu();
	void settings_options_submenu();
	void settings_header_submenu();
	void settings_submenu_submenu();
	void settings_footer_submenu();
	void settings_footer_submenu_bg_color();
	void settings_options_text_color();
	void settings_header_bg_color();
	void spawner_submenu();
	void spawner_vehicle_submenu();
	void demo_submenu();
}