#include "window.hpp"
#include "skcrypt.h"
int main()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
	{
		std::filesystem::create_directories(e("C:\\darkaether\\configs"));
		if (!std::filesystem::exists(e("C:\\darkaether\\configs\\legit.cfg")))
		{
			std::ofstream(e("C:\\darkaether\\configs\\legit.cfg"));
			std::ofstream(e("C:\\darkaether\\configs\\semi_rage.cfg"));
			std::ofstream(e("C:\\darkaether\\configs\\rage.cfg"));
		}
		config.run();
	}
	driver.game_assembly = utils::get_module(utils::globals::process_id, e(L"GameAssembly.dll")); // get gameassembly module
	driver.unity_player = utils::get_module(utils::globals::process_id, e(L"UnityPlayer.dll")); // get unityplayer module

	std::thread([&]() // check if the game is open and get system width and height
		{
			while (true)
			{
				config.Includes.Setiings.width = GetSystemMetrics(SM_CXSCREEN);
				config.Includes.Setiings.height = GetSystemMetrics(SM_CYSCREEN);
				std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			}
		}
	).detach();

	std::thread([&]() // entity loop
		{
			while (true)
			{
				std::chrono::milliseconds(750);
				entity_loop::get_list();
			}
		}
	).detach();

	std::thread([&]() // aimbot loop
		{
			while (true)
			{
				std::chrono::milliseconds(3);
				aimbot::aimbot_loop();
			}
		}
	).detach();

	std::thread([&]() // window loop
		{
			while (true)
			{
				std::chrono::milliseconds(2000);
				window::create_window(window::render_menu);
			}
		}
	).detach();

	std::thread([&]() // 0 ms loop
		{
			while (true)
			{
				std::chrono::milliseconds(1);
				features::fast_features();
			}
		}
	).detach();

	std::thread([&]() // weapon feature loop
		{
			while (true)
			{
				std::chrono::milliseconds(3);
				features::weapon_features();
			}
		}
	).detach();

	std::thread([&]() // movement feature loop
		{
			while (true)
			{
				std::chrono::milliseconds(3);
				features::movement_features();
			}
		}
	).detach();

	std::thread([&]() // misc feature loop
		{
			while (true)
			{
				std::chrono::milliseconds(3);
				features::misc_features();
			}
		}
	).detach();
	Sleep(-1);
}