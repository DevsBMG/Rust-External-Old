#pragma once
#include "imports.hpp"

auto flyhack_bar() -> void
{
	const auto grounded = dark_aether.player->movement()->grounded();
	auto position = dark_aether.player->playerModel()->position();
	if (position.is_empty()) return;
	static auto start = geo::vec3_t();
	if (!grounded && start.is_empty())
		start = position;
	else if (grounded)
	{
		start = geo::vec3_t(0.00f, 0.00f, 0.00f);
		config.Includes.Setiings.m_height = 0.00f;
		config.Includes.Setiings.m_direction = 0.00f;
	}
	else if (!start.is_empty())
	{
		config.Includes.Setiings.m_height = position.y - start.y;
		config.Includes.Setiings.m_direction = geo::vec2_t(position.x, position.z).distance({ start.x, start.z });
	}
}

auto get_rust_installation() -> std::string
{
	LONG   lResult;
	HKEY   hKey;
	char   value[64];
	DWORD  value_length = 64;
	DWORD  dwType = REG_SZ;

	lResult = RegOpenKey(HKEY_LOCAL_MACHINE, TEXT(e("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 252490")), &hKey);
	RegQueryValueExA(hKey, e("InstallLocation"), NULL, &dwType, (LPBYTE)&value, &value_length);

	RegCloseKey(hKey);

	return value;
}

auto texture_bundle_path = get_rust_installation() + e("\\Bundles\\items\\");

class c_texture_cache
{
private:
	std::unordered_map<std::string, IDirect3DTexture9*> texture_cache{};

	auto create_texture(LPDIRECT3DDEVICE9 device, std::string name) -> IDirect3DTexture9*
	{
		auto image_path = texture_bundle_path + name; // todo: make path dynamic
		auto* tex = LPDIRECT3DTEXTURE9();

		if (D3DXCreateTextureFromFileA(device, image_path.c_str(), &tex) != D3D_OK)
			return nullptr;

		return tex;
	}
public:
	auto get_texture(std::string item_name) -> IDirect3DTexture9*
	{
		if (texture_cache.find(item_name) != texture_cache.end())
			return texture_cache[item_name];

		auto texture = create_texture(dx9::p_device, item_name + e(".png"));
		if (!texture)
			return nullptr;

		texture_cache.emplace(item_name, texture);
		return texture;
	}

}; inline c_texture_cache texture_cache;


namespace visuals
{
	auto black_color = geo::vec4_t{ 0, 0, 0, 255 };
	auto main_color = geo::vec4_t{ 248, 214, 255, 255.00f };

