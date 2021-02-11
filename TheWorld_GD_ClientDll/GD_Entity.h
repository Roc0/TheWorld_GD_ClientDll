#pragma once

#include <Godot.hpp>
#include <Spatial.hpp>

#include <TheWorld_ClientApp.h>

#define RIGID_BODY_MODE_RIGID		0		
#define RIGID_BODY_MODE_STATIC		1		
#define RIGID_BODY_MODE_CHARACTER	2		
#define RIGID_BODY_MODE_KINEMATIC	3

namespace godot {

	class GD_Entity : public Spatial
	{
		GODOT_CLASS(GD_Entity, Spatial)

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

		Node* getCamera(void);

		// Entity Attributes
		String getEntityName(bool bIgnoreValid = false);
		int get_id(bool bIgnoreValid = false);

	private:
		int m_id;
		Node* m_pClientApp;
		bool m_isValid;
		bool m_isPlayer;
		String m_entityName;
		Vector3 m_lastPos;
	};

}

