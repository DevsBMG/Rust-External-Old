#pragma once
#include "imports.hpp"

namespace aimbot
{
	namespace prediction
	{
		namespace definition
		{
			struct prediction_values
			{
				float ammovelocity = 300.0;
				float gravity = 1.0;
				float drag = 0.725;
			};

			enum ammo_id : int
			{
				//arrows
				arrow_wooden = 1,
				arrow_highvel = 2,
				arrow_bone = 3,
				arrow_fire = 4,

				//nailgun
				nailgun_nails = 9,

				//rifle_5.56
				rifle_normal = 5,
				rifle_highvel = 6,
				rifle_explo = 7,
				rifle_incen = 8,

				//pistol_9mm
				pistol_normal = 10,
				pistol_highvel = 11,
				pistol_incen = 12,

				//shotgun_12g
				shotgun_buck = 13,
				shotgun_handmade = 14,
				shotgun_incen = 15,
				shotgun_slug = 16,
			};
		}

		namespace stats
		{
			auto ammo_prediction(int ammo_id) -> definition::prediction_values
			{
				definition::prediction_values output;

				//arrows
				if (ammo_id == definition::ammo_id::arrow_wooden)
				{
					output.ammovelocity = 50.00f;
					output.drag = 0.005f;
					output.gravity = 0.75f;
				}
				else if (ammo_id == definition::ammo_id::arrow_highvel)
				{
					output.ammovelocity = 80.00f;
					output.drag = 0.005f;
					output.gravity = 0.50f;
				}
				else if (ammo_id == definition::ammo_id::arrow_bone)
				{
					output.ammovelocity = 45.00f;
					output.drag = 0.01f;
					output.gravity = 0.75f;
				}
				else if (ammo_id == definition::ammo_id::arrow_fire)
				{
					output.ammovelocity = 40.00f;
					output.drag = 0.01f;
					output.gravity = 1.00f;
				}

				//5.56
				if (ammo_id == definition::ammo_id::rifle_normal)
				{
					output.ammovelocity = 375.00f;
					output.drag = 0.60f;
					output.gravity = 1.00f;
				}
				else if (ammo_id == definition::ammo_id::rifle_highvel)
				{
					output.ammovelocity = 450.00f;
					output.drag = 0.60f;
					output.gravity = 1.00f;
				}
				else if (ammo_id == definition::ammo_id::rifle_explo)
				{
					output.ammovelocity = 225.00f;
					output.drag = 0.60f;
					output.gravity = 1.25f;
				}
				else if (ammo_id == definition::ammo_id::rifle_incen)
				{
					output.ammovelocity = 225.00f;
					output.drag = 0.60f;
					output.gravity = 1.00f;
				}

				//nails
				if (ammo_id == definition::ammo_id::nailgun_nails)
				{
					output.ammovelocity = 50.00f;
					output.drag = 0.005f;
					output.gravity = 0.75f;
				}

				//9mm
				if (ammo_id == definition::ammo_id::pistol_normal)
				{
					output.ammovelocity = 300.00f;
					output.drag = 0.70f;
					output.gravity = 1.00f;
				}
				else if (ammo_id == definition::ammo_id::pistol_highvel)
				{
					output.ammovelocity = 400.00f;
					output.drag = 0.70f;
					output.gravity = 1.00f;
				}
				else if (ammo_id == definition::ammo_id::pistol_incen)
				{
					output.ammovelocity = 225.00f;
					output.drag = 0.70f;
					output.gravity = 1.00f;
				}

				//12g
				if (ammo_id == definition::ammo_id::shotgun_buck)
				{
					output.ammovelocity = 225.00f;
					output.drag = 1.00f;
					output.gravity = 1.00f;
				}
				if (ammo_id == definition::ammo_id::shotgun_handmade)
				{
					output.ammovelocity = 100.00f;
					output.drag = 1.00f;
					output.gravity = 1.00f;
				}
				if (ammo_id == definition::ammo_id::shotgun_incen)
				{
					output.ammovelocity = 100.00f;
					output.drag = 1.00f;
					output.gravity = 1.00f;
				}
				else if (ammo_id == definition::ammo_id::shotgun_slug)
				{
					output.ammovelocity = 225.00f;
					output.drag = 0.60f;
					output.gravity = 1.00f;
				}

				return output;
			}

