#pragma once
#include "imports.hpp"

class c_object_class
{
public:
	declare_member ( WORD, tag, 0x54 );

	auto get_name ( ) -> std::string
	{
		auto name = driver.read_chain<std::uintptr_t> ( reinterpret_cast< std::uintptr_t >( this ), { 0x60 } );

		return driver.read_string ( name + 0x0 );
	}

	auto get_position ( ) -> geo::vec3_t
	{
		auto object = driver.read<std::uintptr_t> ( reinterpret_cast< std::uintptr_t >( this ) + 0x30 );
		if ( !object ) return geo::vec3_t ( 0.00f, 0.00f, 0.00f );

		auto transform = driver.read<std::uintptr_t> ( object + 0x08 );
		if ( !transform ) return geo::vec3_t ( 0.00f, 0.00f, 0.00f );

		auto visual_state = driver.read<std::uintptr_t> ( transform + 0x38 );
		if ( !visual_state ) return geo::vec3_t ( 0.00f, 0.00f, 0.00f );

		return driver.read<geo::vec3_t> ( visual_state + 0x90 );
	}
};