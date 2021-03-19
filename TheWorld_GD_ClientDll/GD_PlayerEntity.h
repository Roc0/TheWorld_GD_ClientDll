#pragma once

#include "GD_Entity.h"
#include <Vector2.hpp>
#include <KinematicBody.hpp>

namespace godot {

	class GD_PlayerEntity : public KinematicBody
	{
		GODOT_CLASS(GD_PlayerEntity, KinematicBody)

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

		Vector2 get_2d_movement(void);
		Vector3 moveVertically(Vector3 direction, float _delta);
		Vector3 h_accel(Vector3 direction, float _delta);
		void move(float _delta, KBEntity* kbentity);
		void faceForward(void);

		GD_Entity_Common* entityCommon(void) { return &m_entityCommon; }

	private:
		GD_Entity_Common m_entityCommon;

	private:
		Vector3 m_velocity;
		Vector3 m_direction;
		Vector3 m_facingDirection;
		float m_facingDirectionAngle;
		bool m_initPositionFromServer;

		const int C_MAX_SPEED = 20;
		const float C_ACCEL = 5.0;
		const float C_DECCEL = 15.0;
		const int C_JUMP_SPEED = 15;
		const int C_GRAVITY = -45;
	};

}