			auto ammo_type(int ammo_item_id) -> int
			{
				switch (ammo_item_id)
				{
				case -1211166256:
					return definition::ammo_id::rifle_normal;
					break;
				case 1712070256:
					return definition::ammo_id::rifle_highvel;
					break;
				case -1321651331:
					return definition::ammo_id::rifle_explo;
					break;
				case 605467368:
					return definition::ammo_id::rifle_incen;
					break;

				case 785728077:
					return definition::ammo_id::pistol_normal;
					break;
				case -1691396643:
					return definition::ammo_id::pistol_highvel;
					break;
				case 51984655:
					return definition::ammo_id::pistol_incen;
					break;

				case -2097376851:
					return definition::ammo_id::nailgun_nails;
					break;

				case -1234735557:
					return definition::ammo_id::arrow_wooden;
					break;
				case -1023065463:
					return definition::ammo_id::arrow_highvel;
					break;
				case 215754713:
					return definition::ammo_id::arrow_bone;
					break;
				case 14241751:
					return definition::ammo_id::arrow_fire;
					break;

				case 588596902:
					return definition::ammo_id::shotgun_handmade;
					break;
				case -1685290200:
					return definition::ammo_id::shotgun_buck;
					break;
				case -1036635990:
					return definition::ammo_id::shotgun_incen;
					break;
				case -727717969:
					return definition::ammo_id::shotgun_slug;
					break;
				}
			}
		}

		namespace helpers
		{
			struct
			{
				geo::vec3_t* velocities = new geo::vec3_t[32];
				int current_velocity_record = {};
				int current_velocity_index = {};
				geo::vec3_t velocity_angle = geo::vec3_t{ 0.f, 0.f, 0.f };
			} inline prediction_data;

			auto lerp(float default_val, float target, float alpha) -> float
			{
				return default_val + alpha * (target - default_val);
			}

			auto line_cirlce_intersection(geo::vec3_t center, float radius, geo::vec3_t raystart, geo::vec3_t rayend) -> bool
			{
				geo::vec2_t p(raystart.x, raystart.z);
				geo::vec2_t q(rayend.x, rayend.z);

				float a = q.y - p.y;
				float b = p.x - q.x;
				float c = (a * (p.x) + b * (p.y)) * -1.f;

				float x = center.x;
				float y = center.z;

				float c_x = (b * ((b * x) - (a * y)) - a * c) / (std::pow(a, 2) + std::pow(b, 2));
				float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (std::pow(a, 2) + std::pow(b, 2));

				geo::vec2_t closestpoint(c_x, c_y);

				float distance = p.distance(q);

				if (p.distance(closestpoint) > distance || q.distance(closestpoint) > distance)
					return false;

				if (radius > closestpoint.distance(geo::vec2_t(center.x, center.z)))
				{
					geo::vec2_t p(raystart.x, raystart.y);
					geo::vec2_t q(rayend.x, rayend.y);

					float a = q.y - p.y;
					float b = p.x - q.x;
					float c = (a * (p.x) + b * (p.y)) * -1.f;

					float x = center.x;
					float y = center.y;

					float c_x = (b * ((b * x) - (a * y)) - a * c) / (std::pow(a, 2) + std::pow(b, 2));
					float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (std::pow(a, 2) + std::pow(b, 2));

					geo::vec2_t closestpoint(c_x, c_y);
					if (radius > closestpoint.distance(geo::vec2_t(center.x, center.y))) return true;
					else return false;
				}

				return false;
			}

			auto get_lerp(geo::vec3_t center, geo::vec3_t firstposition, geo::vec3_t lastposition) -> float
			{
				geo::vec3_t center2d = center; geo::vec3_t first2d = firstposition; geo::vec3_t last2d = lastposition;
				center2d.y = 0.f; first2d.y = 0.f; last2d.y = 0.f;

				float distancefromfirst = (center2d - first2d).length();
				float distancefromlast = (center2d - last2d).length();

				float totaldistance = (last2d - first2d).length();

				if (totaldistance == 0)
					return 0.5;
				else
				{
					float percentfromfirst = distancefromfirst / totaldistance;
					float percentfromlast = distancefromlast / totaldistance;

					float lerpvalue = percentfromfirst / (percentfromfirst + percentfromlast);
					return lerpvalue;
				}
			}
			double lastTime;
			double currentTime;
			double deltaTime;
			geo::vec3_t targetCurrentPosition;
			geo::vec3_t targetPreviousPosition;
			auto simulate_movement(c_base_player* ply) -> geo::vec3_t
			{
				static bool set_data = true;

				static c_base_player* lastTarget;
				//if (set_data) {
					if (lastTarget != ply) {
						targetCurrentPosition = geo::vec3_t::zero();
						targetPreviousPosition = geo::vec3_t::zero();
						prediction_data.current_velocity_record = 0;
						prediction_data.current_velocity_index = 0;
						//prediction_data.velocity_angle = velocity;
					}

					//currentTime = systems::get_time_since_startup();
					//deltaTime = currentTime - lastTime;
					//lastTime = currentTime;
					//targetPreviousPosition = targetCurrentPosition;
					//targetCurrentPosition = ply->playerModel()->position();
					int records = 3;

					//auto velocity = c_base_player::Velocity(targetCurrentPosition, targetPreviousPosition, deltaTime);
					auto velocity = ply->playerModel()->newVelocity();

					if (velocity.is_empty())
					{
						velocity = ply->playerModel()->newVelocity();
						//prediction_data.current_velocity_record = 0;
						//prediction_data.current_velocity_index = 0;
						//prediction_data.velocity_angle = velocity;
					}


					prediction_data.velocities[prediction_data.current_velocity_index++] = velocity;
					if (prediction_data.current_velocity_index >= records)
						prediction_data.current_velocity_index = 0.0f;

					prediction_data.current_velocity_record++;
					if (prediction_data.current_velocity_record >= 0.0f)
						prediction_data.current_velocity_record = records;

					int current_records = prediction_data.current_velocity_record;

					geo::vec3_t velocity_sums{};
					for (int j = 0; j < current_records; j++)
						velocity_sums += prediction_data.velocities[j];

					prediction_data.velocity_angle = velocity_sums / (float)current_records;
				//}
				set_data = !set_data;
				lastTarget = ply;
				return prediction_data.velocity_angle;
			}

