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

		bool isMonster(void) { return m_isMonster; }
		void setMonster(bool b = true) { m_isMonster = b; }
		bool isNPC(void) { return m_isNPC; }
		void setNPC(bool b = true) { m_isNPC = b; }

	private:
		bool m_initialPosSetted;
		bool m_isMonster;
		bool m_isNPC;
	};

}

