#pragma once
#include "imports.hpp"

class prefab_vector
{
public:
	prefab_vector(geo::vec3_t prefabpos, std::string prefabname, float dist, geo::vec4_t clr = geo::vec4_t{ 255.00f, 255.00f, 255.00f, 255.00f })
	{
		this->cache.position = prefabpos;
		this->cache.name = prefabname;
		this->cache.distance = dist;
		this->cache.color = clr;
	}
	struct
	{
		geo::vec3_t position = { 0.00f, 0.00f, 0.00f };
		std::string name;
		float distance = 0.00f;
		geo::vec4_t color = geo::vec4_t{ 255.00f, 255.00f, 255.00f, 255.00f };
	}cache;
};

class entity_vector
{
public:
	entity_vector(c_base_player* player, geo::vec3_t playerpos, geo::vec3_t localpos, float dist, bool is_team, bool is_visible)
	{
		this->cache.player = player;

		this->cache.player_pos = playerpos;
		this->cache.local_pos = localpos;

		this->cache.distance = dist;

		this->cache.teammate = is_team;
		this->cache.visible = is_visible;
	}
	struct
	{
		c_base_player* player = nullptr;

		geo::vec3_t player_pos = { 0.00f, 0.00f, 0.00f };
		geo::vec3_t local_pos = { 0.00f, 0.00f, 0.00f };

		float distance = 0.00f;

		bool teammate = false;
		bool visible = false;
	}cache;
};

class loco
{
public:
	c_camera* camera;
	c_tod_sky* tod_sky;
	c_skinnable* skinnable;
	c_convar_admin* convar_admin;
	c_convar_client* convar_client;
	c_convar_graphics* convar_graphics;

	c_base_player* player;

	std::vector< entity_vector> player_list;
	std::vector< prefab_vector> prefab_list;

	std::mutex cache_mutex;
}dark_aether;

