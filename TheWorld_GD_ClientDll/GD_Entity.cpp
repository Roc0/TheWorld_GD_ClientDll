#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_Entity.h"
#include "Utils.h"

#include <InputEvent.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>

using namespace godot;

void GD_Entity::_register_methods()
{
	register_method("_ready", &GD_Entity::_ready);
	register_method("_process", &GD_Entity::_process);
	register_method("_physics_process", &GD_Entity::_physics_process);
	register_method("_input", &GD_Entity::_input);
}

GD_Entity::GD_Entity()
{
	m_id = -1;
	m_pClientApp = NULL;
	setValid(false);
	setPlayer(false);
	m_lastPos = Vector3(0, 0, 0);
}

GD_Entity::~GD_Entity()
{
}

void GD_Entity::_init()
{
	Godot::print("GD_Entity::Init");
}

void GD_Entity::_ready()
{
	Godot::print("GD_Entity::_ready");
}

void GD_Entity::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Entity::_process");

	if (!isValid())
		return;

	char buffer[16];
	
	GD_ClientApp* pApp = (GD_ClientApp*)m_pClientApp;
	bool bPlayer;
	KBEntity* kbentity = pApp->getEntityById(m_id, bPlayer);
	if (kbentity)
	{
		if (isPlayer())
		{
		}
		else
		{
			float x, y, z;
			kbentity->getPosition(x, y, z);
			if (x != 0 || y != 0 || z != 0)
			{
				AABB aabb = ((GD_SpaceWorld*)((GD_ClientApp*)m_pClientApp)->getSpaceWorldNode())->get_aabbForWorldCameraInitPos();
				Vector3 aabb_start = aabb.position;
				Vector3 aabb_end = aabb.position + aabb.size;

				
				Transform t;
				t.origin = Vector3(x, aabb_end.y, z);
				RigidBody* entity = (RigidBody*)get_node("Entity");
				if (!entity)
				{
					pApp->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
					return;
				}
				entity->set_transform(t);
				if (t.origin != m_lastPos)
				{
					m_lastPos = (t.origin);
					String message;	message = message + "Entity " + _itoa(m_id, buffer, 10) + " x = " + _itoa(m_lastPos.x, buffer, 10) + " y = " + _itoa(m_lastPos.y, buffer, 10) + " z = " + _itoa(m_lastPos.z, buffer, 10);
					Godot::print(message);
				}
			}
		}
	}
	else
	{
		pApp->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}
}

void GD_Entity::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Entity::_physics_process");
}

void GD_Entity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_Entity::_input: " + event->as_text());
}

bool GD_Entity::initEntity(int id, Node* pClientApp)
{
	m_id = id;
	m_pClientApp = pClientApp;

	setValid(true);

	char buffer[16];
	//String entityName = getEntityName();
	String nodeName, path;
	int64_t rigidBodyMode;
	
	if (isPlayer())
	{
		Node *pPlayerNode = ((GD_ClientApp*)m_pClientApp)->getPlayerNode(true);
		if (pPlayerNode)
		{
			nodeName = pPlayerNode->get_name();
			int id = ((GD_Entity*)pPlayerNode)->get_id(true);
			nodeName = nodeName + "_" + _itoa(id, buffer, 10);
			pPlayerNode->set_name(nodeName);
		}
		
		nodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;

		// Kinematic body mode. The body behaves like a KinematicBody, and can only move by user code
		rigidBodyMode = RIGID_BODY_MODE_KINEMATIC;

		path = "res://Player.tscn";
	}
	else
	{
		nodeName = GD_CLIENTAPP_OTHER_ENTITY_NODE;
		nodeName = nodeName + "_" + _itoa(id, buffer, 10);

		// Static mode. The body behaves like a StaticBody, and can only move by user code.
		rigidBodyMode = RIGID_BODY_MODE_STATIC;

		path = "res://OtherEntity.tscn";
	}

	set_name(nodeName);

	ResourceLoader* resLoader = ResourceLoader::get_singleton();
	Ref<PackedScene> s = resLoader->load(path);
	if (!s.ptr())
		return false;
	Node* pEntity = s->instance();
	if (!pEntity)
		return false;

	((RigidBody*)pEntity)->set_mode(rigidBodyMode);
	add_child(pEntity);
	//const String str = pEntity->get_name();
	//Godot::print(str);

	return true;
}

bool GD_Entity::destroyEntity(void)
{
	setValid(false);
	return true;
}

int GD_Entity::get_id(bool bIgnoreValid)
{
	if (bIgnoreValid)
		return m_id;
	else
		return isValid() ? m_id : -1;
}

String GD_Entity::getEntityName(bool bIgnoreValid)
{
	if (!isValid() && !bIgnoreValid)
		return "";

	if (m_entityName != "")
		return m_entityName;
	
	GD_ClientApp* pApp = (GD_ClientApp*)m_pClientApp;
	bool bPlayer;
	KBEntity* kbentity = pApp->getEntityById(m_id, bPlayer);
	if (kbentity)
		m_entityName =  kbentity->getName();
	
	//char buffer[16]; _itoa((int)m_id, buffer, 10);
	//String s = "getEntityName (";	s = s + buffer + "): " + kbentity->getName();
	//Godot::print(s);

	return m_entityName;
}
