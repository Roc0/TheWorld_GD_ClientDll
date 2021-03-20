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

#define GET_ENTITY_COMMON(_ptr_entity_common, _entity_node) \
	{ \
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(_entity_node); \
		GD_OtherEntity* pOtherEntityNode = cast_to<GD_OtherEntity>(_entity_node); \
		if (pPlayerEntityNode) \
			_ptr_entity_common = pPlayerEntityNode->entityCommon(); \
		else \
			_ptr_entity_common = pOtherEntityNode->entityCommon(); \
	}

namespace godot {

	class Entity_Visuals
	{
	public:
		Entity_Visuals(int iType);
		~Entity_Visuals();
	
		SpatialMaterial* getEntityShapeMaterial(SpatialMaterial* templateMaterial, int iEntityStatus);

	private:
		int m_iType;
		Ref<SpatialMaterial> m_entityShapeMaterial_freeEntity;
		Ref<SpatialMaterial> m_entityShapeMaterial_deadEntity;
	};

	class GD_Entity_Common //: public RigidBody
	{
	public:
		GD_Entity_Common();
		~GD_Entity_Common();

		bool initEntity(int id, Node* pClientApp, Node* pEntityNode);
		bool destroyEntity(void);

		bool isValid(void) { return m_isValid; }
		void setValid(bool isValid) { m_isValid = isValid; }

		bool isPlayer() { return m_isPlayer; }
		void setPlayer(bool isPlayer) { m_isPlayer = isPlayer; }

		Node* getEntityNode(void);
		Node* getCameraNode(void);
		Node* getCameraPosNode(void);
		Node* getClientAppNode(void) { return m_pClientAppNode; }

		bool isEntityInitializationComplete(void) { return m_isEntityInitializationComplete; }
		void setEntityInitializationComplete(bool isEntityInitializationComplete) { m_isEntityInitializationComplete = isEntityInitializationComplete; }

		void setDesideredPos(Vector3 desideredPos) { m_desideredPos = desideredPos; }
		Vector3 getDesideredPos(void) { return m_desideredPos; };

		void setLastPos(Vector3 lastPos) { m_lastPos = lastPos; }
		Vector3 getLastPos(void) { return m_lastPos; };

		void setDesideredDirection(float yaw, float pitch, float roll)
		{
			m_desideredDirection.z = yaw;
			m_desideredDirection.y = pitch;
			m_desideredDirection.x = roll;
		}
		void getDesideredDirection(float& yaw, float& pitch, float& roll)
		{
			yaw = m_desideredDirection.z;
			pitch = m_desideredDirection.y;
			roll = m_desideredDirection.x;
		};

		void setLastDirection(float yaw, float pitch, float roll)
		{ 
			m_lastDirection.z = yaw; 
			m_lastDirection.y = pitch;
			m_lastDirection.x = roll;
		}
		void getLastDirection(float& yaw, float& pitch, float& roll)
		{ 
			yaw = m_lastDirection.z;
			pitch = m_lastDirection.y;
			roll = m_lastDirection.x;
		};

		void setLastEntityStatus(int iLastEntityStatus) { m_iLastEntityStatus = iLastEntityStatus; }
		int getLastEntityStatus(void) { return m_iLastEntityStatus; }

		bool isDebugEnabled(void);
		void resetDebugEnabled(void);

		// Entity Attributes
		int getId(bool bIgnoreValid = false);
		String getEntityName(bool bIgnoreValid = false);
		String getClassName(bool bIgnoreValid = false);
		void setHP(int HP) { m_HP = HP; }
		int getHP(void) { return m_HP; }
		void setMaxHP(int MaxHP) { m_MaxHP = MaxHP; }
		int getMaxHP(void) { return m_MaxHP; }
		void setMP(int MP) { m_MP = MP; }
		int getMP(void) { return m_MP; }
		void setMaxMP(int MaxMP) { m_MaxMP = MaxMP; }
		int getMaxMP(void) { return m_MaxMP; }

	private:
		// Entity Attributes
		int m_id;
		String m_entityName;
		String m_className;
		int m_HP, m_MaxHP, m_MP, m_MaxMP;

		std::string m_entityNodeName;

		Node* m_pClientAppNode;
		bool m_isValid;
		bool m_isPlayer;
		Vector3 m_lastPos, m_desideredPos;
		Vector3 m_lastDirection, m_desideredDirection;
		int m_iLastEntityStatus;
		bool m_isEntityInitializationComplete;
		int m_isDebugEnabled;
	};

}