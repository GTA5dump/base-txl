#define _CRT_SECURE_NO_WARNINGS
#include "utilities.hpp"
#include <bitset>
#include <shlobj.h>
#include "../invoker/Natives.hpp"
#include "../fiber/fiber.hpp"
#pragma comment(lib, "shell32.lib")
namespace base {
	void input::initialize() {
		m_hwnd = FindWindowA("grcWindow", nullptr);
	}

	bool input::is_key_pressed(const int virtual_key) {
		if (GetForegroundWindow() == m_hwnd) {
			if (GetAsyncKeyState(virtual_key) & 0x8000) {
				return true;
			}
		}
		return false;
	}

	std::string utility::get_documents_path() {
		wchar_t Folder[1024];
		HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
		if (SUCCEEDED(hr))
		{
			char str[1024];
			wcstombs(str, Folder, 1023);
			return str;
		}
		else return "";
	}

	void utility::load_ytd(const char* path, const char* file_name) {
		int id;
		g_pointers.m_register_file(&id, path, true, file_name, false);
	}

	const char* utility::fixed_decimel(float number) {
		double intpart;
		double fracpart = std::modf(number, &intpart);
		std::stringstream ss;
		ss << intpart;
		std::string int_part = ss.str();
		std::stringstream s;
		s << fracpart;
		std::string frac_part = s.str();
		frac_part.erase(0, 1);
		std::string text = int_part + frac_part;
		return text.c_str();
	}

	const char* gta_utility::draw_keyboard() {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 10);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
			g_fibers.wait(&g_main_fiber);
		}
		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) {
			return 0;
		}
		return MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}

	rage::scrProgram* gta_utility::get_program_by_hash(std::uint32_t hash) {
		std::uint32_t programs = *(std::uint32_t*)(g_pointers.m_streamed_scripts + 0x18);
		std::uint64_t program_list = *(std::uint64_t*)g_pointers.m_streamed_scripts;
		for (std::uint32_t i = 0; i < programs; i++) {
			std::uint64_t program = *(std::uint64_t*)(program_list + (i * 0x10));
			std::uint32_t phash = *(std::uint32_t*)(program_list + (i * 0x10) + 0xC);
			if (phash == hash)
				return (rage::scrProgram*)program;
		}
		return nullptr;
    }

	const char* gta_utility::get_vehicle_class_name(int id) {
		std::stringstream ss; ss << "VEH_CLASS_" << id;
		return HUD::GET_LABEL_TEXT_(ss.str().c_str()) == "NULL" ? "Unknown Class" : HUD::GET_LABEL_TEXT_(ss.str().c_str());
	}

	void gta_utility::request_model(std::uint32_t hash) {
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) {
			g_fibers.wait(&g_main_fiber);
		}
	}

	Vehicle gta_utility::spawn_vehicle(std::uint32_t hash) {
		if (!STREAMING::IS_MODEL_VALID(hash)) {
			return NULL;
		}
		request_model(hash);
		float forward = 5.f;
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		float x = forward * sin(g_math.deg_to_rad(heading)) * -1.f;
		float y = forward * cos(g_math.deg_to_rad(heading));
		*(unsigned short*)g_pointers.m_set_this_thread_networked = 0x9090; // we set this thread as networked so we can spawn the vehicle / u can add rage classes and use excute under thread instead of this
		Vehicle the_vehicle = VEHICLE::CREATE_VEHICLE(hash, coords.x + x, coords.y + y, coords.z, heading, NETWORK::NETWORK_IS_SESSION_ACTIVE(), false, false);
		std::int32_t net_id = NETWORK::VEH_TO_NET(the_vehicle);
		*(unsigned short*)g_pointers.m_set_this_thread_networked = 0x0574; // We restore it so we don't get detected 
		NETWORK::NETWORK_FADE_IN_ENTITY(the_vehicle, true, 0); 
		NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(net_id, TRUE);
		if (ENTITY::IS_ENTITY_VISIBLE_TO_SCRIPT(NETWORK::NET_TO_VEH(net_id))) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), NETWORK::NET_TO_VEH(net_id), -1);
			float speed = ENTITY::GET_ENTITY_SPEED(the_vehicle);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(NETWORK::NET_TO_VEH(net_id), speed);
			VEHICLE::SET_VEHICLE_ENGINE_ON(NETWORK::NET_TO_VEH(net_id), TRUE, TRUE, TRUE);
			DECORATOR::DECOR_SET_INT(NETWORK::NET_TO_VEH(net_id), "MPBitset", (1 << 10));
			return the_vehicle;
		}
		return NULL;
	}

	float math::deg_to_rad(float degs) {
		return degs * 3.141592653589793f / 180.f;
	}

	float math::convert_360(float base, float min, float max) {
		float fVar0;
		if (min == max) return min;
		fVar0 = max - min;
		base -= std::round(base - min / fVar0) * fVar0;
		if (base < min) base += fVar0;
		return base;
	}
}