namespace entity_loop
{
	c_base_player* list_player;
	auto get_list() -> void
	{

		dark_aether.camera = c_camera::instance();
		if ( !dark_aether.camera ) return;

		dark_aether.tod_sky = c_tod_sky::instance();
		if (!dark_aether.tod_sky) return;
		//printf("nigger");

		dark_aether.skinnable = c_skinnable::instance();
		if (!dark_aether.skinnable) return;

		dark_aether.player = c_base_player::instance();
		if (!dark_aether.player) return;

		dark_aether.convar_admin = c_convar_admin::instance();
		if (!dark_aether.convar_admin) return;

		dark_aether.convar_client = c_convar_client::instance();
		if (!dark_aether.convar_client) return;

		dark_aether.convar_graphics = c_convar_graphics::instance();
		if (!dark_aether.convar_graphics) return;

		const auto client_entities = c_base_networkable::get_base_entity()->get_client_entities();
		if (!client_entities) return;

		std::vector<entity_vector> base_player_list;
		std::vector<prefab_vector> base_prefab_list;
		
		for (std::uint32_t i = 0; i < client_entities->size(); i++)
		{
			const auto object = driver.read<c_object*>(client_entities->get(i) + 0x10);
			if (!object) continue;

			const auto object_class = object->object_class();
			if (!object_class) continue;

			const auto entity = object->get_instance();
			if (!entity) continue;

			auto object_name = object_class->get_name();

			auto object_position = object_class->get_position();

			auto local_bone = dark_aether.player->get_bone_transform(systems::player_bones::head)->get_position();
			if (object_class->tag() == 6)
			{
				if (!entity->is_valid(dark_aether.player)) continue; list_player = entity;

				auto player_bone = entity->get_bone_transform(systems::player_bones::head)->get_position();

				const auto distance = player_bone.distance(local_bone);
				if (distance > config.Includes.Visual.player_distance) continue; // max distance for esp - add to menu and config system
				{
					//const auto mat = driver.read_chain<std::uintptr_t>(driver.game_assembly, {Vars::m_TodSky_c, 0xB8  , 0x20});
					//const auto ply_model = entity->playerModel();
					//if (!ply_model) continue;

					//const auto skin = (ply_model->skinType() == 1 ? ply_model->FemaleSkin() : ply_model->MaleSkin());

					//const auto skin_set = skin->skinSet();
					//if (!skin_set) continue;

					//for (std::uint32_t i = 0; i < skin_set->size(); i++)
					//{
					//	const auto set = skin_set->get(i);
					//	if (!set) continue;

					//	set->set_eye_material(mat);
					//	set->set_body_material(mat);
					//	set->set_head_material(mat);
					//}

					//auto skinnables = dark_aether.skinnable->All();

					//for (std::uint32_t i = 0; i < skinnables->size(); i++)
					//{
					//	auto skin = skinnables->get(i);
					//	if (!skin) continue;

						//auto groups = skin->Groups();

					//	auto category = skin->Category();

					//	for (std::uint32_t i = 0; i < groups->size(); i++)
					//	{
					//		auto group = groups->get(i);
					//		if (!group) continue;

					//		if (category == systems::category::footwear || category == systems::category::mask || category == systems::category::gloves ||
					//			category == systems::category::hat || category == systems::category::jacket || category == systems::category::pants ||
					//			category == systems::category::shirt ||
					//			category == systems::category::weapon)
					//			group->material()->set_value(driver.read<std::uintptr_t>(mat + 0x10));
					//	}
					//}
					//entity->set_needsClothesRebuild(true);
				}

				base_player_list.push_back({ entity, player_bone, local_bone, distance, entity->is_teammate(dark_aether.player), entity->playerModel()->visible() });
			}
			{ //prefab esp
				auto distance = object_position.distance(local_bone);
				if (distance > config.Includes.Visual.etc_distance) continue; // max prefab distance
				if (config.Includes.Visual.selected_node_esp[0] == true) { if (strstr(object_name.c_str(), e("stone-ore.prefab"))) { base_prefab_list.push_back({ object_position, e("stone ore"), distance }); } }
				if (config.Includes.Visual.selected_node_esp[1] == true) { if (strstr(object_name.c_str(), e("sulfur-ore.prefab"))) { base_prefab_list.push_back({ object_position, e("sulfur ore"), distance, geo::vec4_t(255.00f, 255.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_node_esp[2] == true) { if (strstr(object_name.c_str(), e("metal-ore.prefab"))) { base_prefab_list.push_back({ object_position, e("metal ore"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_node_esp[3] == true) { if (strstr(object_name.c_str(), e("wood-collectable.prefab"))) { base_prefab_list.push_back({ object_position, e("wood-collectable"), distance }); } }
				if (config.Includes.Visual.selected_node_esp[4] == true) { if (strstr(object_name.c_str(), e("stone-collectable.prefab"))) { base_prefab_list.push_back({ object_position, e("stone-collectable"), distance }); } }
				if (config.Includes.Visual.selected_node_esp[5] == true) { if (strstr(object_name.c_str(), e("sulfur-collectable.prefab"))) { base_prefab_list.push_back({ object_position, e("sulfur-collectable"), distance, geo::vec4_t(255.00f, 255.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_node_esp[6] == true) { if (strstr(object_name.c_str(), e("metal-collectable.prefab"))) { base_prefab_list.push_back({ object_position, e("metal-collectable"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }

				if (config.Includes.Visual.selected_etc_esp[0] == true) { if (strstr(object_name.c_str(), e("hemp-collectable.prefab"))) { base_prefab_list.push_back({ object_position, e("hemp-collectable"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_etc_esp[1] == true) { if (strstr(object_name.c_str(), e("recycler"))) { base_prefab_list.push_back({ object_position, e("Recycler"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_etc_esp[2] == true) { if (strstr(object_name.c_str(), e("tugboat"))) { base_prefab_list.push_back({ object_position, e("Tug Boat"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }

				if (config.Includes.Visual.selected_Animal_esp[0] == true) { if (strstr(object_name.c_str(), e("bear"))) { base_prefab_list.push_back({ object_position, e("Bear"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_Animal_esp[1] == true) { if (strstr(object_name.c_str(), e("boar"))) { base_prefab_list.push_back({ object_position, e("Boar"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_Animal_esp[2] == true) { if (strstr(object_name.c_str(), e("chicken"))) { base_prefab_list.push_back({ object_position, e("Chicken"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
				if (config.Includes.Visual.selected_Animal_esp[3] == true) { if (strstr(object_name.c_str(), e("horse"))) { base_prefab_list.push_back({ object_position, e("Horse"), distance, geo::vec4_t(255.00f, 0.00f, 0.00f, 255.00f) }); } }
			}
		}

		dark_aether.cache_mutex.lock();

		dark_aether.player_list = base_player_list;
		dark_aether.prefab_list = base_prefab_list;

		dark_aether.cache_mutex.unlock();
	}
}