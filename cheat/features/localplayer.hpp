#pragma once
#include "imports.hpp"
namespace features
{
	namespace held
	{
		auto recoil_values(float n1, float n2, float n3, float n4, c_recoil_properties* recoil) -> void
		{
			if (config.Includes.Weapon.no_recoil)
			{
				recoil->set_recoilYawMin(config.Includes.Weapon.x_recoli * n1); // replace 0.00f for yaw
				recoil->set_recoilYawMax(config.Includes.Weapon.x_recoli * n1); // replace 0.00f for yaw
				recoil->set_recoilPitchMin(config.Includes.Weapon.y_recoli * n3); // replace 0.00f for pitch
				recoil->set_recoilPitchMax(config.Includes.Weapon.y_recoli * n4); // replace 0.00f for pitch
			}
		}

		auto no_recoil(std::wstring itemname, c_recoil_properties* recoil, c_recoil_properties* new_recoil) -> void
		{
			if (itemname == e(L"pistol.nailgun"))  recoil_values(-1.f, 1.f, -3.f, -6.f, recoil);
			else if (itemname == e(L"pistol.python")) recoil_values(-2.f, 2.f, -15.f, -16.f, recoil);
			else if (itemname == e(L"pistol.revolver")) recoil_values(-1.f, 1.f, -3.f, -6.f, recoil);
			else if (itemname == e(L"shotgun.pump")) recoil_values(4.f, 8.f, -10.f, -14.f, recoil);
			else if (itemname == e(L"shotgun.double")) recoil_values(8.f, 15.f, -10.f, -15.f, recoil);
			else if (itemname == e(L"shotgun.spas12")) recoil_values(4.f, 8.f, -10.f, -14.f, recoil);
			else if (itemname == e(L"shotgun.waterpipe")) recoil_values(4.f, 8.f, -10.f, -14.f, recoil);
			else if (itemname == e(L"crossbow")) recoil_values(-3.f, 3.f, -3.f, -6.f, recoil);
			else if (itemname == e(L"rifle.l96")) recoil_values(-2.f, 2.f, -1.f, -1.5f, recoil);
			else if (itemname == e(L"rifle.bolt")) recoil_values(-4.f, 4.f, -2.f, -3.f, recoil);
			else if (itemname == e(L"rifle.bolt")) recoil_values(-4.f, 4.f, -2.f, -3.f, recoil);
			else if (itemname == e(L"pistol.prototype17")) recoil_values(-1.f, 1.f, -2.f, -2.5f, recoil);
			{
				if (itemname == e(L"rifle.ak")) recoil_values(1.5f, 2.5f, -2.5f, -3.5f, new_recoil);
				else if (itemname == e(L"rifle.ak.ice")) recoil_values(1.5f, 2.5f, -2.5f, -3.5f, new_recoil);
				else if (itemname == e(L"rifle.lr300")) recoil_values(-1.f, 1.f, -2.f, -3.f, new_recoil);
				else if (itemname == e(L"lmg.m249")) recoil_values(1.25f, 2.25f, -3.f, -4.f, new_recoil);
				else if (itemname == e(L"rifle.m39")) recoil_values(1.5f, 2.5f, -3.f, -4.f, new_recoil);
				else if (itemname == e(L"rifle.semiauto")) recoil_values(-0.5f, 0.5f, -2.f, -3.f, new_recoil);
				else if (itemname == e(L"hmlmg")) recoil_values(-1.25f, -2.5f, -3.f, -4.f, new_recoil);
				else if (itemname == e(L"smg.mp5")) recoil_values(-1.f, 1.f, -1.f, -3.f, new_recoil);
				else if (itemname == e(L"smg.thompson")) recoil_values(-1.f, 1.f, -1.5f, -2.f, new_recoil);
				else if (itemname == e(L"smg.2")) recoil_values(-1.f, 1.f, -1.5f, -2.f, new_recoil);
				else if (itemname == e(L"pistol.m92")) recoil_values(-1.f, 1.f, -2.f, -2.5f, new_recoil);
				else if (itemname == e(L"pistol.semiauto")) recoil_values(-1.f, 1.f, -7.f, -8.f, new_recoil);
			}
		}

