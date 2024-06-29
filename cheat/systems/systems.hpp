#pragma once
#include "imports.hpp"

namespace systems
{
	struct gaysex
	{
		geo::vec3_t uno;
		geo::vec3_t dos;
		std::chrono::steady_clock::time_point pornhub;
	};

	std::vector< gaysex> tracers;

	struct unity_str
	{
		char buffer[ 128 ];
	};

	class c_string
	{
	public:
		auto buffer() -> std::wstring
		{
			if (!this) return std::wstring();
			auto local = reinterpret_cast<std::uintptr_t>(this);
			wchar_t buffer[65];
			driver.read_buffer((PVOID)(local + 0x14), (std::uint8_t*)&buffer, 64);
			buffer[64] = '\0';
			return std::wstring(buffer);
		}
	};

	template <typename t>
	class c_list
	{
	public:
		auto size ( ) -> std::uint32_t
		{
			return driver.read<std::uint32_t> ( reinterpret_cast< std::uintptr_t >( this ) + 0x10 );
		}

		auto get ( std::uint32_t idx ) -> t
		{
			return driver.read_chain<t> ( reinterpret_cast< std::uintptr_t >( this ), { 0x18, 0x20ull + ( idx * 0x08ull ) } );
		}
	};

	template <typename t>
	class c_array
	{
	public:
		auto size ( ) -> std::uint32_t
		{
			return driver.read<std::uint32_t> ( reinterpret_cast< std::uintptr_t >( this ) + 0x18 );
		}

		auto get ( std::uint32_t idx ) -> t
		{
			return driver.read<t> ( reinterpret_cast< std::uintptr_t >( this ) + 0x20ull + ( idx * 0x8ull ) );
		}
	};

	class c_color
	{
	public:
		float r, g, b, a;

		c_color ( float _r, float _g, float _b, float _a )
		{

			r = _r;
			g = _g;
			b = _b;
			a = _a;

		}

		c_color ( uint32_t color )
		{
			this->a = ( color >> 24 ) & 0xff;
			this->r = ( color >> 16 ) & 0xff;
			this->g = ( color >> 8 ) & 0xff;
			this->b = ( color & 0xff );
		}

		static c_color from_hsb ( float flHue, float flSaturation, float flBrightness )
		{
			const float h = std::fmodf ( flHue, 1.0f ) / ( 60.0f / 360.0f );
			const int i = static_cast< int >( h );
			const float f = h - static_cast< float >( i );
			const float p = flBrightness * ( 1.0f - flSaturation );
			const float q = flBrightness * ( 1.0f - flSaturation * f );
			const float t = flBrightness * ( 1.0f - flSaturation * ( 1.0f - f ) );

			float r = 0.0f, g = 0.0f, b = 0.0f;

			switch ( i )
			{
			case 0:
				r = flBrightness, g = t, b = p;
				break;
			case 1:
				r = q, g = flBrightness, b = p;
				break;
			case 2:
				r = p, g = flBrightness, b = t;
				break;
			case 3:
				r = p, g = q, b = flBrightness;
				break;
			case 4:
				r = t, g = p, b = flBrightness;
				break;
			case 5:
			default:
				r = flBrightness, g = p, b = q;
				break;
			}

			return c_color ( r, g, b, 255 );
		}

		void random_color ( int iTick )
		{
			this->r = sin ( 3.f * iTick + 0.f ) * 127 + 128;
			this->g = sin ( 3.f * iTick + 0.f ) * 127 + 128;
			this->b = sin ( 3.f * iTick + 0.f ) * 127 + 128;
		}
	};

	enum class lifestate : std::uint32_t
	{
		alive = 0,
		dead = 1,
	};

	enum class category : std::uint32_t
	{
		shirt = 0,
		jacket = 1,
		hat = 2,
		mask = 3,
		footwear = 4,
		pants = 5,
		weapon = 6,
		misc = 7,
		deployable = 9,
		gloves = 10,
	};

	enum class model_flags : std::uint32_t
	{
		ducked = 1,
		jumped = 2,
		on_ground = 4,
		sleeping = 8,
		sprinting = 16,
		on_ladder = 32,
		flying = 64,
		aiming = 128,
		prone = 256,
		mounted = 512,
		relaxed = 1024,
		on_phone = 2048,
		crawling = 4096,
		head_look = 16384
	};

