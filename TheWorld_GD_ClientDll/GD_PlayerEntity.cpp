#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_PlayerEntity.h"
#include "Utils.h"

#include <Godot.hpp>
#include <Reference.hpp>
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
	//Godot::print("GD_PlayerEntity::Init");
}

void GD_PlayerEntity::_ready()
{
	//Godot::print("GD_PlayerEntity::_ready");
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

	char buffer[256];

	GD_ClientApp* pAppNode = (GD_ClientApp*)getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	String entityNodeName = get_name();

	RigidBody* entityNode = (RigidBody*)get_node("Entity");
	if (!entityNode)
	{
		pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}

	// Update Entity Shape
	if (!isEntityInitializationComplete())
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
			entityNode->set_mode(RIGID_BODY_MODE_KINEMATIC);

			add_to_group(GD_CLIENTAPP_PLAYER_GROUP);

			setEntityInitializationComplete(true);
		}
	}

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(getId(true), bPlayer);
	if (kbentity)
	{
		float x, y, z;
		kbentity->getServerPosition(x, y, z);
		if (x != 0 || y != 0 || z != 0)
		{
			AABB aabb = pSpaceWorldNode->get_aabbForWorldCameraInitPos();
			Vector3 aabb_start = aabb.position;
			Vector3 aabb_end = aabb.position + aabb.size;

			Transform t;
			t.origin = Vector3(x, aabb_end.y, z);
			set_transform(t);
			if (t.origin != getLastPos())
			{
				setLastPos(t.origin);
				if (isDebugEnabled())
				{
					sprintf(buffer, "Player %d - x = %f y = %f z = %f", getId(), getLastPos().x, getLastPos().y, getLastPos().z);
					//String message;	message = message + "Player " + _itoa(getId(), buffer, 10) + " x = " + _itoa(getLastPos().x, buffer, 10) + " y = " + _itoa(getLastPos().y, buffer, 10) + " z = " + _itoa(getLastPos().z, buffer, 10);
					Godot::print(buffer);
				}
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