			auto get_trajectory(geo::vec3_t TargetPos, geo::vec3_t InitialPos, float Velocity, float Drag, float GravityMulti, float initialDistance, bool* Sims_finished, float* TravelTime)
			{
				bool SimulationFinished = false;
				int MaximumSims = 30;

				float PitchOffset = 0;
				geo::vec3_t TargetPostion = TargetPos;
				geo::vec3_t OutputAngle;

				for (int i = 0; i < MaximumSims; i++)
				{
					TargetPostion.y += PitchOffset;
					// lvAngle
					geo::vec3_t newAngle = TargetPostion - InitialPos;
					geo::vec3_t Angle = TargetPostion - InitialPos;

					geo::vec3_t CalcVelocity = Angle.normalized() * Velocity;

					geo::vec3_t ProjectilePosition = InitialPos;
					geo::vec3_t PreviousPosition = geo::vec3_t();

					const float StepSize = 0.03125f;
					for (float travelTime = 0; travelTime < 8; travelTime += StepSize)
					{
						PreviousPosition = ProjectilePosition;
						ProjectilePosition = ProjectilePosition + (CalcVelocity * StepSize);

						CalcVelocity.y -= 9.81f * GravityMulti * StepSize;
						CalcVelocity -= CalcVelocity * Drag * StepSize;

						float IntersectionPoint; // 0.01f
						if (line_cirlce_intersection(TargetPos, 0.05f, PreviousPosition, ProjectilePosition)) {
							SimulationFinished = true;

							if (travelTime > 0.1 && InitialPos.distance(ProjectilePosition) > initialDistance) {
								*TravelTime = travelTime - (((get_lerp(TargetPostion, ProjectilePosition, PreviousPosition) * StepSize)) + (initialDistance / Velocity));
							}
							break;
						}
						else if (InitialPos.distance(TargetPos) < InitialPos.distance(ProjectilePosition)) {
							geo::vec3_t CalculatedPosition = geo::vec3_t(TargetPos.x, lerp(ProjectilePosition.y, PreviousPosition.y, get_lerp(TargetPostion, ProjectilePosition, PreviousPosition)), TargetPos.z);
							PitchOffset = TargetPos.y - CalculatedPosition.y;
							break;
						}
					}

					if (SimulationFinished)
						break;
				}
				*Sims_finished = SimulationFinished;
				return TargetPostion;
			}
		}
	}
	auto aimbot_loop() -> void
	{
		c_base_player* target_player = nullptr;
		float dist;
		float lastFOV = 99999;

		for (auto& player : dark_aether.player_list)
		{
			if (!dark_aether.camera || !dark_aether.player) continue;

			const auto ply = player.cache.player;
			if (!ply) continue;

			const auto distance = player.cache.distance;
			if (!distance) continue;

			const auto player_model = ply->playerModel();
			if (!player_model) continue;

			const auto eyes = dark_aether.player->eyes();
			if (!eyes) continue;

			if (player_model->IsNpck__BackingField() && config.Includes.Aimbot.ignoreNPC) continue; // npc check

			//if ( ply->has_flag ( systems::player_flags::wounded ) ) continue; // wounded check
			if (ply->has_flag(systems::player_flags::safezone) && config.Includes.Aimbot.ignoreSafe) continue;
			//if ( !player.cache.visible ) continue; // shitty visual check

			if (player.cache.teammate) continue; // teammate check
			float curfov = dark_aether.camera->aim_fov(ply);
			if (curfov < lastFOV && curfov < config.Includes.Aimbot.Fov_slifer) {
				lastFOV = curfov;
				target_player = ply;
				dist = distance;
			}
		}

		if (target_player && dark_aether.camera->aim_fov(target_player) < config.Includes.Aimbot.Fov_slifer) {
			const auto projectile = dark_aether.player->get_projectile();
			if (!projectile) return;

			const auto wide_name = projectile->shortname();
			if (wide_name.empty()) return;

			if (!utils::is_weapon(wide_name)) return;

			const auto held = projectile->heldEntity();
			if (!held) return;
			auto stats = prediction::stats::ammo_prediction(prediction::stats::ammo_type(held->primaryMagazine()->ammo()->ammo_id()));
			auto body_rot = target_player->eyes()->bodyRotationk__BackingField();
			auto velocity_calculated = prediction::helpers::simulate_movement(target_player);
			//printf("total: %f\n", velocity_calculated.length());
			auto localplayerpos = dark_aether.player->get_bone_transform(systems::player_bones::eyeTranform)->get_position() + geo::quaternion_multi(target_player->eyes()->viewOffset(), &body_rot);
			if (config.Includes.Aimbot.selected_Aim)
			{
				int boner = 1;
				systems::player_bones target_boner;
				switch (config.Includes.Aimbot.selected_AimBone) {
				case 0:
					target_boner = systems::player_bones::head;
					break;
				case 1:
					target_boner = systems::player_bones::neck;
					break;
				case 2:
					target_boner = systems::player_bones::spine4;
					break;
				case 3:
					target_boner = systems::player_bones::pelvis;
					break;
				case 4:
					boner = 1 + rand() % 79;
					if (boner == 5 || boner == 8 || boner == 9 || boner == 10 || boner == 11 || boner == 12 || boner == 17 || boner == 19 || boner == 43 || boner == 44 || boner == 49 || boner == 74 || boner == 75 || boner >= 79) {
						boner = (int)systems::player_bones::spine2;
					}
					target_boner = (systems::player_bones)boner;
					break;
				case 5:
					boner = 1 + rand() % 79;
					if (boner == 5 || boner == 8 || boner == 9 || boner == 10 || boner == 11 || boner == 12 || boner == 17 || boner == 19 || boner == 43 || boner == 44 || boner == 49 || boner == 74 || boner == 75 || boner >= 79 || boner == 47 || boner == 48) {
						boner = (int)systems::player_bones::spine2;
					}
					target_boner = (systems::player_bones)boner;
				}

				auto rah = target_player->get_bone_transform(target_boner)->get_position();
				rah.y -= 0.01;
				if (rah.is_empty()) return;
				bool predicted;
				float travelTime;
				//geo::vec3_t AimPoint;
				auto aimbone = prediction::helpers::get_trajectory(rah, localplayerpos, stats.ammovelocity, stats.drag, stats.gravity, stats.ammovelocity * 0.050f, &predicted, &travelTime);
				if (predicted)
				{
					bool EndPredict = false;
					float firstTT = travelTime;
					geo::vec3_t tt_mod_vel = velocity_calculated * (travelTime-(0.03125f * 2));
					rah += tt_mod_vel;
					geo::vec3_t AimPos = aimbone;
					if (stats.ammovelocity * 0.050f < rah.distance(localplayerpos)) {
						for (float booger = 0.f; booger < firstTT; booger += 0.1) {
							AimPos = prediction::helpers::get_trajectory(rah, localplayerpos, stats.ammovelocity, stats.drag, stats.gravity, stats.ammovelocity * 0.050f, &EndPredict, &travelTime);
							AimPos += velocity_calculated * (travelTime-firstTT);
						}
					}
					//geo::vec3_t Direction = AimPos - localplayerpos;

					auto new_angles = geo::normalize(geo::calculate_angle(localplayerpos, AimPos));

					if (config.Includes.Aimbot.selected_Aim == 1)
					{
						auto player_input = dark_aether.player->input();
						if (player_input)
						{
							if (config.Includes.Aimbot.aimbind == 0 || GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Aimbot.aimbind])) player_input->set_bodyAngles(new_angles);
						}
					}
					else if (config.Includes.Aimbot.selected_Aim == 2)
					{
						if (config.Includes.Aimbot.aimbind == 0 || GetAsyncKeyState(config.Includes.Setiings.keybind[config.Includes.Aimbot.aimbind])) dark_aether.player->eyes()->set_bodyRotationk__BackingField(to_quaternion(new_angles));
					}
				}
			}
		}
	}
}