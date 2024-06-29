#pragma once
#include "imgui.h"
struct globals
{
	bool active = true;
	bool DebugMode = true;
	bool AutoLogin = true;

	bool show_login = true;
	bool swow_accept_login = false;
	bool time;
	bool show = true;

	ImFont* Font3;
	ImFont* Font4;
	ImFont* Font1;
	ImFont* Tahoma;

	ImFont* SubTab;
	ImFont* Font;
	ImFont* FontBold;
	ImFont* Icons;
	ImFont* Icons2;
	ImFont* tester;

	enum pages
	{
		login,
		loading,
		loader
	};
	pages tab{ login };

	enum mtabs
	{
		subscriptions,
		announcements
	};
	mtabs microtabs{ subscriptions };
};
globals global;

struct c_ImGuiS {
	bool time;
	float switch_alpha[3], open_alpha = 0;
	bool show_login = true;
	bool accept_login = true;
	bool toggled = true;
};
inline c_ImGuiS ImGuiS;
