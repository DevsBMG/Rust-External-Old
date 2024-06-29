#pragma once
#include "imports.hpp"
#include "font.h"
#include <map>
#include "../imgui/globals.h"
inline ImVec2 screen_res{ 000, 000 };
inline ImVec2 window_pos{ 0, 0 };
inline ImVec2 window_size{ 550, 384 };
ImFont* Font2;
static DWORD lastKeyPressTime = 0;
inline LPDIRECT3DDEVICE9 dev;
static int sub_tab = 0;
int tab = 0;
inline DWORD  window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
char buffer[32];
using namespace ImGui;


namespace window
{
	auto render_menu() -> void
	{
		SetNextWindowSize(ImVec2(config.Includes.Setiings.width + 15, config.Includes.Setiings.height + 15));
		SetNextWindowPos(ImVec2(-10, -10));
		SetNextWindowBgAlpha(.25f);

		Begin(e("##background"), &config.Includes.Setiings.showing, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove); {}ImGui::End();
		SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
		SetNextWindowBgAlpha(1.0f);

		SetNextWindowSize(window_size);
		Begin(e("###0f984"), &config.Includes.Setiings.showing, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		{
			ImGuiWindow* window = GetCurrentWindow();
			ImVec2 p = GetWindowPos();
			static bool navbar_collapsed = true;
			static float navbar_width = 0.f; navbar_width = ImLerp(navbar_width, navbar_collapsed ? 0.f : 1.f, 0.04f);
			PushItemFlag(ImGuiItemFlags_Disabled, !navbar_collapsed);
			auto draw = GetWindowDrawList();
			ImVec2 pos = GetWindowPos();
			ImVec2 size = GetWindowSize();
			ImGuiContext& g = *GImGui;
			//54, 53, 53
			draw->AddRectFilled(ImVec2(pos.x + 2, pos.y + 1), ImVec2(pos.x + size.x - 2, pos.y + 21), ImColor(11 / 255.0f, 11 / 255.0f, 11 / 255.0f, g.Style.Alpha), 3.f); // upper rect
			draw->AddRect(ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + size.x - 1, pos.y + size.y - 1), ImColor(1.0f, 1.0f, 1.0f, g.Style.Alpha * 0.08f), 3.f); // outline around the window
			draw->AddLine(ImVec2(pos.x + 2, pos.y + 22), ImVec2(pos.x + size.x - 2, pos.y + 22), ImColor(1.0f, 1.0f, 1.0f, g.Style.Alpha * 0.10f)); // upper line                                                             //3, 252, 128
			draw->AddRectFilledMultiColor(ImVec2(pos.x + size.x / 2 + 10, pos.y + 23), ImVec2(pos.x + size.x - 2, pos.y + 24), ImColor(87, 86, 86, 0), ImColor(107 / 255.0f, 106 / 255.0f, 106 / 255.0f, g.Style.Alpha), ImColor(138 / 255.0f, 138 / 255.0f, 138 / 255.0f, g.Style.Alpha), ImColor(163, 163, 163, 0)); // upper gradient
			SetCursorPos(ImVec2(150, -182));

			if (tab == 0)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("general"), ImVec2(195, 140));
				{
					SetCursorPosY(5); SetCursorPosX(10); ImGui::Combo("Aim Type", &config.Includes.Aimbot.selected_Aim, config.Includes.Aimbot.Aimbot_Type, IM_ARRAYSIZE(config.Includes.Aimbot.Aimbot_Type));
					SetCursorPosX(30); SetCursorPosX(10); ImGui::Combo("Aim Bone", &config.Includes.Aimbot.selected_AimBone, config.Includes.Aimbot.AimBone_Type, IM_ARRAYSIZE(config.Includes.Aimbot.AimBone_Type));
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("exploits"), ImVec2(195, 143));
				{
					SetCursorPosX(10); Checkbox(e("Ignore NPC"), &config.Includes.Aimbot.ignoreNPC);
					SetCursorPosX(10); Checkbox(e("Ignore SafeZone"), &config.Includes.Aimbot.ignoreSafe);

				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("accuracy"), ImVec2(195, 322));
				{
					SetCursorPosX(5);
					SetCursorPosY(5);
					Checkbox(e("Show Crosshair"), &config.Includes.Aimbot.crosshair);
					SetCursorPosX(5); Checkbox(e("Show Fov"), &config.Includes.Aimbot.showfov);
					SetCursorPosX(5); SliderFloat(e("Aim Fov"), &config.Includes.Aimbot.Fov_slifer, 0.0f, 360.0f, "%.1f%%");
					SetCursorPosX(5); SliderFloat(e("Aimbot Smoothing"), &config.Includes.Aimbot.smoothingFactor, 0.1f, 100.0f, "%.1f%%");
				}
				EndChild();
			}

