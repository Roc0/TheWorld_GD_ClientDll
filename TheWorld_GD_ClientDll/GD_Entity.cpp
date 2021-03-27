#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_Entity.h"
#include "Utils.h"

#include <Godot.hpp>
#include <Reference.hpp>
#include <InputEvent.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>
#include <SpatialMaterial.hpp>
#include <Position3d.hpp>
#include <CollisionObject.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <PhysicsBody.hpp>
#include <World.hpp>

using namespace godot;

Entity_Visuals::Entity_Visuals(int iType)
{
	m_iType = iType;
}

Entity_Visuals::~Entity_Visuals()
{
}

SpatialMaterial* Entity_Visuals::getEntityShapeMaterial(SpatialMaterial* templateMaterial, int iEntityStatus)
{
	Color greenEntity(51.0 / 255.0, 102.0 / 255.0, 0.0, 255.0 / 255.0);
	Color redEntity(128.0 / 255.0, 25.0 / 255.0, 0.0, 255.0 / 255.0);
	Color lightblueEntity(0.0 / 255.0, 192.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);

	Color whiteEntity(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);

	switch (iEntityStatus)
	{
		case ENTITY_STATE_DEAD:
		{
			if (m_entityShapeMaterial_deadEntity.ptr())
				return m_entityShapeMaterial_deadEntity.ptr();
			else
			{
				m_entityShapeMaterial_deadEntity = templateMaterial->duplicate();
				if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_PLAYER)
				{
					m_entityShapeMaterial_deadEntity->set_albedo(whiteEntity);
				}
				else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_MONSTER)
				{
					m_entityShapeMaterial_deadEntity->set_albedo(whiteEntity);
				}
				else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_NPC)
				{
					m_entityShapeMaterial_deadEntity->set_albedo(whiteEntity);
				}
				return m_entityShapeMaterial_deadEntity.ptr();
			}
		}
		break;

		//case ENTITY_STATE_UNKNOW:
		//case ENTITY_STATE_FREE:
		//case ENTITY_STATE_REST:
		//case ENTITY_STATE_FIGHT:
		//case ENTITY_STATE_MAX:
		default:
		{
			if (m_entityShapeMaterial_freeEntity.ptr())
				return m_entityShapeMaterial_freeEntity.ptr();
			else
			{
				m_entityShapeMaterial_freeEntity = templateMaterial->duplicate();
				if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_PLAYER)
				{
					m_entityShapeMaterial_freeEntity->set_albedo(greenEntity);
				}
				else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_MONSTER)
				{
					m_entityShapeMaterial_freeEntity->set_albedo(redEntity);
				}
				else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_NPC)
				{
					m_entityShapeMaterial_freeEntity->set_albedo(lightblueEntity);
				}
				return m_entityShapeMaterial_freeEntity.ptr();
			}
		}
		break;
	}
}

GD_Entity_Common::GD_Entity_Common()
{
	m_id = -1;
	m_kbentity = NULL;
	m_pClientAppNode = NULL;
	setValid(false);
	setPlayer(false);
	m_lastPos = Vector3(0, 0, 0);
	m_isEntityInitializationComplete = false;
	m_iLastEntityStatus = ENTITY_STATE_UNKNOW;
	m_HP = 0;	m_MaxHP = 0;	m_MP = 0;	m_MaxMP = 0;

	resetDebugEnabled();
}

GD_Entity_Common::~GD_Entity_Common()
{
}

bool GD_Entity_Common::isDebugEnabled(void)
{
	if (m_isDebugEnabled == -1)
	{
		m_isDebugEnabled = ((GD_ClientApp*)m_pClientAppNode)->isDebugEnabled() ? 1 : 0;
	}
	return (m_isDebugEnabled == 1 ? true : false);
}

void GD_Entity_Common::resetDebugEnabled(void)
{
	m_isDebugEnabled = -1;
}

Node* GD_Entity_Common::getEntityNode(void)
{
	if (!m_pClientAppNode)
		return NULL;

	Node* pSpaceWorldNode = ((GD_ClientApp*)m_pClientAppNode)->getSpaceWorldNode();
	if (!pSpaceWorldNode)
		return NULL;

	Node* pWorldNode = ((GD_SpaceWorld*)pSpaceWorldNode)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	String sEntitiesPath = GD_CLIENTAPP_ENTITIES_CONTAINER_NODE;
	NodePath entitiesPath(sEntitiesPath);
	Node* entitiesNode = pWorldNode->get_node(entitiesPath);
	if (!entitiesNode)
		return NULL;

	NodePath entityNodePath(m_entityNodeName.c_str());
	Node* pentityNode = entitiesNode->get_node(entityNodePath);

	return pentityNode;
}

Node* GD_Entity_Common::getCameraNode(void)
{
	Node* entityNode = getEntityNode();
	if (!entityNode)
		return NULL;

	Camera* entityCam = (Camera*)entityNode->get_node_or_null("Camera");
	if (!entityCam)
	{
		entityCam = GD_WorldCamera::_new();
		if (entityCam)
			entityNode->add_child(entityCam);
		else
			return NULL;

		if (isPlayer())
			((GD_WorldCamera*)entityCam)->initPlayerCamera();
		else
			((GD_WorldCamera*)entityCam)->initOtherEntityCamera();

		//((GD_WorldCamera*)entityCam)->look_at_from_position(getLastPos() + Vector3(0, 5, -5), getLastPos(), Vector3(0, 1, 0));
	}

	return entityCam;
}

