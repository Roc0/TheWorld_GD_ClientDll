#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_OtherEntity.h"
#include "Utils.h"

#include <Godot.hpp>
#include <Reference.hpp>
#include <InputEvent.hpp>
#include <RigidBody.hpp>
#include <SpatialMaterial.hpp>
#include <RayCast.hpp>
#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>

using namespace godot;

void GD_OtherEntity::_register_methods()
{
	register_method("_ready", &GD_OtherEntity::_ready);
	register_method("_process", &GD_OtherEntity::_process);
	register_method("_physics_process", &GD_OtherEntity::_physics_process);
	register_method("_input", &GD_OtherEntity::_input);
}

GD_OtherEntity::GD_OtherEntity()
{
	setPlayer(false);
	setMonster(false);
	setNPC(false);

	m_initialPosSetted = false;
}

GD_OtherEntity::~GD_OtherEntity()
{
}

void GD_OtherEntity::_init()
{
	//Godot::print("GD_OtherEntity::Init");
}

void GD_OtherEntity::_ready()
{
	//Godot::print("GD_OtherEntity::_ready");
}

void GD_OtherEntity::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_OtherEntity::_process");

	GD_Entity::_process(_delta);

	if (!isValid())
		return;

	//String entityName = getEntityName();
	//if (entityName == "")
	//	return;

	char buffer[16];

	GD_ClientApp* pAppNode = (GD_ClientApp*)getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	String entityNodeName = get_name();

	// Update Entity Shape
	if (!isEntityInitializationComplete())
	{
		MeshInstance* entityShape = (MeshInstance*)get_node("Shape");
		if (!entityShape)
		{
			pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
			return;
		}

		Ref<SpatialMaterial> entityShapeMaterial = entityShape->get_surface_material(0);
		if (entityShapeMaterial.ptr())
		{
			std::wstring s = getClassName().unicode_str();
			if (s != L"")
			{
				std::wstring m(L"monster");
				if (caseInSensWStringEqual(s, m))
				{
					Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_MONSTER);
					SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr());
					entityShape->set_material_override(mat);
					set_mode(RIGID_BODY_MODE_STATIC);
					
					setMonster();
					add_to_group(GD_CLIENTAPP_MONSTERS_GROUP);
				}
				else
				{
					Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_NPC);
					SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr());
					entityShape->set_material_override(mat);
					set_mode(RIGID_BODY_MODE_STATIC);

					setNPC();
					add_to_group(GD_CLIENTAPP_NPCS_GROUP);
				}
				setEntityInitializationComplete(true);
			}
		}
	}
}

void GD_OtherEntity::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_OtherEntity::_physics_process");

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

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(getId(true), bPlayer);
	if (kbentity)
	{
		Vector3 serverPos;
		kbentity->getForClientPosition(serverPos.x, serverPos.y, serverPos.z);
		// specificare meglio cosa si fa se uno fra x y e z è nullo (per y come ora per gli altri posizione precedente)
		if (serverPos.x != 0 || serverPos.y != 0 || serverPos.z != 0)
		{
			Vector3 lastPos = getLastPos();

			if (!(lastPos.x == serverPos.x && lastPos.z == serverPos.z))
			{
				if (serverPos.y == 0)
				{
					AABB aabb = pSpaceWorldNode->get_aabbForWorldCameraInitPos();
					Vector3 aabb_start = aabb.position;
					Vector3 aabb_end = aabb.position + aabb.size;

					Vector3 posOnGround(serverPos.x, aabb_end.y, serverPos.z);

					PhysicsDirectSpaceState* pSpaceState = get_world()->get_direct_space_state();
					Dictionary dict = pSpaceState->intersect_ray(Vector3(serverPos.x, aabb_end.y, serverPos.z), Vector3(serverPos.x, aabb_start.y, serverPos.z));
					if (dict.empty())
					{
						// Something was wrong
						//serverPos.y = aabb_end.y;
					}
					else
					{
						posOnGround = dict["position"];
						serverPos.y = posOnGround.y;

						MeshInstance* pMeshI = (MeshInstance*)get_node("Shape");
						if (pMeshI)
						{
							aabb = pMeshI->get_aabb();
							Vector3 startingPoint = aabb.position;
							Vector3 endingPoint = startingPoint + aabb.size;
							float offset = (endingPoint.y - startingPoint.y) / 2;
							serverPos.y += offset;
						}
					}
				}

				Transform t;
				t = get_transform();
				t.origin = serverPos;
				if (t.origin != lastPos)
				{
					set_transform(t);
					setLastPos(t.origin);
					kbentity->setForClientPosition(t.origin.x, t.origin.y, t.origin.z);
					if (isDebugEnabled())
					{
						sprintf(buffer, "********************************************************************** Entity %d - %f/%f/%f", getId(), getLastPos().x, getLastPos().y, getLastPos().z);
						//String message;	message = message + "Entity " + _itoa(getId(), buffer, 10) + " x = " + _itoa(getLastPos().x, buffer, 10) + " y = " + _itoa(getLastPos().y, buffer, 10) + " z = " + _itoa(getLastPos().z, buffer, 10);
						Godot::print(buffer);
					}
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

void GD_OtherEntity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_OtherEntity::_input: " + event->as_text());
}

bool GD_OtherEntity::initEntity(int id, Node* pClientApp)
{
	bool b = GD_Entity::initEntity(id, pClientApp);

	if (!b)
		return false;

	return b;
}

bool GD_OtherEntity::destroyEntity(void)
{
	bool b = GD_Entity::destroyEntity();

	return b;
}
