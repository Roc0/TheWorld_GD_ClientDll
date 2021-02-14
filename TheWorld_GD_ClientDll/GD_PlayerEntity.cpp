#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_PlayerEntity.h"
#include "Utils.h"

#include <InputEvent.hpp>
#include <RigidBody.hpp>
#include <SpatialMaterial.hpp>
#include <RayCast.hpp>

using namespace godot;

void GD_PlayerEntity::_register_methods()
{
	register_method("_ready", &GD_PlayerEntity::_ready);
	register_method("_process", &GD_PlayerEntity::_process);
	register_method("_physics_process", &GD_PlayerEntity::_physics_process);
	register_method("_input", &GD_PlayerEntity::_input);
}

GD_PlayerEntity::GD_PlayerEntity()
{
	setPlayer(true);
}

GD_PlayerEntity::~GD_PlayerEntity()
{
}

void GD_PlayerEntity::_init()
{
	Godot::print("GD_PlayerEntity::Init");
}

void GD_PlayerEntity::_ready()
{
	Godot::print("GD_PlayerEntity::_ready");
}

void GD_PlayerEntity::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_PlayerEntity::_process");

	GD_Entity::_process(_delta);

	if (!isValid())
		return;

	//String entityName = getEntityName();
	//if (entityName == "")
	//	return;

	char buffer[16];

	GD_ClientApp* pAppNode = (GD_ClientApp*)getClientAppNode();

	if (!((GD_SpaceWorld*)pAppNode->getSpaceWorldNode())->isWorldInitialized())
		return;

	String entityNodeName = get_name();

	RigidBody* entityNode = (RigidBody*)get_node("Entity");
	if (!entityNode)
	{
		pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}

	// Update Entity Shape
	if (!isEntityShapeUpdated())
	{
		MeshInstance* entityShape = (MeshInstance*)get_node("Entity/Shape");
		if (!entityShape)
		{
			pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
			return;
		}

		Ref<SpatialMaterial> entityShapeMaterial = entityShape->get_surface_material(0);
		if (entityShapeMaterial.ptr())
		{
			Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_PLAYER);
			SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr());
			entityShape->set_material_override(mat);
			setEntityShapeUpdated(true);
		}
	}

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(get_id(true), bPlayer);
	if (kbentity)
	{
		float x, y, z;
		kbentity->getPosition(x, y, z);
		if (x != 0 || y != 0 || z != 0)
		{
			AABB aabb = ((GD_SpaceWorld*)pAppNode->getSpaceWorldNode())->get_aabbForWorldCameraInitPos();
			Vector3 aabb_start = aabb.position;
			Vector3 aabb_end = aabb.position + aabb.size;

			RayCast* pCaster = (RayCast*)entityNode->get_node("RayCast");

			Transform t;
			t.origin = Vector3(x, aabb_end.y, z);
			entityNode->set_transform(t);
			if (t.origin != getLastPos())
			{
				setLastPos(t.origin);
				//String message;	message = message + "Player " + _itoa(m_id, buffer, 10) + " x = " + _itoa(m_lastPos.x, buffer, 10) + " y = " + _itoa(m_lastPos.y, buffer, 10) + " z = " + _itoa(m_lastPos.z, buffer, 10);
				//Godot::print(message);
			}
		}
	}
	else
	{
		pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}
}

void GD_PlayerEntity::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_PlayerEntity::_physics_process");
}

void GD_PlayerEntity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_PlayerEntity::_input: " + event->as_text());
}

bool GD_PlayerEntity::initEntity(int id, Node* pClientApp)
{
	Node* pEntityNode = NULL;

	bool b = GD_Entity::initEntity(id, pClientApp, &pEntityNode);

	if (!b || !pEntityNode)
		return false;

	return b;
}

bool GD_PlayerEntity::destroyEntity(void)
{
	bool b = GD_Entity::destroyEntity();

	return b;
}
