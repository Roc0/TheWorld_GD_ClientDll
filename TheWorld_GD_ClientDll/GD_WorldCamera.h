#pragma once

#include <Godot.hpp>
#include <Camera.hpp>
#include <InputEvent.hpp>

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
		void _physics_process(float _delta);
		void _input(const Ref<InputEvent> event);
		bool updateCamera(void);

	private:
		// Camera
		Vector2 m_mouseRelativePos;
		float m_sensitivity;			// 0.0 - 1.0
		float m_smoothness;				// 0.0 - 0.999 tick 0.001
		float m_yaw;
		float m_lastYaw;
		float m_totalYaw;
		int m_yawLimit;
		float m_pitch;
		float m_lastPitch;
		float m_totalPitch;
		int m_pitchLimit;
		// Camera
	};

}