		auto weapon_repeat_delay(std::wstring weapon_name) -> float
		{
			if (weapon_name == e(L"rifle.ak")) return 0.1333f;
			else if (weapon_name == e(L"rifle.ak.ice")) return 0.1333f;
			else if (weapon_name == e(L"smg.2")) return 0.10f;
			else if (weapon_name == e(L"rifle.lr300")) return 0.12f;
			else if (weapon_name == e(L"lmg.m249")) return 0.12f;
			else if (weapon_name == e(L"rifle.m39")) return 0.20f;
			else if (weapon_name == e(L"pistol.m92")) return 0.15f;
			else if (weapon_name == e(L"smg.mp5")) return 0.10f;
			else if (weapon_name == e(L"pistol.nailgun")) return 0.15f;
			else if (weapon_name == e(L"pistol.revolver")) return 0.175f;
			else if (weapon_name == e(L"pistol.semiauto")) return 0.15f;
			else if (weapon_name == e(L"rifle.semiauto")) return 0.175f;
			else if (weapon_name == e(L"smg.thompson")) return 0.13f;
			else if (weapon_name == e(L"pistol.python")) return 0.15f;
			else if (weapon_name == e(L"hmlmg")) return 0.13f;
			else if (weapon_name == e(L"pistol.prototype17")) return 0.09f;
		}
	}

