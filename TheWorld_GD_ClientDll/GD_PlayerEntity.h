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
		float m_facingDirection;
	};

}
