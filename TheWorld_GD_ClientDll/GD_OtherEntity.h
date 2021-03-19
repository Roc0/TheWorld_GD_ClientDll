#pragma once

#include "GD_Entity.h"
#include <StaticBody.hpp>

namespace godot {

	class GD_OtherEntity : public StaticBody
	{
		GODOT_CLASS(GD_OtherEntity, StaticBody)

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

		GD_Entity_Common* entityCommon(void) { return &m_entityCommon; }

	private:
		GD_Entity_Common m_entityCommon;

	private:
		bool m_initialPosSetted;
		bool m_isMonster;
		bool m_isNPC;
	};

}