	auto weapon_features() -> void
	{
		if (dark_aether.camera && dark_aether.player && dark_aether.player->has_flag(systems::player_flags::connected) && dark_aether.player->lifestate() == systems::lifestate::alive)
		{
			const auto projectile = dark_aether.player->get_projectile();
			if (!projectile) return;

			const auto wide_name = projectile->shortname();
			if (wide_name.empty()) return;

			if (!utils::is_weapon(projectile->shortname())) return;

			const auto held = projectile->heldEntity();
			if (!held) return;

			//held->viewModel()->instance().
			//static int timerh = 0;
		//static int last_typeh = 0;
		//if (config.Includes.Visual.HandChams) {
		//	timerh++;
		//	//printf("timer: %d \n", timer);
		//	if (timerh > 50 || last_typeh != config.Includes.Visual.HandChamstype) {
		//		timerh = 0;
		//		last_typeh = config.Includes.Visual.chamstype;

		//		const auto mat = config.Includes.Visual.chamstype == 0 ? driver.read_chain<std::uintptr_t>(driver.game_assembly, { 54133368, 0xB8 , 0x208 }) : driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_TodSky_c, 0xB8, 0x20 }); //driver.read_chain<std::uintptr_t>(driver.game_assembly, { Vars::m_LaserBeam_c, 0xB8 , 0x190, 0x38, 0x40 })
		//		const auto ply_model = dark_aether.player->playerModel();
		//		if (!ply_model) continue;

		//		const auto skin = (ply_model->skinType() == 1 ? ply_model->FemaleSkin() : ply_model->MaleSkin());

		//		const auto skin_set = skin->skinSet();
		//		if (!skin_set) continue;

		//		for (std::uint32_t i = 0; i < skin_set->size(); i++)
		//		{
		//			const auto set = skin_set->get(i);
		//			if (!set) continue;
		//			set->set_eye_material((uintptr_t)mat);
		//			set->set_body_material((uintptr_t)mat);
		//			set->set_head_material((uintptr_t)mat);
		//		}

		//		auto skinnables = dark_aether.skinnable->All();

		//		for (std::uint32_t i = 0; i < skinnables->size(); i++)
		//		{
		//			auto skin = skinnables->get(i);
		//			if (!skin) continue;

		//			auto groups = skin->Groups();

		//			auto category = skin->Category();

		//			for (std::uint32_t i = 0; i < groups->size(); i++)
		//			{
		//				auto group = groups->get(i);
		//				if (!group) continue;

		//				if (category == systems::category::footwear || category == systems::category::mask || category == systems::category::gloves ||
		//					category == systems::category::hat || category == systems::category::jacket || category == systems::category::pants ||
		//					category == systems::category::shirt /*||
		//					category == systems::category::weapon*/)
		//					group->material()->set_value(driver.read<std::uintptr_t>((uintptr_t)mat + 0x10));
		//			}
		//		}
		//		ply->set_needsClothesRebuild(true);
		//	}
		//}
			//weapon features

			if (config.Includes.Weapon.instantbow)
			{   //instant bow
				if (wide_name.find(e(L"bow.compound")) != std::wstring::npos)
				{
					held->set_stringHoldDurationMax(0.00f);
					held->set_stringBonusDamage(1000000.00f);
					held->set_movementPenaltyRampUpTime(1000000.00f);
				}
				else if (wide_name.find(e(L"bow.hunting")) != std::wstring::npos)
				{
					held->set_attackReady(true);
					held->set_arrowBack(10.00f);
				}
			}

			if (config.Includes.Weapon.insteoka)
			{	//instant eoka
				if (wide_name.find(e(L"eoka")) != std::wstring::npos)
				{
					held->set_strikes(1);
					held->set_successFraction(1.00f);
					held->set__didSparkThisFrame(true);
				}
			}

			//if (true)
			//{	// shoot in debug 
			//	if (/*GetAsyncKeyState(0x42) <<<<<put a custom bind here &&*/ GetAsyncKeyState(VK_LBUTTON))
			//	{
			//		auto local_pos = dark_aether.player->playerModel()->position();

			//		auto camera_pos = dark_aether.camera->position();

			//		if (camera_pos > local_pos - geo::vec3_t(2.00f, 2.00f, 2.00f) && camera_pos < local_pos + geo::vec3_t(2.00f, 2.00f, 2.00f)) held->set_canChangeFireModes(false);
			//		else
			//		{
			//			held->set_currentBurst(1);
			//			held->set_isBurstWeapon(true);
			//		}
			//	}
			//	else
			//	{
			//		held->set_currentBurst(1);
			//		held->set_isBurstWeapon(false);
			//	}
			//}

			if (true)
			{	//no recoil
				auto recoil = held->recoil();
				auto new_recoil = recoil->newRecoilOverride();
				if (recoil || new_recoil) held::no_recoil(wide_name, recoil, new_recoil);
			}

			if (true)
			{	//not lower when omnisprint
				if (wide_name.find(e(L"bow.compound")) == std::wstring::npos
					&& wide_name.find(e(L"bow.hunting")) == std::wstring::npos
					&& wide_name.find(e(L"crossbow")) == std::wstring::npos
					&& wide_name.find(e(L"pistol.eoka")) == std::wstring::npos)
				{
					auto view_model = held->viewModel();
					auto instance = view_model->instance();
					if (view_model && instance) instance->get_lower()->set_should_lower(false);
				}
			}
			if (config.Includes.Weapon.Shoot_fast)
			{
				held->set_repeatDelay(0.05);
			}
			if (config.Includes.Weapon.no_animaion) {
				//*(uintptr_t*)(held->viewModel()->instance() + Classes::BaseViewModel::animator) = 0;
				auto vmi = held->viewModel()->instance();
				if (vmi) {
					vmi->get_bob()->set_bobanim(0);
					vmi->get_lower()->set_should_lower(0);
					vmi->get_punch()->set_punchanim(0);
				}
			}

			if (config.Includes.Weapon.no_spread)
			{	//no spread guns and shotguns
				held->set_aimCone(config.Includes.Weapon.no_spread_slider);//-5.00f
				held->set_hipAimCone(config.Includes.Weapon.no_spread_slider);//-5.00f
				held->set_stancePenalty(0.00f);//0.00f
				held->set_aimconePenalty(0.00f);//0.00f
				held->set_aimConePenaltyMax(0.00f);//0.00f
				held->set_stancePenaltyScale(0.00f);//0.00f
				held->set_aimconePenaltyPerShot(0.00f);//0.00f

				auto projectile_mod = [](std::uintptr_t each) -> void
				{
					auto created = reinterpret_cast<c_projectile*>(each);
					auto projectile_mod = created->mod();
					if (projectile_mod)
					{
						projectile_mod->set_projectileSpread(0.00f);
						projectile_mod->set_projectileVelocitySpread(0.00f);
						//driver.write((uintptr_t)created + Classes::Projectile::initialVelocity, geo::vec3_t(0, 0, 0));
						//driver.write((uintptr_t)created + Classes::Projectile::swimRandom, 0.f);
					}
				};

				held->created_projectiles(projectile_mod);
			}

			if (true)
			{	//2.50m thickbullet
				auto thickness = [](std::uintptr_t each) -> void
				{
					auto created = reinterpret_cast<c_projectile*>(each);
					if (created)
					{
						created->set_thickness(config.Includes.Weapon.Big_Bullet ? 0.65f : 0.1f);
					}
				};
				held->created_projectiles(thickness);
			}

			if (config.Includes.Weapon.auto_matic)
			{	//automatic
				held->set_automatic(true);
			}

			if (config.Includes.Weapon.no_sway)
			{	//nosway
				held->set_aimSway(config.Includes.Weapon.no_sway_slider);//0.00f
				held->set_aimSwaySpeed(config.Includes.Weapon.no_sway_slider);//0.00f
			}
		}
	}