	enum class player_flags : std::uint32_t
	{
		unused1 = 1,
		unused2 = 2,
		isadmin = 4,
		receivingsnapshot = 8,
		sleeping = 16,
		spectating = 32,
		wounded = 64,
		isdeveloper = 128,
		connected = 256,
		thirdpersonviewmode = 1024,
		eyesviewmode = 2048,
		chatmute = 4096,
		nosprint = 8192,
		aiming = 16384,
		displaysash = 32768,
		relaxed = 65536,
		safezone = 131072,
		serverfall = 262144,
		incapacitated = 524288,
		workbench1 = 1048576,
		workbench2 = 2097152,
		workbench3 = 4194304,
	};

	enum class layer : std::uint32_t
	{
		none = 0,
		transparentfx = 1,
		ignore_raycast = 2,
		reserved1 = 3,
		water = 4,
		ui = 5,
		reserved2 = 6,
		reserved3 = 7,
		deployed = 8,
		ragdoll = 9,
		invisible = 10,
		ai = 11,
		player_movement = 12,
		vehicle_detailed = 13,
		game_trace = 14,
		vehicle_world = 15,
		world = 16,
		player_server = 17,
		trigger = 18,
		player_model_rendering = 19,
		physics_projectile = 20,
		construction = 21,
		construction_socket = 22,
		terrain = 23,
		transparent = 24,
		clutter = 25,
		debris = 26,
		vehicle_large = 27,
		prevent_movement = 28,
		prevent_building = 29,
		tree = 30,
		unused2 = 31,
	};

	enum class player_bones : std::uint32_t
	{
		l_hip = 1,
		l_knee,
		l_foot,
		l_toe,
		l_ankle_scale,
		pelvis,
		penis,
		GenitalCensor,
		GenitalCensor_LOD0,
		Inner_LOD0,
		GenitalCensor_LOD1,
		GenitalCensor_LOD2,
		r_hip,
		r_knee,
		r_foot,
		r_toe,
		r_ankle_scale,
		spine1,
		spine1_scale,
		spine2,
		spine3,
		spine4,
		l_clavicle,
		l_upperarm,
		l_forearm,
		l_hand,
		l_index1,
		l_index2,
		l_index3,
		l_little1,
		l_little2,
		l_little3,
		l_middle1,
		l_middle2,
		l_middle3,
		l_prop,
		l_ring1,
		l_ring2,
		l_ring3,
		l_thumb1,
		l_thumb2,
		l_thumb3,
		IKtarget_righthand_min,
		IKtarget_righthand_max,
		l_ulna,
		neck,
		head,
		jaw,
		eyeTranform,
		l_eye,
		l_Eyelid,
		r_eye,
		r_Eyelid,
		r_clavicle,
		r_upperarm,
		r_forearm,
		r_hand,
		r_index1,
		r_index2,
		r_index3,
		r_little1,
		r_little2,
		r_little3,
		r_middle1,
		r_middle2,
		r_middle3,
		r_prop,
		r_ring1,
		r_ring2,
		r_ring3,
		r_thumb1,
		r_thumb2,
		r_thumb3,
		IKtarget_lefthand_min,
		IKtarget_lefthand_max,
		r_ulna,
		l_breast,
		r_breast,
		BoobCensor,
		BreastCensor_LOD0,
		BreastCensor_LOD1,
		BreastCensor_LOD2,
		collision,
		displacement
	};

	bool timer_initialized = false;
	double timeSinceStartup = 0;
	double timeFrequency = 0;
	double get_time_since_startup() {
		LARGE_INTEGER PerformanceCount;
		LARGE_INTEGER FrequencyCount;

		if (!timer_initialized) {
			timer_initialized = true;

			PerformanceCount.QuadPart = 0;
			QueryPerformanceCounter(&PerformanceCount);

			FrequencyCount.QuadPart = 0;
			QueryPerformanceFrequency(&FrequencyCount);

			timeFrequency = (FrequencyCount.QuadPart);

			timeSinceStartup = (PerformanceCount.QuadPart);
		}

		PerformanceCount.QuadPart = 0;
		QueryPerformanceCounter(&PerformanceCount);

		return (double(PerformanceCount.QuadPart) - timeSinceStartup) / timeFrequency;
	}
}