			if (tab == 1)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("general"), ImVec2(195, 140));
				{
					SetCursorPosX(5);  SetCursorPosY(5); Checkbox(e("Automatic"), &config.Includes.Weapon.auto_matic);
					SetCursorPosX(5);  Checkbox(e("Instant Bow"), &config.Includes.Weapon.instantbow);
					SetCursorPosX(5);  Checkbox(e("Instant Eoak"), &config.Includes.Weapon.insteoka);
					SetCursorPosX(5);  Checkbox(e("Recoil Compensation"), &config.Includes.Weapon.no_recoil);
					SetCursorPosX(5);  Checkbox(e("Spread Compensation"), &config.Includes.Weapon.no_spread);
					SetCursorPosX(5);  Checkbox(e("Sway Compensation"), &config.Includes.Weapon.no_sway);
					SetCursorPosX(5);  Checkbox(e("No Animation"), &config.Includes.Weapon.no_animaion);
					SetCursorPosX(5);  Checkbox(e("Big Bullet"), &config.Includes.Weapon.Big_Bullet);
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("exploits"), ImVec2(195, 143));
				{
					//SetCursorPosX(5);  Checkbox(e("Hand Chams"), &config.Includes.Visual.HandChams);
					//SetCursorPosX(5);  ImGui::Combo("Hand Type", &config.Includes.Visual.HandChamstype, config.Includes.Visual.HandChams_types, IM_ARRAYSIZE(config.Includes.Visual.HandChams_types));
					//SetCursorPosX(5);  Checkbox(e("Weapon Chams"), &config.Includes.Visual.WeaponChams);
					//SetCursorPosX(5);  ImGui::Combo("Weapon Type", &config.Includes.Visual.WeaponChamstype, config.Includes.Visual.HandChams_types, IM_ARRAYSIZE(config.Includes.Visual.HandChams_types));
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("accuracy"), ImVec2(195, 322));
				{
					SetCursorPosX(10); SetCursorPosY(5); SliderFloat(e("X Recoil"), &config.Includes.Weapon.x_recoli, 0.0f, 1.0f, "%.2f%%");
					SetCursorPosX(10); SliderFloat(e("Y Recoil"), &config.Includes.Weapon.y_recoli, 0.0f, 1.0f, "%.2f%%");
					SetCursorPosX(10); SliderFloat(e("Spread"), &config.Includes.Weapon.no_spread_slider, -512.0f, 1.0f, "%.2f%%");
					SetCursorPosX(10); SliderFloat(e("Sway"), &config.Includes.Weapon.no_sway_slider, 0.0f, 1.0f, "%.2f%%");
				}
				EndChild();
			}

			if (tab == 2)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("general"), ImVec2(195, 140));
				{
					SetCursorPosX(5);
					SetCursorPosY(5);
					Checkbox(e("spyder man"), &config.Includes.Misc.spider_man);
					Spacing();
					SetCursorPosX(5);
					Checkbox(e("infinite jump"), &config.Includes.Misc.infinite_jump);
					Spacing();
					SetCursorPosX(5);
					Checkbox(e("always Sprint"), &config.Includes.Misc.omnis_print);
					Spacing();
					SetCursorPosX(5);
					Checkbox(e("shoot while mounted"), &config.Includes.Misc.hold_item_while_mounted);
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("exploits"), ImVec2(195, 143));
				{
					SetCursorPosX(10); SetCursorPosY(5); SliderFloat(e("FOV Changer"), &config.Includes.Misc.fov_changer, 0.0f, 140.0f, "%.1f%%");
					SetCursorPosX(10); SliderFloat(e("Zoom"), &config.Includes.Misc.zoom, 0.f, 140.f, "%.1f%%");
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("accuracy"), ImVec2(195, 322));
				{

				}
				EndChild();
			}

			if (tab == 3)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("general"), ImVec2(195, 140));
				{
					SetCursorPosX(5);  SetCursorPosY(5); Checkbox(e("Box"), &config.Includes.Visual.box_esp);
					SetCursorPosX(5);  Checkbox(e("Skeleton"), &config.Includes.Visual.skelly_esp);
					SetCursorPosX(5);  Checkbox(e("Name"), &config.Includes.Visual.name_esp);
					SetCursorPosX(5);  Checkbox(e("Distance"), &config.Includes.Visual.distance_esp);
					SetCursorPosX(5);  Checkbox(e("Health"), &config.Includes.Visual.health_esp);
					SetCursorPosX(5);  Checkbox(e("Belt"), &config.Includes.Visual.belt_esp);
					SetCursorPosX(5);  Checkbox(e("Held"), &config.Includes.Visual.held_item_esp);

					SetCursorPosX(5);   Checkbox(e("Chams"), &config.Includes.Visual.chams);
					SetCursorPosX(5);   Combo("Type", &config.Includes.Visual.chamstype, config.Includes.Visual.chams_types, IM_ARRAYSIZE(config.Includes.Visual.chams_types));
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("etc"), ImVec2(195, 143));
				{
					SetCursorPosX(5);  SliderFloat(e("Player Distance"), &config.Includes.Visual.player_distance, 100.0f, 1000.0f, "%.1f%%");
					SetCursorPosX(5);  SliderFloat(e("World Distance"), &config.Includes.Visual.etc_distance, 100.0f, 1000.0f, "%.1f%%");

				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("accuracy"), ImVec2(195, 322));
				{
					SetCursorPosX(5); Checkbox(e("Show Player"), &config.Includes.Visual.showPlayer);
					SetCursorPosX(5); Checkbox(e("Show Safe"), &config.Includes.Visual.showSafe);
				}
				EndChild();
			}

			if (tab == 4)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("generala"), ImVec2(195, 140));
				{
					SetCursorPosX(5); SetCursorPosY(5); Checkbox(e("Box"), &config.Includes.Visual.NPCbox_esp);
					SetCursorPosX(5); Checkbox(e("Skeleton"), &config.Includes.Visual.NPCskelly_esp);
					SetCursorPosX(5);  Checkbox(e("Name"), &config.Includes.Visual.NPCname_esp);
					SetCursorPosX(5);  Checkbox(e("Distance"), &config.Includes.Visual.NPCdistance_esp);
					SetCursorPosX(5);  Checkbox(e("Health"), &config.Includes.Visual.NPChealth_esp);
					SetCursorPosX(5);  Checkbox(e("Belt"), &config.Includes.Visual.NPCbelt_esp);
					SetCursorPosX(5);  Checkbox(e("Held"), &config.Includes.Visual.NPCheld_item_esp);

					//SetCursorPosX(5);  Checkbox(e("Chams"), &config.Includes.Visual.NPCchams);
					//SetCursorPosX(5);  SliderInt(e("Type"), &config.Includes.Visual.NPCchamstype, 0, 6);
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("etcc"), ImVec2(195, 143));
				{
					SetCursorPosX(5);  SliderFloat(e("NPC Distance"), &config.Includes.Visual.npc_distance, 100.0f, 1000.0f, "%.1f%%");
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("accuracyy"), ImVec2(195, 322));
				{
					SetCursorPosX(5); Checkbox(e("Show NPC"), &config.Includes.Visual.showNPC);
				}
				EndChild();
			}

			if (tab == 5)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("Node"), ImVec2(195, 140));
				{
					static std::string preview = "";
					preview = "";
					std::vector<std::string> vec;
					for (size_t i = 0; i < IM_ARRAYSIZE(config.Includes.Visual.selected_node_esp_type); i++)
					{
						Selectable(config.Includes.Visual.selected_node_esp_type[i], &config.Includes.Visual.selected_node_esp[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						if (config.Includes.Visual.selected_node_esp[i])
							vec.push_back(config.Includes.Visual.selected_node_esp_type[i]);
					}
					for (size_t i = 0; i < vec.size(); i++)
					{
						if (vec.size() == 1)
							preview += vec.at(i);
						else if (!(i == vec.size() - 1))
							preview += vec.at(i) + ",";
						else
							preview += vec.at(i);
					}
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("ETC"), ImVec2(195, 143));
				{
					static std::string preview = "";
					preview = "";
					std::vector<std::string> vec;
					for (size_t i = 0; i < IM_ARRAYSIZE(config.Includes.Visual.selected_etc_esp_type); i++)
					{
						ImGui::Selectable(config.Includes.Visual.selected_etc_esp_type[i], &config.Includes.Visual.selected_etc_esp[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						if (config.Includes.Visual.selected_etc_esp[i])
							vec.push_back(config.Includes.Visual.selected_etc_esp_type[i]);
					}
					for (size_t i = 0; i < vec.size(); i++)
					{
						if (vec.size() == 1)
							preview += vec.at(i);
						else if (!(i == vec.size() - 1))
							preview += vec.at(i) + ",";
						else
							preview += vec.at(i);
					}
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("Animal"), ImVec2(195, 322));
				{
					static std::string preview = "";
					preview = "";
					std::vector<std::string> vec;
					for (size_t i = 0; i < IM_ARRAYSIZE(config.Includes.Visual.selected_Animal_esp_type); i++)
					{
						ImGui::Selectable(config.Includes.Visual.selected_Animal_esp_type[i], &config.Includes.Visual.selected_Animal_esp[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						if (config.Includes.Visual.selected_Animal_esp[i])
							vec.push_back(config.Includes.Visual.selected_Animal_esp_type[i]);
					}
					for (size_t i = 0; i < vec.size(); i++)
					{
						if (vec.size() == 1)
							preview += vec.at(i);
						else if (!(i == vec.size() - 1))
							preview += vec.at(i) + ",";
						else
							preview += vec.at(i);
					}
				}
				EndChild();
			}

			if (tab == 6)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("Aim"), ImVec2(195, 140));
				{
					Text("Aim Bind");
					BeginChild("AimBind", ImVec2(0,GetTextLineHeightWithSpacing() * 3.3), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Aimbot.aimbind);

						if (Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Aimbot.aimbind = i;
						}

						if (item_selected)
							SetItemDefaultFocus();
					}
					EndChild();
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("Movement"), ImVec2(195, 143));
				{
					Checkbox(e("Fly Hack Bar"), &config.Includes.Visual.fly_hack_bar);
					Text("Fly Bind");
					BeginChild("FlyBind", ImVec2(0,GetTextLineHeightWithSpacing() * 3.3), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Misc.flyhackKeybind);

						if (Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Misc.flyhackKeybind = i;
						}

						if (item_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndChild();
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				ImGui::BeginChild(e("Misc"), ImVec2(195, 322));
				{
					ImGui::Text("Zoom Bind");
					ImGui::BeginChild("ZoomBind", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3.3), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Misc.zoomKeybind);

						if (ImGui::Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Misc.zoomKeybind = i;
						}

						if (item_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndChild();

					ImGui::Text("Long Neck UP");
					ImGui::BeginChild("neckup", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 3.3), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Misc.updesncybind);

						if (ImGui::Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Misc.updesncybind = i;
						}

						if (item_selected)
							SetItemDefaultFocus();
					}
					EndChild();

					Text("Long Neck Right");
					BeginChild("neckupRight", ImVec2(0, GetTextLineHeightWithSpacing() * 3.3), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Misc.rightdesncybind);

						if (Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Misc.rightdesncybind = i;
						}

						if (item_selected)
							SetItemDefaultFocus();
					}
					EndChild();

					Text("Long Neck Left");
					BeginChild("neckupleft", ImVec2(0, GetTextLineHeightWithSpacing() * 3.1), true, ImGuiWindowFlags_HorizontalScrollbar);
					for (int i = 0; i < IM_ARRAYSIZE(config.Includes.Setiings.keys); i++)
					{
						const bool item_selected = (i == config.Includes.Misc.leftdesncybind);

						if (Selectable(config.Includes.Setiings.keys[i], item_selected))
						{
							config.Includes.Misc.leftdesncybind = i;
						}

						if (item_selected)
							SetItemDefaultFocus();
					}
					EndChild();
				}
				EndChild();
			}

			if (tab == 7)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("Optimize"), ImVec2(195, 140));
				{
					SetCursorPosX(5); SliderInt(e("Player Wait"), &config.Includes.Setiings.get_list, 750, 7500, "%.2f%%");
					SetCursorPosX(5); SliderInt(e("movement Wait"), &config.Includes.Setiings.movement_features, 1, 100, "%.2f%%");
					SetCursorPosX(5); SliderInt(e("weapon Wait"), &config.Includes.Setiings.weapon_features, 1, 100, "%.2f%%");
					SetCursorPosX(5); SliderInt(e("Menu Wait"), &config.Includes.Setiings.menu_sleep, 5, 100, "%.2f%%");
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("2 Optimize"), ImVec2(195, 143));
				{
					SetCursorPosX(5); Checkbox("Bad Pc Mode", &config.Includes.Setiings.badpcmode);
					SetCursorPosX(5); SliderInt(e("Aim Wait"), &config.Includes.Setiings.aim_sleep, 1, 100, "%.2f%%");
					SetCursorPosX(5); SliderInt(e("Misc Wait"), &config.Includes.Setiings.misc_features, 1, 100, "%.2f%%");
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("Other"), ImVec2(195, 322));
				{

				}
				EndChild();
			}

			if (tab == 8)
			{
				SetCursorPos(ImVec2(137, 55));
				BeginChild(e("rage"), ImVec2(195, 140));
				{
					constexpr auto& rage_config_items = config.get_configs();
					auto current_rage_config = 2;

					if ((size_t)(current_rage_config) >= rage_config_items.size())
						current_rage_config = 2;

					SetCursorPosX(5);  SetCursorPosY(5); if (Button("Load Rage"))
						config.load(current_rage_config);


					SetCursorPosX(5); if (Button("save Rage"))
						config.save(current_rage_config);
				}
				EndChild();

				SetCursorPos(ImVec2(137, 235));
				BeginChild(e("semi"), ImVec2(195, 143));
				{
					constexpr auto& semi_config_items = config.get_configs();
					auto current_semi_config = 1;

					if ((size_t)(current_semi_config) >= semi_config_items.size())
						current_semi_config = 1;

					SetCursorPosX(5);  SetCursorPosY(5); if (Button("Load Semi Legit"))
						config.load(current_semi_config);

					SetCursorPosX(5); if (Button("save Semi Legit"))
						config.save(current_semi_config);
				}
				EndChild();

				SetCursorPos(ImVec2(342, 55));
				BeginChild(e("legit"), ImVec2(195, 322));
				{
					constexpr auto& legit_config_items = config.get_configs();
					auto current_legit_config = 0;

					if ((size_t)(current_legit_config) >= legit_config_items.size())
						current_legit_config = 0;

					SetCursorPosX(5);  SetCursorPosY(5); if (Button("Load Legit"))
						config.load(current_legit_config);

					SetCursorPosX(5); if (Button("save Legit"))
						config.save(current_legit_config);
				}
				EndChild();
			}

			PopItemFlag();
			SetCursorPos({ 0, 0 });
	        BeginChild(e("navbar"), { 100 + 85 * navbar_width, window->Size.y }, 0, ImGuiWindowFlags_NoBackground);
			{
				ImVec2 childSize = GetContentRegionAvail();
				SetCursorPosY(5);
				ImVec2 MAIN = CalcTextSize("");
				SetCursorPosX(childSize.x / 2.0f - MAIN.x / 2.0f);TextDisabled("");
				if (Tab("Aim", tab == 0)) tab = 0;
				if (Tab("Weapon", tab == 1)) tab = 1;
				if (Tab("Misc", tab == 2)) tab = 2;
				if (Tab("Player Visuals", tab == 3)) tab = 3;
				if (Tab("NPC Visuals", tab == 4)) tab = 4;
				if (Tab("World Visuals", tab == 5)) tab = 5;
				if (Tab("Keys", tab == 6)) tab = 6;
				if (Tab("Optimize", tab == 7)) tab = 7;
				if (Tab("Settings", tab == 8)) tab = 8;
			}
			EndChild();
		}
		End();
	}
	ImFont* Font;
	auto create_window(std::function<void()> callback) -> bool
	{
		const auto window = FindWindowW(e(L"MedalOverlayClass"), e(L"MedalOverlay"));
		if (!window)
			return false;

		const auto window_hwnd = FindWindow(NULL, e("Rust"));
		if (!window_hwnd)
			return false;

		ShowWindow(window, SW_SHOW);
		UpdateWindow(window);

		if (!dx9::create_device(window, config.Includes.Setiings.width, config.Includes.Setiings.height))
			return false;

		CreateContext();
		ImGuiIO& io =GetIO();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(dx9::p_device);
		ImFontConfig font_cfg;
		Font2 = io.Fonts->AddFontFromMemoryTTF((void*)font, sizeof(font), 13.f, &font_cfg);
		dx9::g_d3d->Release();
		SetWindowPos(window, 0, 0, 0, config.Includes.Setiings.width, config.Includes.Setiings.height, 0);

		auto exstyle = GetWindowLongA(window, GWL_EXSTYLE);
		exstyle &= ~WS_EX_NOACTIVATE;
		SetWindowLongA(window, GWL_EXSTYLE, exstyle);

		auto msg = MSG{};
		while (true)
		{
			if (PeekMessage(&msg, 0, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			DWORD currentTime = GetTickCount();
			if (currentTime - lastKeyPressTime > 1000)
			{
				if (GetForegroundWindow() == window_hwnd)
				{
					SetWindowPos(window, GetWindow(window_hwnd, GW_HWNDPREV), 0, 0, config.Includes.Setiings.width, config.Includes.Setiings.height, SWP_NOMOVE | SWP_NOSIZE);
					lastKeyPressTime = currentTime;
				}
			}

			if (config.Includes.Setiings.showing)
			{
				POINT p;
				GetCursorPos(&p);
				io.MousePos.x = p.x;
				io.MousePos.y = p.y;

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					io.MouseDown[0] = true;
					io.MouseClicked[0] = true;
					io.MouseClickedPos[0].x = io.MousePos.x;
					io.MouseClickedPos[0].x = io.MousePos.y;
				}
				else io.MouseDown[0] = false;
			}
			static bool canPress = true;
			if (GetAsyncKeyState(VK_INSERT) & 0x8000)
			{			
				if (canPress) {
					config.Includes.Setiings.showing = !config.Includes.Setiings.showing;
					canPress = false;
				}
			}
			else canPress = true;

			dx9::begin_scene();
			//draw visuals - do prefabs here too
			{
				visuals::draw_loop();
			}
			auto foreground = ImGui::GetForegroundDrawList();
			auto background = ImGui::GetBackgroundDrawList();

			if (config.Includes.Setiings.showing) callback(); // menu rendering
			dx9::end_scene();
		}

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		DestroyContext();
		DestroyWindow(window);
	}
}