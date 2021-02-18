#pragma once

#include <Godot.hpp>
#include <Spatial.hpp>
#include <SpatialMaterial.hpp>

#include <TheWorld_ClientApp.h>

#define RIGID_BODY_MODE_RIGID		0		
#define RIGID_BODY_MODE_STATIC		1		
#define RIGID_BODY_MODE_CHARACTER	2		
#define RIGID_BODY_MODE_KINEMATIC	3

namespace godot {

	class Entity_Visuals
	{
	public:
		Entity_Visuals(int iType);
		~Entity_Visuals();
	
		SpatialMaterial* getEntityShapeMaterial(SpatialMaterial* templateMaterial);

	private:
		int m_iType;
		Ref<SpatialMaterial> m_entityShapeMaterial;
	};

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

		bool initEntity(int id, Node* pClientApp, Node** ppEntityNode);
		bool destroyEntity(void);

		bool isValid(void) { return m_isValid; }
		void setValid(bool isValid) { m_isValid = isValid; }

		bool isPlayer() { return m_isPlayer; }
		void setPlayer(bool isPlayer) { m_isPlayer = isPlayer; }

		Node* getCameraNode(void);
		Node* getClientAppNode(void) { return m_pClientAppNode; }

		bool isEntityShapeUpdated(void) { return m_isEntityShapeUpdated; }
		void setEntityShapeUpdated(bool isEntityShapeUpdated) { m_isEntityShapeUpdated = isEntityShapeUpdated; }

		void setLastPos(Vector3 lastPos) { m_lastPos = lastPos; }
		Vector3 getLastPos(void) { return m_lastPos; };

		bool isDebugEnabled(void) { return m_isDebugEnabled; }

		// Entity Attributes
		int getId(bool bIgnoreValid = false);
		String getEntityName(bool bIgnoreValid = false);
		String getClassName(bool bIgnoreValid = false);

	private:
		// Entity Attributes
		int m_id;
		String m_entityName;
		String m_className;

		Node* m_pClientAppNode;
		bool m_isValid;
		bool m_isPlayer;
		Vector3 m_lastPos;
		bool m_isEntityShapeUpdated;
		bool m_isDebugEnabled;
	};

}