	auto draw_loop() -> void
	{
		const auto gui = ImGui::GetBackgroundDrawList();
		geo::vec2_t size, bottom, top;
		const geo::vec2_t screen_center = { (float)config.Includes.Setiings.width / 2, (float)config.Includes.Setiings.height / 2 };

		if (config.Includes.Visual.fly_hack_bar)
		{
			flyhack_bar();
			config.Includes.Setiings.fly_height = config.Includes.Setiings.m_height * 52.f;
			config.Includes.Setiings.fly_direction = config.Includes.Setiings.m_direction * 52.f;
			if (config.Includes.Setiings.fly_height >= 255.f) config.Includes.Setiings.fly_height = 0.f;
			if (config.Includes.Setiings.fly_height <= 0.f) config.Includes.Setiings.fly_height = 0.f;
			if (config.Includes.Setiings.fly_direction >= 255.f) config.Includes.Setiings.fly_direction = 0.f;
			if (config.Includes.Setiings.fly_direction <= 0.f) config.Includes.Setiings.fly_direction = 0.f;

			if (config.Includes.Setiings.fly_height >= 1.f)
			{
				render::FilledRect(config.Includes.Setiings.width / 2, config.Includes.Setiings.height / 7.50, 250, 5, ImColor(56, 55, 55));
				render::FilledRect(config.Includes.Setiings.width / 2, config.Includes.Setiings.height / 7.50, config.Includes.Setiings.fly_height, 5, ImColor(0.00f, 160.00f, 255.00f, 255.00f));
			}

			if (config.Includes.Setiings.fly_direction >= 1.f)
			{
				render::FilledRect(config.Includes.Setiings.width / 2, config.Includes.Setiings.height / 8.00, 250, 5, ImColor(56, 55, 55));
				render::FilledRect(config.Includes.Setiings.width / 2, config.Includes.Setiings.height / 8.00, config.Includes.Setiings.fly_direction, 5, ImColor(0.00f, 160.00f, 255.00f, 255.00f));
			}
		}
		if (config.Includes.Aimbot.Fov_slifer > 0 && config.Includes.Aimbot.showfov){// fov cercle
			gui->AddCircle({ ((float)config.Includes.Setiings.width / 2.f) - 0.5f, ((float)config.Includes.Setiings.height / 2.f) - 0.5f }, config.Includes.Aimbot.Fov_slifer, ImColor(255, 255, 255));
		}

		if (config.Includes.Aimbot.crosshair) {
			render::FilledRect((config.Includes.Setiings.width / 2) - 2.5, (config.Includes.Setiings.height / 2) - 2.5, 5, 5, ImColor(5, 5, 5, 245));
			render::FilledRect((config.Includes.Setiings.width / 2) - 1.5, (config.Includes.Setiings.height / 2) - 1.5, 3, 3, ImColor(255, 255, 255, 255));
		}

		const auto projectile = dark_aether.player->get_projectile();
		if (projectile)
		{
			if (utils::is_weapon(projectile->shortname()))
			{
				const auto held = projectile->heldEntity();
				if (held)
				{
					if (config.Includes.Weapon.Bullet_Tracer)
					{	//draw bullet tracers
						geo::vec2_t hit_point_2d, local_player_2d;
						for (int i = 0; i < geo::bullet_tracer.size(); i++)
						{
							auto local_pos = geo::bullet_tracer.at(i).local_pos;
							auto bullet_pos = geo::bullet_tracer.at(i).bullet_pos;
							if (dark_aether.camera->world_to_screen(bullet_pos, &hit_point_2d)
								&& dark_aether.camera->world_to_screen(local_pos, &local_player_2d)
								&& std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - geo::bullet_tracer.at(i).shot_at) <= std::chrono::seconds(2))
							{
								if (local_pos.distance(bullet_pos) < 4.00f) continue;

								render::draw_line(hit_point_2d, local_player_2d, geo::vec4_t(255.00f, 255.00f, 255.00f, 255.00f), 1.50f);//0, 160, 255
							}
						}
					}

					{	//reload indicator
						if (held->isReloading() && geo::reload_indicator.m_reload_time > 0.00f && geo::reload_indicator.m_reload_duration > 0.00f)
						{
							const auto current = geo::reload_indicator.m_reload_duration - geo::reload_indicator.m_reload_time;
							const auto max = geo::reload_indicator.m_reload_duration;
							const auto color = render::quick::get_health_color(current / max);

							geo::vec2_t size = { 95.00f, 6.00f };
							geo::vec2_t position = { screen_center.x, screen_center.y + 16.00f };

							gui->AddRectFilled // background
							(
								{ position.x - (size.x / 2.00f), position.y },
								{ position.x - (size.x / 2.00f) + size.x, position.y + size.y + 1.00f },
								ImColor(0, 0, 0, (int)255.00f),
								0.00f
							);

							gui->AddRectFilled // colored
							(
								{ position.x - ((size.x - 1.00f) / 2.00f), position.y + 1.00f },
								{ position.x - ((size.x - 1.00f) / 2.00f) + utils::bound((current * (size.x - 2.00f) / max) + 1.00f, 0.00f, (size.x - 2.00f)), position.y + size.y },
								ImColor((int)color.x, (int)color.y, (int)color.z, (int)255.00f),
								0.00f
							);
							render::add_text({ screen_center.x - 3.00f, screen_center.y + 12.00f }, geo::vec4_t{ 255, 255, 255, 255 }, std::to_string(static_cast<std::int32_t>(geo::reload_indicator.m_reload_time / 1000.00f)));
						}
					}
				}
			}
		}

