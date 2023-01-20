#include "../ui/interface.hpp"
#include "../fiber/fiber.hpp"
#include "script.hpp"
#include "../invoker/Natives.hpp"
#include "../log/log.hpp"
namespace base {
	void home_submenu() {
		g_interface.draw_sub("Demo", demo_submenu);
		g_interface.draw_sub("Demo", demo_submenu);
		g_interface.draw_sub("Spawner", spawner_submenu);
		g_interface.draw_sub("Settings", settings_submenu);
	}
	void settings_submenu() {
		g_interface.draw_sub("Options", settings_options_submenu);
		g_interface.draw_sub("Header", settings_header_submenu);
		g_interface.draw_sub("Sub Menu", settings_submenu_submenu);
		g_interface.draw_break("Miscellaneous");
		g_interface.draw_number("Menu Pos X", &g_interface.m_menu_pos.x, 0.01f, 1.f, 0.01f);
		g_interface.draw_number("Menu Pos Y", &g_interface.m_menu_pos.y, 0.01f, 1.f, 0.01f);
		g_interface.draw_bool("Render Globe", &g_interface.m_render_glare);
		g_interface.draw_number("Menu Scale", &g_interface.m_global_scale, 0.1f, 10.f, 0.1f);
	}
	void settings_options_submenu() {
		g_interface.draw_sub("Option Text Color", settings_options_text_color); // You can do others ur self :))
		g_interface.draw_number("Option Height", &g_interface.m_option_height, 0.f, 0.070f, 0.001f, false);
		g_interface.draw_number("Font Size", &g_interface.m_option_font_size, 0.0f, 2.f, 0.01f);
		g_interface.draw_number("Font", &g_interface.m_option_font, 0, 7);
		g_interface.draw_break("Break");
		g_interface.draw_number("Font Size", &g_interface.m_break_font_size, 0.0f, 2.f, 0.01f);
		g_interface.draw_number("Font", &g_interface.m_option_font, 0, 7);
		g_interface.draw_break("Sprites");
		g_interface.draw_number("Toggle Size", &g_interface.m_toggle_size, 0.f, 0.1f, 0.001f, false);
		g_interface.draw_number("Rotated Arrow Size", &g_interface.m_arrow_rotated_size, 0.f, 0.1f, 0.001f, false);
	}
	void settings_header_submenu() {
		g_interface.draw_sub("Header Background Color", settings_header_bg_color); 
		g_interface.draw_number("Header Height", &g_interface.m_header_height, 0.f, 0.200f, 0.001f, false);
	}
	void settings_submenu_submenu() {
	}
	void settings_options_text_color() {
		g_interface.draw_number("Red", &g_interface.m_option_text.r, 0, 255, 1);
		g_interface.draw_number("Green", &g_interface.m_option_text.g, 0, 255, 1);
		g_interface.draw_number("Blue", &g_interface.m_option_text.b, 0, 255, 1);
		g_interface.draw_number("Alpha", &g_interface.m_option_text.a, 0, 255, 1);
		g_interface.draw_rect(g_interface.m_option_text, Vector2(g_interface.m_menu_pos.x, g_interface.m_draw_base_y + (g_interface.m_option_height / 2.f)), Vector2(g_interface.m_width, g_interface.m_option_height));
	}
	void settings_header_bg_color() {
		g_interface.draw_number("Red", &g_interface.m_header_back_ground.r, 0, 255, 1);
		g_interface.draw_number("Green", &g_interface.m_header_back_ground.g, 0, 255, 1);
		g_interface.draw_number("Blue", &g_interface.m_header_back_ground.b, 0, 255, 1);
		g_interface.draw_number("Alpha", &g_interface.m_header_back_ground.a, 0, 255, 1);
		g_interface.draw_rect(g_interface.m_header_back_ground, Vector2(g_interface.m_menu_pos.x, g_interface.m_draw_base_y + (g_interface.m_option_height / 2.f)), Vector2(g_interface.m_width, g_interface.m_option_height));
	}
	void settings_footer_submenu() {
		g_interface.draw_sub("Footer Background Color", settings_footer_submenu_bg_color);
		g_interface.draw_bool("Dynamic Footer", &g_interface.m_dynamic_footer);
		g_interface.draw_number("Footer Height", &g_interface.m_footer_height, 0.f, 0.170f, 0.001f, false);
		g_interface.draw_number("Footer Arrow Size", &g_interface.m_footer_arrow_size, 0.f, 0.170f, 0.001f, false);
	}
	void settings_footer_submenu_bg_color() {
		g_interface.draw_number("Red", &g_interface.m_footer_background.r, 0, 255, 1);
		g_interface.draw_number("Green", &g_interface.m_footer_background.g, 0, 255, 1);
		g_interface.draw_number("Blue", &g_interface.m_footer_background.b, 0, 255, 1);
		g_interface.draw_number("Alpha", &g_interface.m_footer_background.a, 0, 255, 1);
		g_interface.draw_rect(g_interface.m_footer_background, Vector2(g_interface.m_menu_pos.x, g_interface.m_draw_base_y + (g_interface.m_option_height / 2.f)), Vector2(g_interface.m_width, g_interface.m_option_height));
	}
	void spawner_submenu() {
		for (std::int32_t i = 0; i < 23; i++) {
			if (g_interface.draw_sub(g_gta_utility.get_vehicle_class_name(i), spawner_vehicle_submenu)) {
				m_selected_vehicle_class = i;
			}
		}
	}
	void spawner_vehicle_submenu() {
		if (g_pointers.m_vehicle_hash_pool != nullptr) {
			for (std::int32_t i = 0; i < g_pointers.m_vehicle_hash_pool->capacity; i++) {
				std::uint64_t info = g_pointers.m_vehicle_hash_pool->get(i);
				if (info != NULL) {
					if ((*(BYTE*)(info + 157) & 0x1F) == 5) {
						std::string make_ptr = (char*)((uintptr_t)info + 0x2A4);
						std::string model_ptr = (char*)((uintptr_t)info + 0x298);
						if (VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(joaat(model_ptr)) == m_selected_vehicle_class) {
							std::stringstream ss;
							std::string make(make_ptr);
							std::string model(model_ptr);
							if (make[0] || model[0]) {
								make = HUD::GET_LABEL_TEXT_(make.c_str());
								model = HUD::GET_LABEL_TEXT_(model.c_str());
								if (make != "NULL" && model != "NULL") {
									ss << make << " " << model;
								}
								else if (model != "NULL") {
									ss << model;
								}
								else {
									ss << "Unknown";
								}
							}
							if (g_interface.draw_regular(ss.str().c_str())) {
								g_gta_utility.spawn_vehicle(*(std::uint32_t*)(info + 0x18));
							}
						}
					}
				}
			}
		}
	}
	void demo_submenu() {
		if (g_interface.draw_regular("Example Option")) {
			g_log.send("Menu", "Invoke Function");
		}
		if (g_interface.draw_bool("Example Toggle", &bool_example)) {
			g_log.send("Menu", "Invoke Function");
		}
		if (g_interface.draw_number("Example Number", &number_example, 0, 10, 2, true)) {
			g_log.send("Menu", "Invoke Function");
		}
		if (g_interface.draw_array("Example Array", array_example, &array_position_example, true)) {
			g_log.send("Menu", "Value is {}", array_example[array_position_example]);
		}
		if (g_interface.draw_bool_number("Example Bool Number", &bool_example, &number_example, 0, 10, 2, true)) {
			g_log.send("Menu", "Invoke Function");
		}
		if (g_interface.draw_bool_array("Example Bool Array", &bool_example, array_example, &array_position_example, true)) {
			g_log.send("Menu", "Value is {} & bool value is", array_example[array_position_example], bool_example);
		}
	}
	void main_script() {
		while (true) {
			g_interface.input_tick();
			g_interface.draw_tick();
			g_fibers.wait(&g_main_fiber);
		}
	}
	void script::attach() {
		g_fibers.add(&g_main_fiber, &main_script);
		g_log.send("Fibers", "Created Main Fiber");
	}
	void script::detach() {
		g_fibers.remove(&g_main_fiber);
		g_log.send("Fibers", "Removed Main Fiber");
	}
}