#pragma once

#include <Godot.hpp>
#include <Spatial.hpp>

namespace godot {

	class GD_TerrainM : public Spatial
	{
		GODOT_CLASS(GD_TerrainM, Spatial)

	public:
		static void _register_methods();

		GD_TerrainM();
		~GD_TerrainM();

		//
		// Godot Standard Functions
		//
		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
	private:
	};

}