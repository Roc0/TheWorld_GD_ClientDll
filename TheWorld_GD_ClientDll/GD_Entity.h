#pragma once

#include <Godot.hpp>
#include <Spatial.hpp>
#include <SpatialMaterial.hpp>
#include <RigidBody.hpp>

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

	class GD_Entity : public RigidBody
	{
		GODOT_CLASS(GD_Entity, RigidBody)

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

		Node* getCameraNode(void);
		Node* getCameraPosNode(void);
		Node* getClientAppNode(void) { return m_pClientAppNode; }

		bool isEntityInitializationComplete(void) { return m_isEntityInitializationComplete; }
		void setEntityInitializationComplete(bool isEntityInitializationComplete) { m_isEntityInitializationComplete = isEntityInitializationComplete; }

		void setLastPos(Vector3 lastPos) { m_lastPos = lastPos; }
		Vector3 getLastPos(void) { return m_lastPos; };

		void setLastYaw(float lastYaw) { m_lastYaw = lastYaw; }
		float getLastYaw(void) { return m_lastYaw; };

		bool isDebugEnabled(void);
		void resetDebugEnabled(void);

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
		float m_lastYaw;
		bool m_isEntityInitializationComplete;
		int m_isDebugEnabled;
	};

}

