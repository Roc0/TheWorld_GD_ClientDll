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

	if (!isValid())
		return;

	GD_Entity::_process(_delta);

	//String entityName = getEntityName();
	//if (entityName == "")
	//	return;

	char buffer[1024];

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

	Vector3 desideredEntityPos = getDesideredPos();
	Vector3 lastPos = getLastPos();

	if (desideredEntityPos != lastPos)
	{
		Transform t;
		t = get_transform();
		t.origin = desideredEntityPos;
		set_transform(t);
		setLastPos(desideredEntityPos);
		/*if (isDebugEnabled())
		{
			sprintf(buffer, "********************************************************************** Entity %d - %f/%f/%f", getId(), getLastPos().x, getLastPos().y, getLastPos().z);
			//String message;	message = message + "Entity " + _itoa(getId(), buffer, 10) + " x = " + _itoa(getLastPos().x, buffer, 10) + " y = " + _itoa(getLastPos().y, buffer, 10) + " z = " + _itoa(getLastPos().z, buffer, 10);
			Godot::print(buffer);
		}*/
	}

	{
		MeshInstance* pMeshI = (MeshInstance*)get_node("Shape");
		if (pMeshI)
		{
			//AABB aabb = pMeshI->get_aabb();
			//Vector3 startingPoint = aabb.position;
			//Vector3 endingPoint = startingPoint + aabb.size;
			//Vector3 startLine((endingPoint.x - startingPoint.x) / 2, (endingPoint.y - startingPoint.y) * 2, (endingPoint.z - startingPoint.z) / 2);
			//t = pMeshI->get_global_transform();
			//Vector3 endLine = startLine + Vector3;
			//startLine = pMeshI->to_global(startLine);
			//endLine = pMeshI->to_global(endLine);
			float desideredYaw, desideredPitch, desideredRoll;
			getDesideredDirection(desideredYaw, desideredPitch, desideredRoll);
			float lastYaw, lastPitch, lastRoll;
			getLastDirection(lastYaw, lastPitch, lastRoll);

			if (lastYaw != desideredYaw)
			{
				//pMeshI->rotate_y(yaw + kPi / 2);
				pMeshI->global_rotate(Vector3(0, 1, 0), desideredYaw);
				setLastDirection(desideredYaw, lastPitch, lastRoll);
				if (isDebugEnabled())
				{
					sprintf(buffer, "********************************************************************** Entity %d - %f", getId(), desideredYaw);
					Godot::print(buffer);
				}
			}
		}
	}

	if (isDebugEnabled())
	{
		Node* lineDrawerNode = get_node_or_null(NodePath("/root/DrawLine3d"));
		if (lineDrawerNode)
		{
			lineDrawerNode->call("DrawCartesianAxis", desideredEntityPos, 1.0);
		}
	}
	
	resetDebugEnabled();
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

	//char buffer[256];

	GD_ClientApp* pAppNode = (GD_ClientApp*)getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(getId(true), bPlayer);
	if (kbentity)
	{
		{
			Vector3 entityPos;
			kbentity->getForClientPosition(entityPos.x, entityPos.y, entityPos.z);
			// specificare meglio cosa si fa se uno fra x y e z � nullo (per y come ora per gli altri posizione precedente)
			if (entityPos.x != 0 || entityPos.y != 0 || entityPos.z != 0)
			{
				Vector3 lastPos = getLastPos();

				if (lastPos.x == entityPos.x && lastPos.z == entityPos.z)
				{
					setDesideredPos(lastPos);
				}
				else
				{
					if (entityPos.y == 0)
					{
						AABB aabb = pSpaceWorldNode->get_aabbForWorldCameraInitPos();
						Vector3 aabb_start = aabb.position;
						Vector3 aabb_end = aabb.position + aabb.size;

						Vector3 posOnGround(entityPos.x, aabb_end.y, entityPos.z);

						PhysicsDirectSpaceState* pSpaceState = get_world()->get_direct_space_state();
						Dictionary dict = pSpaceState->intersect_ray(Vector3(entityPos.x, aabb_end.y, entityPos.z), Vector3(entityPos.x, aabb_start.y, entityPos.z));
						if (dict.empty())
						{
							// Something was wrong
							//serverPos.y = aabb_end.y;
						}
						else
						{
							posOnGround = dict["position"];
							entityPos.y = posOnGround.y;

							MeshInstance* pMeshI = (MeshInstance*)get_node("Shape");
							if (pMeshI)
							{
								aabb = pMeshI->get_aabb();
								Vector3 startingPoint = aabb.position;
								Vector3 endingPoint = startingPoint + aabb.size;
								float offset = (endingPoint.y - startingPoint.y) / 2;
								entityPos.y += offset;
							}

							kbentity->setForClientPosition(entityPos.x, entityPos.y, entityPos.z);
						}
					}
					
					setDesideredPos(entityPos);

					/*Transform t;
					t = get_transform();
					t.origin = entityPos;
					if (t.origin != lastPos)
					{
						set_transform(t);
						setLastPos(t.origin);
						kbentity->setForClientPosition(t.origin.x, t.origin.y, t.origin.z);
						//if (isDebugEnabled())
						//{
						//	sprintf(buffer, "********************************************************************** Entity %d - %f/%f/%f", getId(), getLastPos().x, getLastPos().y, getLastPos().z);
						//	//String message;	message = message + "Entity " + _itoa(getId(), buffer, 10) + " x = " + _itoa(getLastPos().x, buffer, 10) + " y = " + _itoa(getLastPos().y, buffer, 10) + " z = " + _itoa(getLastPos().z, buffer, 10);
						//	Godot::print(buffer);
						//}
					}

					{
						MeshInstance* pMeshI = (MeshInstance*)get_node("Shape");
						if (pMeshI)
						{
							//AABB aabb = pMeshI->get_aabb();
							//Vector3 startingPoint = aabb.position;
							//Vector3 endingPoint = startingPoint + aabb.size;
							//Vector3 startLine((endingPoint.x - startingPoint.x) / 2, (endingPoint.y - startingPoint.y) * 2, (endingPoint.z - startingPoint.z) / 2);
							//t = pMeshI->get_global_transform();
							//Vector3 endLine = startLine + Vector3;
							//startLine = pMeshI->to_global(startLine);
							//endLine = pMeshI->to_global(endLine);
							float yaw, pitch, roll;
							kbentity->getForClientDirection(yaw, pitch, roll);

							if (getLastYaw() != yaw)
							{
								//pMeshI->rotate_y(yaw + kPi / 2);
								pMeshI->global_rotate(Vector3(0, 1, 0), yaw);
								setLastYaw(yaw);
								if (isDebugEnabled())
								{
									sprintf(buffer, "********************************************************************** Entity %d - %f", getId(), yaw);
									Godot::print(buffer);
								}
							}
						}
					}*/
				}
			}
		}

		{
			float yaw, pitch, roll;
			kbentity->getForClientDirection(yaw, pitch, roll);
			setDesideredDirection(yaw, pitch, roll);
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
