#pragma once
#include "../common.hpp"
#include "../fiber/fiber.hpp"
namespace base {
	class interface {
	public:
		float get_text_width(char const* text, int32_t font, float scale_override);
		float get_text_height(int32_t font, float size);
		Vector2 get_spirit_scale(float size);
		
		void draw_left_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline);
		void draw_right_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline);
		void draw_center_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline);

		void draw_sprite(char const* dict, char const* texture, Vector2 position, Vector2 size, float rotation, RGBA color);
		void draw_rect(RGBA color, Vector2 position, Vector2 size);

		void forward_menu(void(*menu)(), char const* submenu);
		void back_menu();

		void draw_option_background();

		void play_frontend_sound(char const* sound_name);

		// Variables 
	public:
		bool m_opened = { false };

		Vector2 m_menu_pos = { 0.8f, 0.1f };

		float m_draw_base_y = { 0.f };
		float m_last_option_base = { 0.f };
		float m_global_scale = { 1.f };
		float m_width = { 0.21f };
		float m_sub_padding = { 2.07f };
		float m_right_padding = { 2.0f };
		float m_left_padding = { 2.05f };

		// Header

		float m_header_height = { 0.083f };
		float m_header_font_size = { 1.f };
		int m_header_font = { 7 };

		RGBA m_header_back_ground = { 50, 100, 255, 255 };
		RGBA m_header_text = { 255, 255, 255, 255 };
		// Glare

		bool m_render_glare = { true };
		int m_glare_handle;
		float m_glare_direction;
		void draw_glare();

		// Submenu
		float m_submenu_height = { 0.035f };
		int m_submenu_font = 0;
		float m_submenu_font_size = { 0.3f };

		RGBA m_submenu_backgound = { 0, 0, 0, 255 };
		RGBA m_submenu_text = { 255, 255, 255, 255 };
		//  Option
		float m_option_height = { 0.035f };
		int m_option_font = 0;
		float m_option_font_size = { 0.3f};

		RGBA m_option_backgound = {0 , 0 , 0, 150};
		RGBA m_option_scroll = { 255, 255, 255, 255 };
		RGBA m_option_text = { 255, 255, 255, 255 };
		RGBA m_option_text_selected = { 0, 0, 0, 255 };
		// Toggle
		float m_toggle_size = { 0.028f };
		RGBA m_toggle_color_on = { 0, 255, 0, 255 };
		RGBA m_toggle_color_off = { 255, 0, 0, 255 };
		float m_arrow_rotated_size = { 0.035f };
		// Break
		int m_break_font = 1;
		float m_break_font_size = { 0.4f };

		RGBA m_break_text = { 255, 255, 255, 255 };
		// Footer 
		bool m_dynamic_footer = { true };

		float m_footer_height = { 0.035f };
		float m_footer_arrow_size = { 0.035f };

		RGBA m_footer_background = { 0, 0, 0, 255 };
		// Option Desc 
		int m_desc_font = 0;

		RGBA m_desc_background = { 0, 0, 0, 200 };
		RGBA m_desc_line = { 255, 255, 255, 255 };
		// Scroller
		float m_target = { 0.f };
		float m_current = { 0.f };
		float m_speed = { 0.1281f };

		RGBA m_scroller_color = { 255, 255, 255, 255 };
		// Options
		int m_max_vis_options = 13;
		int m_option_count;
		int m_current_option;
		int m_previous_option;
		// Menu
		int m_menu_level;
		int m_option_array[1000];

		void(*m_current_menu)();
		void(*m_menu_array[1000])();


		char const* m_current_sub_menu;
		char const* m_sub_menu_array[1000];
		char const* m_current_desc;
		// Input 
		bool m_select_pressed = false;
		bool m_up_pressed = false;
		bool m_down_pressed = false;
		bool m_left_pressed = false;
		bool m_right_pressed = false;
		int m_delay = 150;
		int m_previous_tick = 0;
		
	public:
		// Ticks
		void input_tick();
		void draw_tick();
		// Drawing 
	public:
		void draw_header();
		void draw_sub_menu(const char* sub);
		void draw_footer();
		void add_option(const char* option);
		bool draw_regular(const char* option);
		bool draw_sub(char const* option, void(*sub)());
		bool draw_bool(const char* option, bool* toggle);
		bool draw_number(const char* option, float* numeral, float min, float max, float step = 1.0f, bool action_input = false);
		bool draw_number(const char* option, int* numeral, int min, int max, int step = 1, bool action_input = false);
		bool draw_array(const char* option, const char** array, int* position, bool action_input = false);
		bool draw_bool_number(const char* option, bool* toggle, int* numeral, int min, int max, int step = 1, bool action_input = false);
		bool draw_bool_number(const char* option, bool* toggle, float* numeral, float min, float max, float step = 1.0f, bool action_input = false);
		bool draw_bool_array(const char* option, bool* toggle, const char** array, int* position, bool action_input = false);
		void draw_break(const char* option);
		// Scroller
	
		void update_scroller();
		void draw_scroller();
		
	};
	inline interface g_interface;
}