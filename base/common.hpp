#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <minwindef.h>
#include <filesystem>

#include <vector>
#include "utilities/utilities.hpp"
#include "memory/memory.hpp"
#include "pointers/pointers.hpp"

using namespace std::chrono_literals;

namespace base {
	inline bool g_running = true;
	inline const char* m_menu_name = "base";
}