		for (auto& player : dark_aether.player_list)
		{
			if (!dark_aether.camera || !dark_aether.player) continue;

			auto ply = player.cache.player;
			if (!ply) continue;

			if (ply->has_flag(systems::player_flags::safezone) && !config.Includes.Visual.showSafe) continue;
			
			auto distance = player.cache.distance;
			if (!distance) continue;

			auto player_model = ply->playerModel();
			if (!player_model) continue;

			auto player_eye = ply->get_bone_transform(systems::player_bones::eyeTranform);
			if (!player_eye) continue;

			auto position = player_model->position();
			if (position.is_empty()) continue;

			auto position_eye = player_eye->get_position();
			if (position_eye.is_empty()) continue;

			float height = ply->modelState()->has_flag(systems::model_flags::crawling) ? 0.8 : ply->modelState()->has_flag(systems::model_flags::ducked) ? 1.35f : 1.81f;
			if (!dark_aether.camera->world_to_screen(position + geo::vec3_t{0.f, height, 0.f}, &top) || !dark_aether.camera->world_to_screen(position - geo::vec3_t{ 0.f, height / 10.f, 0.f }, &bottom)) continue;

			size = { (bottom.y - top.y) / (float)1.8f, bottom.y - top.y };
			if (size.is_empty()) continue;

			if (config.Includes.Visual.showPlayer && !ply->playerModel()->IsNpck__BackingField()) {
				if (config.Includes.Visual.health_esp)
				{
					//healthesp
					const auto health = ply->_health();
					const auto max_health = float(ply->playerModel()->IsNpck__BackingField() ? 150.00f : 100.00f);
					const auto color = render::quick::get_health_color(health / max_health);


					geo::vec2_t a = { (top.x + size.x / 2.f) + 4, top.y - 1.00f };
					geo::vec2_t b = { (bottom.x + size.x / 2.f) + 4, bottom.y };
					geo::vec2_t c = { (top.x + size.x / 2.f) + 4, top.y + (size.y - (size.y * float(health / max_health))) - 1.00f };
					float ingus = abs(b.y - c.y);
					geo::vec2_t booger = moveTowards_2d(b, a, ingus);
					render::draw_line(a, b, { black_color.x, black_color.y, black_color.x, black_color.w }, 3.0f, false);
					render::draw_line(booger, b, { color.x, color.y, color.z, 255.00f }, 2.0f, false);

				}
				//
				float text_padding = 0.00f;
				if (config.Includes.Visual.belt_esp)
				{
					auto belt = ply->inventory()->get_belt()->item_list();
					if (!belt) continue;

					auto belt_size = belt->size();

					for (int i = 0; i < belt_size; i++)
					{
						auto weapon = belt->get_item(i);
						if (!weapon) continue;

						text_padding = 37.f;

						auto texture = texture_cache.get_texture(weapon->get_name());

						constexpr float padding = 25.00f;

						if (weapon->uid() == ply->clActiveItem()) gui->AddRect({ bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 }, ImColor(0, 255, 0, 255));
						{
							gui->AddRectFilled({ bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 }, ImColor(158, 156, 152, 80));
							gui->AddImage(texture, { bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 });
						}

						render::add_text({ bottom.x + 1.00f + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 1 }, geo::vec4_t{ 255.00f, 0.00f, 0.00f, 255.00f }, std::to_string(weapon->amount()) + "x");
					}
				}
				if (config.Includes.Visual.name_esp)
				{
					if (ply->has_flag(systems::player_flags::wounded))
					{
						auto display_name = ply->_displayName()->buffer();
						auto name = std::string(display_name.begin(), display_name.end());
						if (name.empty()) continue;
						render::add_text({ top.x - (ImGui::CalcTextSize(name.c_str()).x / 2), top.y - 14 }, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f), name.c_str());
					}
					else
					{
						auto display_name = ply->_displayName()->buffer();
						auto name = std::string(display_name.begin(), display_name.end());
						if (name.empty()) continue;
						render::add_text({ top.x - (ImGui::CalcTextSize(name.c_str()).x / 2), top.y - 14 }, main_color, name.c_str());
					}
				}

				if (config.Includes.Visual.distance_esp)
				{
					auto distance_string = std::to_string((int)distance) + e("m");
					if (distance_string.empty()) continue;
					render::add_text({ bottom.x - (ImGui::CalcTextSize(distance_string.c_str()).x / 2), bottom.y + text_padding }, main_color, distance_string.c_str());
					text_padding += 10;
				}

				if (config.Includes.Visual.held_item_esp)
				{
					auto buffer = ply->get_projectile()->shortname();
					auto weapon_string = std::string(buffer.begin(), buffer.end());
					if (weapon_string.empty()) continue;
					render::add_text({ bottom.x - (ImGui::CalcTextSize(weapon_string.c_str()).x / 2.1f), bottom.y + text_padding }, main_color, weapon_string.c_str());

				}

				if (config.Includes.Visual.box_esp)
				{//box
					geo::vec2_t a = { (top.x + size.x / 2.3f), top.y - 1.00f };
					geo::vec2_t b = { (bottom.x + size.x / 2.3f), bottom.y };
					//float ingus = abs(b.y - c.y);
					//geo::vec2_t booger = moveTowards_2d(b, a, ingus);
					render::draw_line(a, b, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(a - geo::vec2_t{ size.x, 0 }, b - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(a, a - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(b, b - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);

					//gui->AddRect({ top.x - size.y / 4.00f, top.y }, { bottom.x + size.y / 4.00f, bottom.y + 3.00f }, ImColor{ black_color.x, black_color.y, black_color.x, black_color.w });
					//gui->AddRect({ top.x - size.y / 4.00f, top.y }, { bottom.x + size.y / 4.00f, bottom.y + 3.00f }, ImColor{ main_color.x, main_color.y, main_color.z, main_color.w }, 0, 0, 0.7);
				}

				if (config.Includes.Visual.skelly_esp && size.y > 5)
				{//skeleton
					auto head = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::head)->get_position());
					if (head.is_empty()) continue;

					auto neck = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::neck)->get_position());
					if (neck.is_empty()) continue;

					//auto r_clavicle = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_clavicle)->get_position());
					//if (r_clavicle.is_empty()) continue;

					auto r_upperarm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_upperarm)->get_position());
					if (r_upperarm.is_empty()) continue;

					auto r_hand = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_hand)->get_position());
					if (r_hand.is_empty()) continue;

					//auto l_clavicle = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_clavicle)->get_position());
					//if (l_clavicle.is_empty()) continue;

					auto l_upperarm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_upperarm)->get_position());
					if (l_upperarm.is_empty()) continue;

					auto l_forearm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_forearm)->get_position());
					if (l_forearm.is_empty()) continue;

					auto r_forearm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_forearm)->get_position());
					if (r_forearm.is_empty()) continue;

					auto l_hand = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_hand)->get_position());
					if (l_hand.is_empty()) continue;

					auto spine = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::spine1)->get_position());
					if (spine.is_empty()) continue;

					auto r_hip = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_hip)->get_position());
					if (r_hip.is_empty()) continue;

					auto r_knee = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_knee)->get_position());
					if (r_knee.is_empty()) continue;

					auto r_foot = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_foot)->get_position());
					if (r_foot.is_empty()) continue;

					auto l_hip = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_hip)->get_position());
					if (l_hip.is_empty()) continue;

					auto l_knee = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_knee)->get_position());
					if (l_knee.is_empty()) continue;

					auto l_foot = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_foot)->get_position());
					if (l_foot.is_empty()) continue;

					render::draw_line(head, neck, main_color, 1, false); // head

					render::draw_line(neck, r_upperarm, main_color, 1, false); render::draw_line(neck, l_upperarm, main_color, 1, false);//shoulders

					render::draw_line(r_upperarm, r_forearm, main_color, 1, false); render::draw_line(l_upperarm, l_forearm, main_color, 1, false);//bicep

					render::draw_line(r_forearm, r_hand, main_color, 1, false); render::draw_line(l_forearm, l_hand, main_color, 1, false);//forearm

					render::draw_line(neck, spine, main_color, 1, false);//back

					render::draw_line(spine, r_hip, main_color, 1, false);	render::draw_line(spine, l_hip, main_color, 1, false);//hip flexors

					render::draw_line(r_hip, r_knee, main_color, 1, false); render::draw_line(l_hip, l_knee, main_color, 1, false);//thigh

					render::draw_line(r_knee, r_foot, main_color, 1, false); render::draw_line(l_knee, l_foot, main_color, 1, false);//shin

				}
			}

			if (config.Includes.Visual.showNPC && ply->playerModel()->IsNpck__BackingField()) {
				if (config.Includes.Visual.NPChealth_esp)
				{
					//healthesp
					const auto health = ply->_health();
					const auto max_health = float(ply->playerModel()->IsNpck__BackingField() ? 150.00f : 100.00f);
					const auto color = render::quick::get_health_color(health / max_health);


					geo::vec2_t a = { (top.x + size.x / 2.f) + 4, top.y - 1.00f };
					geo::vec2_t b = { (bottom.x + size.x / 2.f) + 4, bottom.y };
					geo::vec2_t c = { (top.x + size.x / 2.f) + 4, top.y + (size.y - (size.y * float(health / max_health))) - 1.00f };
					float ingus = abs(b.y - c.y);
					geo::vec2_t booger = moveTowards_2d(b, a, ingus);
					render::draw_line(a, b, { black_color.x, black_color.y, black_color.x, black_color.w }, 3.0f, false);
					render::draw_line(booger, b, { color.x, color.y, color.z, 255.00f }, 2.0f, false);

				}
				//
				float text_padding = 0.00f;
				if (config.Includes.Visual.NPCbelt_esp)
				{
					auto belt = ply->inventory()->get_belt()->item_list();
					if (!belt) continue;

					auto belt_size = belt->size();

					for (int i = 0; i < belt_size; i++)
					{
						auto weapon = belt->get_item(i);
						if (!weapon) continue;

						text_padding = 37.f;

						auto texture = texture_cache.get_texture(weapon->get_name());

						constexpr float padding = 25.00f;

						if (weapon->uid() == ply->clActiveItem()) gui->AddRect({ bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 }, ImColor(0, 255, 0, 255));
						{
							gui->AddRectFilled({ bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 }, ImColor(158, 156, 152, 80));
							gui->AddImage(texture, { bottom.x + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + 7.00f }, { bottom.x + padding + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 2 });
						}

						render::add_text({ bottom.x + 1.00f + (i * padding) - ((belt_size * padding) / 2.00f), bottom.y + padding + 1 }, geo::vec4_t{ 255.00f, 0.00f, 0.00f, 255.00f }, std::to_string(weapon->amount()) + "x");
					}
				}
				if (config.Includes.Visual.NPCname_esp)
				{
					if (ply->has_flag(systems::player_flags::wounded))
					{
						auto display_name = ply->_displayName()->buffer();
						auto name = std::string(display_name.begin(), display_name.end());
						if (name.empty()) continue;
						render::add_text({ top.x - (ImGui::CalcTextSize(name.c_str()).x / 2), top.y - 14 }, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f), name.c_str());
					}
					else
					{
						auto display_name = ply->_displayName()->buffer();
						auto name = std::string(display_name.begin(), display_name.end());
						if (name.empty()) continue;
						render::add_text({ top.x - (ImGui::CalcTextSize(name.c_str()).x / 2), top.y - 14 }, main_color, name.c_str());
					}
				}

				if (config.Includes.Visual.NPCdistance_esp)
				{
					auto distance_string = std::to_string((int)distance) + e("m");
					if (distance_string.empty()) continue;
					render::add_text({ bottom.x - (ImGui::CalcTextSize(distance_string.c_str()).x / 2), bottom.y + text_padding }, main_color, distance_string.c_str());
					text_padding += 10;
				}

				if (config.Includes.Visual.NPCheld_item_esp)
				{
					auto buffer = ply->get_projectile()->shortname();
					auto weapon_string = std::string(buffer.begin(), buffer.end());
					if (weapon_string.empty()) continue;
					render::add_text({ bottom.x - (ImGui::CalcTextSize(weapon_string.c_str()).x / 2.1f), bottom.y + text_padding }, main_color, weapon_string.c_str());

				}

				if (config.Includes.Visual.NPCbox_esp)
				{//box
					geo::vec2_t a = { (top.x + size.x / 2.3f), top.y - 1.00f };
					geo::vec2_t b = { (bottom.x + size.x / 2.3f), bottom.y };
					//float ingus = abs(b.y - c.y);
					//geo::vec2_t booger = moveTowards_2d(b, a, ingus);
					render::draw_line(a, b, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(a - geo::vec2_t{ size.x, 0 }, b - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(a, a - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);
					render::draw_line(b, b - geo::vec2_t{ size.x, 0 }, geo::vec4_t{ main_color.x, main_color.y, main_color.z, main_color.w }, 1.0f);

					//gui->AddRect({ top.x - size.y / 4.00f, top.y }, { bottom.x + size.y / 4.00f, bottom.y + 3.00f }, ImColor{ black_color.x, black_color.y, black_color.x, black_color.w });
					//gui->AddRect({ top.x - size.y / 4.00f, top.y }, { bottom.x + size.y / 4.00f, bottom.y + 3.00f }, ImColor{ main_color.x, main_color.y, main_color.z, main_color.w }, 0, 0, 0.7);
				}
				if (config.Includes.Visual.NPCskelly_esp && size.y > 5)
				{//skeleton
					auto head = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::head)->get_position());
					if (head.is_empty()) continue;

					auto neck = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::neck)->get_position());
					if (neck.is_empty()) continue;

					//auto r_clavicle = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_clavicle)->get_position());
					//if (r_clavicle.is_empty()) continue;

					auto r_upperarm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_upperarm)->get_position());
					if (r_upperarm.is_empty()) continue;

					auto r_hand = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_hand)->get_position());
					if (r_hand.is_empty()) continue;

					//auto l_clavicle = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_clavicle)->get_position());
					//if (l_clavicle.is_empty()) continue;

					auto l_upperarm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_upperarm)->get_position());
					if (l_upperarm.is_empty()) continue;

					auto l_forearm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_forearm)->get_position());
					if (l_forearm.is_empty()) continue;

					auto r_forearm = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_forearm)->get_position());
					if (r_forearm.is_empty()) continue;

					auto l_hand = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_hand)->get_position());
					if (l_hand.is_empty()) continue;

					auto spine = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::spine1)->get_position());
					if (spine.is_empty()) continue;

					auto r_hip = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_hip)->get_position());
					if (r_hip.is_empty()) continue;

					auto r_knee = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_knee)->get_position());
					if (r_knee.is_empty()) continue;

					auto r_foot = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::r_foot)->get_position());
					if (r_foot.is_empty()) continue;

					auto l_hip = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_hip)->get_position());
					if (l_hip.is_empty()) continue;

					auto l_knee = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_knee)->get_position());
					if (l_knee.is_empty()) continue;

					auto l_foot = dark_aether.camera->world_to_screen_point(ply->get_bone_transform(systems::player_bones::l_foot)->get_position());
					if (l_foot.is_empty()) continue;

					render::draw_line(head, neck, main_color, 1, false); // head

					render::draw_line(neck, r_upperarm, main_color, 1, false); render::draw_line(neck, l_upperarm, main_color, 1, false);//shoulders

					render::draw_line(r_upperarm, r_forearm, main_color, 1, false); render::draw_line(l_upperarm, l_forearm, main_color, 1, false);//bicep

					render::draw_line(r_forearm, r_hand, main_color, 1, false); render::draw_line(l_forearm, l_hand, main_color, 1, false);//forearm

					render::draw_line(neck, spine, main_color, 1, false);//back

					render::draw_line(spine, r_hip, main_color, 1, false);	render::draw_line(spine, l_hip, main_color, 1, false);//hip flexors

					render::draw_line(r_hip, r_knee, main_color, 1, false); render::draw_line(l_hip, l_knee, main_color, 1, false);//thigh

					render::draw_line(r_knee, r_foot, main_color, 1, false); render::draw_line(l_knee, l_foot, main_color, 1, false);//shin

				}
			}
			//if (config.Includes.Visual.text_belt_esp)
			//{
			//	if (dark_aether.camera->aim_fov(ply) < config.Includes.Aimbot.Fov_slifer)
			//	{
			//		DWORD  window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
			//		ImGui::SetNextWindowSize({ 230, 200 });
			//		ImGui::Begin("minimenu", 0, window_flags);
			//		{
			//			ImGui::SetCursorPosX(0);
			//			ImGui::BeginChild("mini-menu", { 230, 225 });
			//			{
			//				auto belt = ply->inventory()->get_belt()->item_list();
			//				if (!belt) continue;
			//				auto belt_size = belt->size();
			//				if (!belt_size) continue;
			//				for (int i = 0; i < belt_size; i++)
			//				{
			//					auto weapon = belt->get_item(i);
			//					if (!weapon) continue;
			//					auto texture = texture_cache.get_texture(weapon->get_name());
			//					constexpr float padding = 36.00f;
			//					if (weapon->uid() == ply->clActiveItem()) gui->AddRect({ top.x + (i * padding) - ((belt_size * padding) / 2.00f) - 1.00f, top.y - 46.00f }, { top.x + 40.00f + (i * padding) - ((belt_size * padding) / 2.00f) + 2.00f, top.y - 5.00f }, ImColor(0, 255, 0, 255));
			//					{
			//						ImGui::SameLine();
			//						ImGui::Image(texture, ImVec2(25, 25));
			//						ImGui::SameLine();
			//						ImGui::Spacing();
			//					}
			//				}
			//			}
			//			ImGui::EndChild();
			//		}
			//	}
			//}
			static int timer = 0;
			static int last_type = 0;
			if (config.Includes.Visual.chams) {
				timer++;
				//printf("timer: %d \n", timer);
				if (timer > 50 || last_type != config.Includes.Visual.chamstype) {
					timer = 0;
					last_type = config.Includes.Visual.chamstype;

					DWORD64 mat = NULL;
					switch (config.Includes.Visual.chamstype) {
					case 0:
						mat = NULL;//null chams
						break;
					case 1:
						mat = driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_OutlineManager_c, 0x1a0, 0xc8, 0x88 });//white
						break;
					case 2:
						mat = driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_TodSky_c, 0xB8, 0x20 });//
						break;
					case 3:
						mat = driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_FlashBangOverlay_c, 0xB8 , 0x160 });//
						break;
					case 4:
						mat = driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_OutlineManager_c, 0xB8 , 0x1c8, 0x2f8, 0xf0 });//
					default:
						mat = NULL;//null chams
						break;
					}
					const auto ply_model = ply->playerModel();
					if (!ply_model) continue;

					const auto skin = (ply_model->skinType() == 1 ? ply_model->FemaleSkin() : ply_model->MaleSkin());

					const auto skin_set = skin->skinSet();
					if (!skin_set) continue;

					for (std::uint32_t i = 0; i < skin_set->size(); i++)
					{
						const auto set = skin_set->get(i);
						if (!set) continue;

						set->set_eye_material((uintptr_t)mat);
						set->set_body_material((uintptr_t)mat);
						set->set_head_material((uintptr_t)mat);
					}

					auto skinnables = dark_aether.skinnable->All();

					for (std::uint32_t i = 0; i < skinnables->size(); i++)
					{
						auto skin = skinnables->get(i);
						if (!skin) continue;

						auto groups = skin->Groups();

						auto category = skin->Category();

						for (std::uint32_t i = 0; i < groups->size(); i++)
						{
							auto group = groups->get(i);
							if (!group) continue;

							if (category == systems::category::footwear || category == systems::category::mask || category == systems::category::gloves ||
								category == systems::category::hat || category == systems::category::jacket || category == systems::category::pants ||
								category == systems::category::shirt)
								group->material()->set_value(driver.read<std::uintptr_t>((uintptr_t)mat + 0x10));
						}
					}
					ply->set_needsClothesRebuild(true);
				}
			}
		}

		for (auto& prefablist : dark_aether.prefab_list)
		{
			auto pos = prefablist.cache.position;
			if (pos.is_empty()) continue;
			auto distance = prefablist.cache.distance;
			if (!distance) continue;
			auto name = prefablist.cache.name;

			if (!dark_aether.camera->world_to_screen({ pos.x, pos.y + 1.75f, pos.z }, &top) || !dark_aether.camera->world_to_screen({ pos.x, pos.y - 0.25f, pos.z }, &bottom)) continue;
			size = { (bottom.y - top.y) / (float)1.8f, bottom.y - top.y };
			if (size.is_empty()) continue;
			gui->AddText({ top.x - (ImGui::CalcTextSize(name.c_str()).x / 2), top.y - 16 }, ImColor(255, 255, 255), name.c_str());
		}
	}
}