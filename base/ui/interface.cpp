#include "interface.hpp"
#include "../invoker/Natives.hpp"
#include "../script/script.hpp"
namespace base {
	float interface::get_text_width(char const* text, int32_t font, float scaleOverride) {
		HUD::BEGIN_TEXT_COMMAND_GET_WIDTH_("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_WEBSITE(text);
		float scale = m_option_height;
		HUD::SET_TEXT_FONT(0);
		if (font != -1) {
			scale = scaleOverride * m_global_scale;
			HUD::SET_TEXT_FONT(font);
		}
		HUD::SET_TEXT_SCALE(0.0f, scale);
		return HUD::END_TEXT_COMMAND_GET_WIDTH_(TRUE);
	}

	float interface::get_text_height(int32_t font, float size) {
		return HUD::GET_RENDERED_CHARACTER_HEIGHT(size, font);
	}

	Vector2 interface::get_spirit_scale(float size) {
		int x;
		int y;
		GRAPHICS::GET_ACTIVE_SCREEN_RESOLUTION_(&x, &y);
		return Vector2(((float)y / (float)x) * size, size);
	}

	void interface::draw_left_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline) {
		HUD::SET_TEXT_SCALE(size * m_global_scale, size * m_global_scale);
		HUD::SET_TEXT_FONT(font);
		HUD::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
		if (outline)
			HUD::SET_TEXT_OUTLINE();
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(position.x * m_global_scale, position.y * m_global_scale, 0);
	}