	auto movement_features() -> void
	{
		if (dark_aether.camera && dark_aether.player && dark_aether.player->has_flag(systems::player_flags::connected) && dark_aether.player->lifestate() == systems::lifestate::alive)
		{
			const auto base_movement = dark_aether.player->movement();
			if (!base_movement) return;

			const auto model_state = dark_aether.player->modelState();
			if (!model_state) return;

			const auto eyes = dark_aether.player->eyes();
			if (!eyes) return;

			{
				if (GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Misc.zoomKeybind]))
				{
					dark_aether.convar_graphics->set_graphicsFov(config.Includes.Misc.zoom);
				}
				else 
				{
					dark_aether.convar_graphics->set_graphicsFov(config.Includes.Misc.fov_changer);
				}
			}
				if (GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Misc.flyhackKeybind]))
				{
					model_state->set_waterLevel(1.10f); // 0.988f
					auto rotation = eyes->bodyRotationk__BackingField();
					auto target_movement = geo::vec3_t(0.00f, 0.039f, 0.00f);

					auto speed = 4.00f;
					if (GetAsyncKeyState(0x57)) target_movement = geo::quaternion_multi(geo::vec3_t::get_forward(), &rotation);
					if (GetAsyncKeyState(0x53)) target_movement = geo::quaternion_multi(geo::vec3_t::get_backward(), &rotation);
					if (GetAsyncKeyState(0x44)) target_movement = geo::quaternion_multi(geo::vec3_t::get_right(), &rotation);
					if (GetAsyncKeyState(0x41)) target_movement = geo::quaternion_multi(geo::vec3_t::get_left(), &rotation);

					if (GetAsyncKeyState(VK_SPACE)) target_movement.y = 1.00f;
					if (GetAsyncKeyState(VK_LCONTROL)) target_movement.y - 1.00f;
					if (GetAsyncKeyState(VK_SHIFT))
					{
						speed = 10.00f;
					}

					base_movement->set_capsuleCenter(0.90f);
					base_movement->set_capsuleHeight(1.80f);
					//base_movement->set_capsuleCenter(-3000.00f);
					//base_movement->set_capsuleHeight(-3000.00f);

					base_movement->set_TargetMovementk__BackingField(target_movement * speed);
				}

			if (config.Includes.Misc.spider_man)
			{	//spiderman
				base_movement->set_groundAngleNew ( 0.f );
			}

			if (config.Includes.Misc.infinite_jump)
			{	//infinite jump
				if ( GetAsyncKeyState ( VK_SPACE ) )
				{
					base_movement->set_jumpTime ( 0.51f );
					base_movement->set_landTime ( 0.3f );
					base_movement->set_groundTime ( 99999.f );
				}
			}
			if (config.Includes.Misc.hold_item_while_mounted)
			{	//hold items while mounted
				auto mounted = dark_aether.player->mounted ( );
				if ( mounted )
				{
					if ( model_state->has_flag ( systems::model_flags::mounted ) ) mounted->set_canWieldItems ( true );
				}
			}
		}
	}

	auto misc_features() -> void
	{
		if (dark_aether.camera && dark_aether.player && dark_aether.player->has_flag(systems::player_flags::connected) && dark_aether.player->lifestate() == systems::lifestate::alive)
		{
			const auto base_movement = dark_aether.player->movement();
			if (!base_movement) return;

			const auto eyes = dark_aether.player->eyes();
			if (!eyes) return;

			//misc features

			if (GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Misc.updesncybind])) {
				eyes->set_viewOffset(geo::vec3_t(0.00f, 1.50f, 0.00f));
				dark_aether.player->set_clientTickInterval(0.99f);
			}// UP 
			else if (GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Misc.leftdesncybind])) {
				eyes->set_viewOffset(geo::vec3_t(-8.50f, 0.00f, 0.00f));
				dark_aether.player->set_clientTickInterval(0.99f);
			}// LEFT
			else if (GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Misc.rightdesncybind])) {
				eyes->set_viewOffset(geo::vec3_t(8.50f, 0.00f, 0.00f));
				dark_aether.player->set_clientTickInterval(0.99f);
			}// RIGHT
			else dark_aether.player->set_clientTickInterval(0.05f);
		}
	}

	auto fast_features() -> void
	{
		if (dark_aether.camera && dark_aether.player && dark_aether.player->has_flag(systems::player_flags::connected) && dark_aether.player->lifestate() == systems::lifestate::alive)
		{
			const auto base_movement = dark_aether.player->movement();
			if (!base_movement) return;

			//omnisprint and or spinbot

			if (config.Includes.Misc.omnis_print)
			{	//omnisprint
				base_movement->set_Runningk__BackingField(1.00f);
			}
		}
	}
}