bool GD_Entity_Common::initEntity(int id, Node* pClientApp, Node* pEntityNode)
{
	m_id = id;
	m_pClientAppNode = pClientApp;

	setValid(true);

	char buffer[16];
	String path;
	int64_t rigidBodyMode;

	if (isPlayer())
	{
		m_entityNodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;
		path = "res://Player.tscn";
	}
	else
	{
		m_entityNodeName = GD_CLIENTAPP_OTHER_ENTITY_NODE;
		m_entityNodeName = m_entityNodeName + "_" + _itoa(id, buffer, 10);
		path = "res://OtherEntity.tscn";
	}

	pEntityNode->set_name(m_entityNodeName.c_str());
	pEntityNode->add_to_group(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	//set_mode(rigidBodyMode);

	// use resource as template duplicating nodes
	{
		ResourceLoader* resLoader = ResourceLoader::get_singleton();
		Ref<PackedScene> s = resLoader->load(path);
		if (!s.ptr())
			return false;
		Node* pEntityNodeTemplate = s->instance();
		if (!pEntityNodeTemplate)
			return false;

		Node *pNode = pEntityNodeTemplate->get_node("Tail");
		if (!pNode)
			return false;
		pEntityNode->add_child(pNode->duplicate());
		pNode = pEntityNodeTemplate->get_node("Body");
		if (!pNode)
			return false;
		pEntityNode->add_child(pNode->duplicate());
		pNode = pEntityNodeTemplate->get_node("Head");
		if (!pNode)
			return false;
		pEntityNode->add_child(pNode->duplicate());

		pNode = pEntityNodeTemplate->get_node("CollisionShape");
		if (!pNode)
			return false;
		pEntityNode->add_child(pNode->duplicate());

		pEntityNodeTemplate->queue_free();
	}

	if (((GD_ClientApp*)m_pClientAppNode)->isDebugEnabled())
		Godot::print("GD_Entity_Common::initEntity " + String(m_entityNodeName.c_str()));
	
	return true;
}

bool GD_Entity_Common::destroyEntity(void)
{
	setValid(false);
	return true;
}

int GD_Entity_Common::getId(bool bIgnoreValid)
{
	if (bIgnoreValid)
		return m_id;
	else
		return isValid() ? m_id : -1;
}

KBEntity* GD_Entity_Common::getEntity(bool bIgnoreValid)
{
	if (!isValid() && !bIgnoreValid)
		return NULL;

	if (m_kbentity)
		return m_kbentity;
	else
	{
		bool bPlayer;
		m_kbentity = ((GD_ClientApp*)m_pClientAppNode)->getEntityById(m_id, bPlayer);
		return m_kbentity;
	}
}

String GD_Entity_Common::getEntityName(bool bIgnoreValid)
{
	if (!isValid() && !bIgnoreValid)
		return "";

	if (m_entityName != "")
		return m_entityName;
	
	GD_ClientApp* pApp = (GD_ClientApp*)m_pClientAppNode;
	bool bPlayer;
	KBEntity* kbentity = pApp->getEntityById(m_id, bPlayer);
	if (kbentity)
		m_entityName =  kbentity->getName();
	
	//char buffer[16]; _itoa((int)m_id, buffer, 10);
	//String s = "getEntityName (";	s = s + buffer + "): " + kbentity->getName();
	//Godot::print(s);

	return m_entityName;
}

String GD_Entity_Common::getClassName(bool bIgnoreValid)
{
	if (!isValid() && !bIgnoreValid)
		return "";

	if (m_className != "")
		return m_className;

	GD_ClientApp* pApp = (GD_ClientApp*)m_pClientAppNode;
	bool bPlayer;
	KBEntity* kbentity = pApp->getEntityById(m_id, bPlayer);
	if (kbentity)
		m_className = kbentity->getClassName();

	return m_className;
}

void GD_Entity_Common::CalcPositionOnGround(PhysicsBody* pEntityNode)
{
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)((GD_ClientApp*)m_pClientAppNode)->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	AABB aabb = pSpaceWorldNode->get_aabbForWorldCameraInitPos();
	Vector3 aabb_start = aabb.position;
	Vector3 aabb_end = aabb.position + aabb.size;

	Vector3 posOnGround;

	PhysicsDirectSpaceState* pSpaceState = pEntityNode->get_world()->get_direct_space_state();
	KBEntity* kbentity = getEntity();
	Vector3 entityPos;
	kbentity->getForClientPosition(entityPos.x, entityPos.y, entityPos.z);
	Dictionary dict = pSpaceState->intersect_ray(Vector3(entityPos.x, aabb_end.y, entityPos.z), Vector3(entityPos.x, aabb_start.y, entityPos.z));
	if (dict.empty())
	{
		// Something was wrong
		posOnGround = Vector3(entityPos.x, aabb_end.y, entityPos.z);
		entityPos.y = posOnGround.y;
	}
	else
	{
		posOnGround = dict["position"];
		entityPos.y = posOnGround.y;

		MeshInstance* pMeshI = (MeshInstance*)pEntityNode->get_node("Body");
		if (pMeshI)
		{
			aabb = pMeshI->get_aabb();
			Vector3 startingPoint = aabb.position;
			Vector3 endingPoint = startingPoint + aabb.size;
			float offset = (endingPoint.y - startingPoint.y) / 2;
			entityPos.y += offset;
		}
	}

	kbentity->setForClientPosition(entityPos.x, entityPos.y, entityPos.z);
}
