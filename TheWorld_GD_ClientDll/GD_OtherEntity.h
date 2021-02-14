#pragma once

#include "GD_Entity.h"

namespace godot {

	class GD_OtherEntity : public GD_Entity
	{
		GODOT_CLASS(GD_OtherEntity, GD_Entity)

	public:
		static void _register_methods();

		GD_OtherEntity();
		~GD_OtherEntity();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
		void _physics_process(float _delta);
		void _input(const Ref<InputEvent> event);

		bool initEntity(int id, Node* pClientApp);
		bool destroyEntity(void);

	private:
		bool m_initialPosSetted;
	};

}

