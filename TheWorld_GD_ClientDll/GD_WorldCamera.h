#pragma once

#include <Godot.hpp>
#include <Camera.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

	class GD_WorldCamera : public Camera
	{
		GODOT_CLASS(GD_WorldCamera, Camera)

	public:
		static void _register_methods();

		GD_WorldCamera();
		~GD_WorldCamera();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);

	private:
	};

}

