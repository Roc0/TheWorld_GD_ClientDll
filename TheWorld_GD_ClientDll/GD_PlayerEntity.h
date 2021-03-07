#pragma once

#include "GD_Entity.h"

namespace godot {

	class GD_PlayerEntity : public GD_Entity
	{
		GODOT_CLASS(GD_PlayerEntity, GD_Entity)

	public:
		static void _register_methods();

		GD_PlayerEntity();
		~GD_PlayerEntity();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
		void _physics_process(float _delta);
		void _input(const Ref<InputEvent> event);

		bool initEntity(int id, Node* pClientApp);
		bool destroyEntity(void);

		void move(float _delta);
		void faceForward(void);

	private:
		Vector3 m_velocity;
		Vector3 m_direction;
		float m_facingDirection;
		bool m_initPositionFromServer;

		const int C_MAX_SPEED = 20;
		const float C_ACCEL = 5.0;
		const float C_DECCEL = 15.0;
		const int C_JUMP_SPEED = 15;
		const int C_GRAVITY = -45;
	};

}