	void interface::draw_right_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline) {
		HUD::SET_TEXT_WRAP(0.f, position.x * m_global_scale);
		HUD::SET_TEXT_RIGHT_JUSTIFY(true);
		draw_left_text(text, color, position, size, font, outline);
	}

	void interface::draw_center_text(char const* text, RGBA color, Vector2 position, float size, int font, bool outline) {
	    HUD::SET_TEXT_CENTRE(true);
		draw_left_text(text, color, position, size, font, outline);
	}

	void interface::draw_sprite(char const* dict, char const* texture, Vector2 position, Vector2 size, float rotation, RGBA color) {
		if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict))
			GRAPHICS::DRAW_SPRITE(dict, texture, position.x * m_global_scale, position.y * m_global_scale, size.x * m_global_scale, size.y * m_global_scale, rotation, color.r, color.g, color.b, color.a);
		else
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, false);
	}

	void interface::draw_rect(RGBA color, Vector2 position, Vector2 size) {
		GRAPHICS::DRAW_RECT(position.x * m_global_scale, position.y * m_global_scale, size.x * m_global_scale, size.y * m_global_scale, color.r, color.g, color.b, color.a, 0);
	}

	void interface::play_frontend_sound(char const* sound_name) {
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
	}

	void interface::forward_menu(void(*menu)(), char const* sub_menu) {
		m_menu_array[m_menu_level] = m_current_menu;
		m_sub_menu_array[m_menu_level] = m_current_sub_menu;
		m_option_array[m_menu_level] = m_current_option;
		m_menu_level++;
		m_current_menu = menu;
		m_current_sub_menu = sub_menu;
		m_current_option = 1;
	}

	void interface::back_menu() {
		m_menu_level--;
		m_current_menu = m_menu_array[m_menu_level];
		m_current_sub_menu = m_sub_menu_array[m_menu_level];
		m_current_option = m_option_array[m_menu_level];
	}

	void interface::input_tick() {
		if (GetTickCount64() - m_previous_tick > m_delay) {
			if (GetAsyncKeyState(VK_INSERT) || PAD::IS_DISABLED_CONTROL_PRESSED(2, 21) && PAD::IS_DISABLED_CONTROL_PRESSED(2, 20)) {
				if (m_menu_level == 0)
				forward_menu(home_submenu, "Home");
				play_frontend_sound(m_opened ? "SELECT" : "BACK");
				m_opened = !m_opened;
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 194)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("BACK");
					if (m_menu_level == 1)
						m_opened = false;
					else if (m_menu_level != 1)
						back_menu();
				}
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 194)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("BACK");
					if (m_menu_level == 1)
						m_opened = false;
					else if (m_menu_level != 1)
						back_menu();
				}
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 172)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("NAV_UP_DOWN");
					if (m_current_option > 1)
						m_current_option--;
					else
						m_current_option = m_option_count;
				}
				m_up_pressed = true;
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 173)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("NAV_UP_DOWN");
					if (m_current_option < m_option_count)
						m_current_option++;
					else
						m_current_option = 1;
				}
				m_down_pressed = true;
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 174)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("NAV_LEFT_RIGHT");
					m_left_pressed = true;
				}
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 175)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("NAV_LEFT_RIGHT");
					m_right_pressed = true;
				}
				m_previous_tick = GetTickCount64();
			}
			else if (PAD::IS_DISABLED_CONTROL_PRESSED(2, 191)) {
				if (m_opened && m_menu_level != 0) {
					play_frontend_sound("SELECT");
					m_select_pressed = true;
				}
				m_previous_tick = GetTickCount64();
			}
		}
	}

	void interface::draw_tick() {
		HUD::DISPLAY_HUD_WHEN_PAUSED_THIS_FRAME();
		GRAPHICS::FORCE_RENDER_IN_GAME_UI(true);
		GRAPHICS::SET_SCRIPT_GFX_DRAW_BEHIND_PAUSEMENU(true);
		if (m_menu_level != 0 && m_opened) {
			m_draw_base_y = m_menu_pos.y;
			draw_header();
			draw_sub_menu(m_current_sub_menu);
			draw_option_background();
			m_current_menu();
			draw_footer();
		}
		m_select_pressed = false;
		m_up_pressed = false;
		m_down_pressed = false;
		m_left_pressed = false;
		m_right_pressed = false;
	}

	void interface::draw_glare() {
		if (m_render_glare) {
			m_glare_handle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
			Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
			float dir = g_math.convert_360(rot.z, 0, 360);
			if ((m_glare_direction == 0 || m_glare_direction - dir > 0.5) || m_glare_direction - dir < -0.5)
			{
				m_glare_direction = dir;
				GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_glare_handle, "SET_DATA_SLOT");
				GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(m_glare_direction);
				GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			}
			GRAPHICS::DRAW_SCALEFORM_MOVIE(m_glare_handle, (m_menu_pos.x + 0.339f) * m_global_scale, (m_draw_base_y + (m_header_height / 2.f) + 0.38f) * m_global_scale, (m_width + 0.82700f) * m_global_scale, (m_header_height + 0.852f) * m_global_scale, 255, 255, 255, 255, 0);
		}
	}

	void interface::draw_header() {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		PAD::DISABLE_CONTROL_ACTION(2, 27, TRUE);
		draw_rect(m_header_back_ground, Vector2(m_menu_pos.x, m_draw_base_y + (m_header_height / 2.f)), Vector2(m_width, m_header_height));
		draw_center_text(m_menu_name, m_header_text, Vector2(m_menu_pos.x, m_draw_base_y + (m_header_height / 2.f) - (get_text_height(m_header_font, m_header_font_size) / 2.f)), m_header_font_size, m_header_font, true);
		draw_glare();
		m_draw_base_y += m_header_height;
	}

	void interface::draw_sub_menu(const char* title) {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		draw_rect(m_submenu_backgound, Vector2(m_menu_pos.x, m_draw_base_y + (m_submenu_height / 2.f)), Vector2(m_width, m_submenu_height));
		draw_left_text(title, m_submenu_text, Vector2(m_menu_pos.x - (m_width / 2.06f), m_draw_base_y + (m_submenu_height / 2.f) - (get_text_height(m_submenu_font, m_submenu_font_size) / 1.5f)), m_submenu_font_size, m_submenu_font, false);
		draw_right_text(std::format("{}/{}", m_current_option, m_option_count).c_str(), m_submenu_text, Vector2(m_menu_pos.x + (m_width / m_sub_padding), m_draw_base_y + (m_submenu_height / 2.f) - (get_text_height(m_submenu_font, m_submenu_font_size) / 1.5f)), m_submenu_font_size, m_submenu_font, false);
		m_draw_base_y += m_submenu_height;
	}

	void interface::draw_option_background() {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		int visible_options = m_option_count > m_max_vis_options ? m_max_vis_options : m_option_count;
        // draw_sprite("commonmenu", "gradient_bgd", Vector2(m_menu_pos.x, m_header_height + m_submenu_height + m_menu_pos.y + ((m_option_height * visible_options) / 2.f)), Vector2(m_width, m_option_height * visible_options), 0.f, {255, 255, 255, 255}); // if u want a fade effect 
		draw_rect(m_option_backgound, Vector2(m_menu_pos.x, m_header_height + m_submenu_height + m_menu_pos.y + ((m_option_height * visible_options) / 2.f)), Vector2(m_width, m_option_height * visible_options));
		m_option_count = 0;
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(3);
	}

	void interface::update_scroller() {	
		if (m_current != m_target)
			m_current = m_current * (1 - m_speed) + m_target * m_speed;
		if ((m_current > m_target - 0.0005) && (m_current < m_target + 0.0005))
			m_current = m_target;
	}

	void interface::draw_scroller() {
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {	
			m_target = m_draw_base_y + (m_option_height / 2.f);
			update_scroller();
			GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(2);
			draw_rect(m_scroller_color, Vector2(m_menu_pos.x, m_current), Vector2(m_width, m_option_height));
			GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(3);
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			m_target = m_last_option_base + (m_option_height / 2.f);
			update_scroller();
			GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(2);
			draw_rect(m_scroller_color, Vector2(m_menu_pos.x, m_current), Vector2(m_width, m_option_height));
			GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(3);
		}
	}

	void interface::add_option(const char* option) {
		m_last_option_base = (m_header_height + (m_menu_pos.y) + m_submenu_height + ((m_option_count - (m_current_option - m_max_vis_options)) * m_option_height));
		m_option_count++;
		bool selected = m_current_option == m_option_count ? true : false;
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			draw_left_text(option, selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x - (m_width / m_left_padding), m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			if (selected)
				draw_scroller();
		}
		else if ((m_option_count > (m_current_option - m_max_vis_options)) && m_option_count <= m_current_option) {
			draw_left_text(option, selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x - (m_width / m_left_padding), m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			if (selected)
				draw_scroller();
		}
	}

	bool interface::draw_regular(const char* option) {
		add_option(option);
		if (m_option_count <= m_max_vis_options)
			m_draw_base_y += m_option_height;
		if (m_option_count == m_current_option)
			if (m_select_pressed)
				return true;
		return false;
	}

	bool interface::draw_sub(char const* option, void(*sub)()) {
		add_option(option);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			draw_rect(m_header_back_ground, Vector2(m_menu_pos.x + (m_width / m_right_padding) - (0.0035f / 2.f), m_draw_base_y + (m_option_height / 2.f)), Vector2(0.0035f, m_option_height));
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			draw_rect(m_header_back_ground, Vector2(m_menu_pos.x + (m_width / m_right_padding) - (0.0035f / 2.f), m_last_option_base + (m_option_height / 2.f)), Vector2(0.0035f, m_option_height));
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (m_current_option == m_option_count) {
			if (m_select_pressed) {
				forward_menu(sub, option);
				return true;
			}
		}
		return false;
	}

	bool interface::draw_bool(const char* option, bool* toggle) {
		add_option(option);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) { 
			draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (m_current_option == m_option_count) {
			if (m_select_pressed) {
				*toggle = !*toggle;
				return true;
			}
		}
		return false;
	}

	bool interface::draw_number(const char* option, float* numeral, float min, float max, float step, bool action_input) {
		add_option(option); 
		bool selected = m_current_option == m_option_count ? true : false;
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0015f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0015f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (m_option_count == m_current_option) {
			if (m_left_pressed) {
				if (*numeral >= min) {
					*numeral -= step;
				} else {
					*numeral = max;
				}
				if (action_input) {
				  return true;
				}
			}
			if (m_right_pressed) {
				if (*numeral < max) {
					*numeral += step;
				} else {
					*numeral = min;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				const char* keyboard = g_gta_utility.draw_keyboard();
				if (keyboard) {
					*numeral = std::atof(keyboard);
				}
				return true;
			}
		}
		return false;
	}

	bool interface::draw_number(const char* option, int* numeral, int min, int max, int step, bool action_input) {
		add_option(option);
		bool selected = m_current_option == m_option_count ? true : false;
		std::string right_text = std::format("{}", *numeral);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0015f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0015f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (selected) {
			if (m_left_pressed) {
				if (*numeral >= min) {
					*numeral -= step;
				}
				else {
					*numeral = max;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_right_pressed) {
				if (*numeral < max) {
					*numeral += step;
				}
				else {
					*numeral = min;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				const char* keyboard = g_gta_utility.draw_keyboard();
				if (keyboard) {
					*numeral = std::atoi(keyboard);
				}
				return true;
			}
		}
		return false;
	}

	bool interface::draw_array(const char* option, const char** array, int* position, bool action_input) {
		add_option(option);
		bool selected = m_current_option == m_option_count ? true : false;
		int max = sizeof(array) / sizeof(array[0]) + 1;
		std::string right_text = std::format("{}", array[*position]);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0015f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.0165f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding), m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (m_current_option == m_option_count) {
			if (m_left_pressed) {
				if (*position > 0) {
					*position -= 1;
				}
				else {
					*position = max;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_right_pressed) {
				if (*position < max) {
					*position += 1;
				}
				else {
					*position = 0;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				return true;
			}
		}
		return false;
	}

	bool interface::draw_bool_number(const char* option, bool* toggle, int* numeral, int min, int max, int step, bool action_input) {
		add_option(option);
		bool selected = m_current_option == m_option_count ? true : false;
		std::string right_text = std::format("{}", *numeral);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 1.8f), m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (selected) {
			if (m_left_pressed) {
				if (*numeral >= min) {
					*numeral -= step;
				}
				else {
					*numeral = max;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_right_pressed) {
				if (*numeral < max) {
					*numeral += step;
				}
				else {
					*numeral = min;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				*toggle = !*toggle;
				if (g_input.is_key_pressed(VK_LSHIFT)) {				
					const char* keyboard = g_gta_utility.draw_keyboard();
					if (keyboard) {
						*numeral = std::atoi(keyboard);
					}
				}
				return true;
			}
		}
		return false;
	}

	bool interface::draw_bool_number(const char* option, bool* toggle, float* numeral, float min, float max, float step, bool action_input) {
		add_option(option);
		bool selected = m_current_option == m_option_count ? true : false;
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 1.8f), m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(g_utility.fixed_decimel(*numeral), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (selected) {
			if (m_left_pressed) {
				if (*numeral >= min) {
					*numeral -= step;
				}
				else {
					*numeral = max;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_right_pressed) {
				if (*numeral < max) {
					*numeral += step;
				}
				else {
					*numeral = min;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				*toggle = !*toggle;
				if (g_input.is_key_pressed(VK_LSHIFT)) {
					const char* keyboard = g_gta_utility.draw_keyboard();
					if (keyboard) {
						*numeral = std::atof(keyboard);
					}
				}
				return true;
			}
		}
		return false;
	}

	bool interface::draw_bool_array(const char* option, bool* toggle, const char** array, int* position, bool action_input) {
		add_option(option);
		bool selected = m_current_option == m_option_count ? true : false;
		int max = sizeof(array) / sizeof(array[0]) + 1;
		std::string right_text = std::format("{}", array[*position]);
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 2.8f), m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_draw_base_y + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			if (selected) {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_sprite("commonmenu", "shop_arrows_upanddown", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_arrow_rotated_size).x / 2.f) - (get_spirit_scale(m_toggle_size).x / 2.8f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_arrow_rotated_size), 90.f, selected ? m_option_text_selected : m_option_text);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.016f - (get_spirit_scale(m_toggle_size).x / 1.8f), m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
			else {
				draw_sprite("commonmenu", "common_medal", Vector2(m_menu_pos.x + (m_width / m_right_padding) - (get_spirit_scale(m_toggle_size).x / 3.f), m_last_option_base + (m_option_height / 2.f)), get_spirit_scale(m_toggle_size), 0.f, *toggle ? m_toggle_color_on : m_toggle_color_off);
				draw_right_text(right_text.c_str(), selected ? m_option_text_selected : m_option_text, Vector2(m_menu_pos.x + (m_width / m_right_padding) - 0.014f, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_option_font, m_option_font_size) / 1.5f)), m_option_font_size, m_option_font, false);
			}
		}
		if (m_option_count <= m_max_vis_options) {
			m_draw_base_y += m_option_height;
		}
		if (m_current_option == m_option_count) {
			if (m_left_pressed) {
				if (*position > 0) {
					*position -= 1;
				}
				else {
					*position = max;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_right_pressed) {
				if (*position < max) {
					*position += 1;
				}
				else {
					*position = 0;
				}
				if (action_input) {
					return true;
				}
			}
			if (m_select_pressed) {
				*toggle = !*toggle;
				return true;
			}
		}
		return false;
	}

	void interface::draw_break(const char* option) {
		m_last_option_base = (m_header_height + (m_menu_pos.y) + m_submenu_height + (m_max_vis_options - 1) * m_option_height);
		m_option_count++;
		if (m_up_pressed && m_option_count == m_current_option) {
			m_current_option--;
		}
		if (m_down_pressed && m_option_count == m_current_option) {
			m_current_option++;
		}
		bool selected = m_current_option == m_option_count ? true : false;
		if (m_current_option <= m_max_vis_options && m_option_count <= m_max_vis_options) {
			draw_center_text(option, m_break_text, Vector2(m_menu_pos.x, m_draw_base_y + (m_option_height / 2.f) - (get_text_height(m_break_font, m_break_font_size) / 1.5f)), m_break_font_size, m_break_font, false);
			if (selected)
				draw_scroller();
		}
		else if (m_option_count > (m_current_option - m_max_vis_options) && m_option_count <= m_current_option) {
			draw_center_text(option, m_break_text, Vector2(m_menu_pos.x, m_last_option_base + (m_option_height / 2.f) - (get_text_height(m_break_font, m_break_font_size) / 1.5f)), m_break_font_size, m_break_font, false);
			if (selected)
				draw_scroller();
		}
		if (m_option_count <= m_max_vis_options)
			m_draw_base_y += m_option_height;
	}

	void interface::draw_footer() {
		float size = m_footer_arrow_size;
		float rotation = 0.f;
		const char* texture = "shop_arrows_upanddown";
		if (m_current_option == 1) {
			rotation = 90.f;
			texture = "arrowright";
			size *= 0.7f;
		}
		else if (m_current_option == m_option_count) {
			rotation = 270.f;
			texture = "arrowright";
			size *= 0.7f;
		}
		if (m_option_count >= m_max_vis_options && m_dynamic_footer) {
			Vector2 sizee = get_spirit_scale(size);
			draw_rect(m_footer_background, Vector2(m_menu_pos.x, m_draw_base_y + (m_footer_height / 2.f)), Vector2(m_width, m_footer_height));
			draw_sprite("commonmenu", texture, Vector2(m_menu_pos.x, m_draw_base_y + (m_footer_height / 2.f)), sizee, rotation, {255, 255, 255, 255});
			m_draw_base_y += m_footer_height;
		}
		else if (!m_dynamic_footer) {
			Vector2 sizee = get_spirit_scale(size);
			draw_rect(m_footer_background, Vector2(m_menu_pos.x, m_draw_base_y + (m_footer_height / 2.f)), Vector2(m_width, m_footer_height));
			draw_sprite("commonmenu", texture, Vector2(m_menu_pos.x, m_draw_base_y + (m_footer_height / 2.f)), sizee, rotation, { 255, 255, 255, 255 });
			m_draw_base_y += m_footer_height;
		}
		
	}
}


