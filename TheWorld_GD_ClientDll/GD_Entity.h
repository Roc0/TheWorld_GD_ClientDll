#pragma once

#include <Godot.hpp>
#include <StaticBody.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

	class GD_Entity : public StaticBody
	{
		GODOT_CLASS(GD_Entity, StaticBody)

	public:
		static void _register_methods();

		GD_Entity();
		~GD_Entity();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
		void _physics_process(float _delta);
		void _input(const Ref<InputEvent> event);

		bool initEntity(int id, Node* pClientApp);
		bool destroyEntity(void);

		bool isValid(void) { return m_isValid; }
		void setValid(bool isValid) { m_isValid = isValid; }

		bool isPlayer() { return m_isPlayer; }
		void setPlayer(bool isPlayer) { m_isPlayer = isPlayer; }


		// Entity Attributes
		String getEntityName();
		int get_id(bool bIgnoreValid = false);

	private:
		int m_id;
		Node* m_pClientApp;
		bool m_isValid;
		bool m_isPlayer;
		String m_entityName;
	};

}

