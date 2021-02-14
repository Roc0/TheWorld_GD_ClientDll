#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_Entity.h"
#include "Utils.h"

#include <InputEvent.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>
#include <SpatialMaterial.hpp>
//#include <SpatialMaterial.hpp>

using namespace godot;

Entity_Visuals::Entity_Visuals(int iType)
{
	m_iType = iType;
}

Entity_Visuals::~Entity_Visuals()
{
}

SpatialMaterial* Entity_Visuals::getEntityShapeMaterial(SpatialMaterial* templateMaterial)
{
	Color greenEntity(51.0 / 255, 102.0 / 255, 0, 255.0 / 255);
	Color redEntity(128.0 / 255, 25.0 / 255, 0, 255.0 / 255);
	Color lightblueEntity(0 / 255, 192.0 / 255, 255.0 / 255, 255.0 / 255);

	if (m_entityShapeMaterial.ptr())
		return m_entityShapeMaterial.ptr();
	else
	{
		m_entityShapeMaterial = templateMaterial->duplicate();
		if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_PLAYER)
		{
			m_entityShapeMaterial->set_albedo(greenEntity);
		}
		else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_MONSTER)
		{
			m_entityShapeMaterial->set_albedo(redEntity);
		}
		else if (m_iType == GD_CLIENTAPP_ENTITYVISUALS_NPC)
		{
			m_entityShapeMaterial->set_albedo(lightblueEntity);
		}
		return m_entityShapeMaterial.ptr();
	}
}

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
	m_pClientAppNode = NULL;
	setValid(false);
	setPlayer(false);
	m_lastPos = Vector3(0, 0, 0);
	m_isEntityShapeUpdated = false;
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

Node* GD_Entity::getCameraNode(void)
{
	Camera* entityCam = (Camera*)get_node("Entity/Camera");
	if (!entityCam)
	{
		return NULL;
	}

	return entityCam;
}

bool GD_Entity::initEntity(int id, Node* pClientApp, Node** ppEntityNode)
{
	m_id = id;
	m_pClientAppNode = pClientApp;

	setValid(true);

	char buffer[16];
	//String entityName = getEntityName();
	String nodeName, path;
	int64_t rigidBodyMode;

	//Color shapeColor;
	//Color greenEntity(51.0 / 255, 102.0 / 255, 0, 255.0 / 255);
	//Color redEntity(128.0 / 255, 25.0 / 255, 0, 255.0 / 255);
	//Color lightblueEntity(0 / 255, 192.0 / 255, 255.0 / 255, 255.0 / 255);

	if (isPlayer())
	{
		nodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;

		// Kinematic body mode. The body behaves like a KinematicBody, and can only move by user code
		rigidBodyMode = RIGID_BODY_MODE_KINEMATIC;

		path = "res://Player.tscn";

		//shapeColor = greenEntity;
	}
	else
	{
		nodeName = GD_CLIENTAPP_OTHER_ENTITY_NODE;
		nodeName = nodeName + "_" + _itoa(id, buffer, 10);

		// Static mode. The body behaves like a StaticBody, and can only move by user code.
		//rigidBodyMode = RIGID_BODY_MODE_STATIC;
		rigidBodyMode = RIGID_BODY_MODE_RIGID;

		path = "res://OtherEntity.tscn";

		/*std::wstring s = getClassName().unicode_str();
		std::wstring m(L"monster");
		if (caseInSensWStringEqual(s, m))
			shapeColor = redEntity;
		else
			shapeColor = lightblueEntity;*/
	}

	set_name(nodeName);

	ResourceLoader* resLoader = ResourceLoader::get_singleton();
	Ref<PackedScene> s = resLoader->load(path);
	if (!s.ptr())
		return false;
	Node* pEntityNode = s->instance();
	if (!pEntityNode)
		return false;

	((RigidBody*)pEntityNode)->set_mode(rigidBodyMode);
	add_child(pEntityNode);

	*ppEntityNode = pEntityNode;
	
	/*MeshInstance* shape = (MeshInstance*)pEntityNode->get_node("Shape");
	if (!shape)
		return false;

	Ref<SpatialMaterial> shapeMaterial = shape->get_surface_material(0);
	if (!shapeMaterial.ptr())
		return false;

	shapeMaterial->set_albedo(shapeColor);*/

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

String GD_Entity::getClassName(bool bIgnoreValid)
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
