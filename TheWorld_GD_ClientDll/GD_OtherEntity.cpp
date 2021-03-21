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
	entityCommon()->setPlayer(false);
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
	//return;	// DEBUG_MODE
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_OtherEntity::_process");

	if (!entityCommon()->isValid())
		return;

	char buffer[1024];

	GD_ClientApp* pAppNode = (GD_ClientApp*)entityCommon()->getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	String entityNodeName = get_name();

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(entityCommon()->getId(true), bPlayer);
	if (!kbentity)
	{
		pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}

	// *******************
	// Update Entity Shape
	// *******************
	if (!entityCommon()->isEntityInitializationComplete() || kbentity->getState() != entityCommon()->getLastEntityStatus())
	{
		entityCommon()->setLastEntityStatus(kbentity->getState());
		
		MeshInstance* entityShape = (MeshInstance*)get_node("Shape");
		if (!entityShape)
		{
			pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
			return;
		}

		Ref<SpatialMaterial> entityShapeMaterial = entityShape->get_surface_material(0);
		if (entityShapeMaterial.ptr())
		{
			std::wstring s = entityCommon()->getClassName().unicode_str();
			if (s != L"")
			{
				std::wstring m(L"monster");
				if (caseInSensWStringEqual(s, m))
				{
					Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_MONSTER);
					SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr(), kbentity->getState());
					entityShape->set_material_override(mat);
					//set_mode(RIGID_BODY_MODE_STATIC);
					
					setMonster();
					add_to_group(GD_CLIENTAPP_MONSTERS_GROUP);
				}
				else
				{
					Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_NPC);
					SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr(), kbentity->getState());
					entityShape->set_material_override(mat);
					//set_mode(RIGID_BODY_MODE_STATIC);

					setNPC();
					add_to_group(GD_CLIENTAPP_NPCS_GROUP);
				}
				entityCommon()->setEntityInitializationComplete(true);
			}
		}
	}
	// *******************
	// Update Entity Shape
	// *******************

	// *******************
	// Set Entity Position
	// *******************
	Vector3 desideredEntityPos = entityCommon()->getDesideredPos();
	Vector3 lastPos = entityCommon()->getLastPos();
	if (desideredEntityPos != lastPos)
	{
		Transform t;
		t = get_transform();
		t.origin = desideredEntityPos;
		set_transform(t);
		entityCommon()->setLastPos(desideredEntityPos);
	}
	// *******************
	// Set Entity Position
	// *******************

	// *********************
	// Set Shape Orientation
	// *********************
	Vector3 realDirection = Vector3(0, 0, 0);
	if (lastPos != Vector3(0, 0, 0))
	{
		realDirection = desideredEntityPos - lastPos;
		realDirection.normalize();
	}
	/*if (isNPC())
	{
		if (entityCommon()->isDebugEnabled())
		{
			Godot::print("NPC");
		}
	}*/
	{
		MeshInstance* pMeshI = (MeshInstance*)get_node("Shape");
		if (pMeshI)
		{
			float desideredYaw, desideredPitch, desideredRoll;
			entityCommon()->getDesideredDirection(desideredYaw, desideredPitch, desideredRoll);
			float lastYaw, lastPitch, lastRoll;
			entityCommon()->getLastDirection(lastYaw, lastPitch, lastRoll);

			if (lastYaw != desideredYaw)
			{
				//pMeshI->rotate_y(yaw + kPi / 2);
				//pMeshI->global_rotate(Vector3(0, 1, 0), desideredYaw);
				entityCommon()->setLastDirection(desideredYaw, lastPitch, lastRoll);
				if (entityCommon()->isDebugEnabled())
				{
					sprintf(buffer, "********************************************************************** Entity %d - %f", entityCommon()->getId(), desideredYaw);
					Godot::print(buffer);
				}
			}

			Transform t = get_transform();
			Vector3 desideredOrientation = desideredEntityPos + realDirection;
			if (desideredOrientation != t.origin)
				pMeshI->look_at(desideredOrientation, Vector3(0, 1, 0));
		}
	}
	// *********************
	// Set Shape Orientation
	// *********************

	// *****************
	// Set Entity Camera
	// *****************
	Node * entityCamera = entityCommon()->getCameraNode();
	if (entityCamera)
	{
		Vector3 cameraPos = entityCommon()->getLastPos() + Vector3(-1, 0, 0);
		Vector3 cameraLookAt = cameraPos;
		if (realDirection != Vector3(0, 0, 0))
			cameraLookAt = cameraPos + realDirection;

		//((GD_WorldCamera*)entityCamera)->look_at_from_position(entityCommon()->getLastPos() + Vector3(0, 10, -10), entityCommon()->getLastPos(), Vector3(0, 1, 0));
		//cameraPos = cameraLookAt;
		if (cameraPos != cameraLookAt)
		{
			/*((GD_WorldCamera*)entityCamera)->look_at_from_position(cameraPos, cameraLookAt, Vector3(0, 1, 0));
			Transform t;
			t = ((GD_WorldCamera*)entityCamera)->get_transform();*/
		}
		else
		{
			((GD_WorldCamera*)entityCamera)->look_at_from_position(entityCommon()->getLastPos() + Vector3(0, 5, -5), entityCommon()->getLastPos(), Vector3(0, 1, 0));
			
			/*Transform t;
			t.origin = Vector3(0, 20, -20);
			t.basis.x = Vector3(1, 0, 0);
			t.basis.y = Vector3(0, -0.707, 0.707);
			t.basis.z = Vector3(0, -0.707, -0.707);
			((GD_WorldCamera*)entityCamera)->set_transform(t);*/
		}
	}
	// *****************
	// Set Entity Camera
	// *****************

	// *************
	// DEBUG Drawing
	// *************
	if (entityCommon()->isDebugEnabled())
	{
		Node* lineDrawerNode = get_node_or_null(NodePath("/root/DrawLine3d"));
		if (lineDrawerNode)
		{
			lineDrawerNode->call("DrawCartesianAxis", desideredEntityPos, 1.0);

			Color yellow(244.0 / 256.0, 246.0 / 256.0, 10.0 / 256.0);
			lineDrawerNode->call("DrawRay", desideredEntityPos, realDirection, yellow, 0.1);

			Color white(1, 1, 1, 1);
			Vector3 realDirectionProjectedOnXZPlane(realDirection.x, 0, realDirection.z);
			realDirectionProjectedOnXZPlane.normalize();
			lineDrawerNode->call("DrawRay", desideredEntityPos, realDirectionProjectedOnXZPlane, white, 0.1);

			//float desideredYaw, desideredPitch, desideredRoll;
			//getDesideredDirection(desideredYaw, desideredPitch, desideredRoll);
			//Vector3 normalX(1, 0, 0);	Vector3 normalY(0, 1, 0);
			//Vector3 yawDirection = normalX.rotated(normalY, desideredYaw - kPi / 2);
			//yawDirection.normalize();
			//lineDrawerNode->call("DrawRay", desideredEntityPos, yawDirection, Color(0, 0, 0, 1), 0.1);
		}
	}
	// *************
	// DEBUG Drawing
	// *************

	entityCommon()->resetDebugEnabled();
}

void GD_OtherEntity::_physics_process(float _delta)
{
	//return;	// DEBUG_MODE
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_OtherEntity::_physics_process");

	if (!entityCommon()->isValid())
		return;

	//String entityName = getEntityName();
	//if (entityName == "")
	//	return;

	GD_ClientApp* pAppNode = (GD_ClientApp*)entityCommon()->getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	bool bPlayer;
	KBEntity* kbentity = pAppNode->getEntityById(entityCommon()->getId(true), bPlayer);
	if (kbentity)
	{
		{
			Vector3 entityPos;
			kbentity->getForClientPosition(entityPos.x, entityPos.y, entityPos.z);
			// specificare meglio cosa si fa se uno fra x y e z è nullo (per y come ora per gli altri posizione precedente)
			if (entityPos.x != 0 || entityPos.y != 0 || entityPos.z != 0)
			{
				Vector3 lastPos = entityCommon()->getLastPos();

				if (lastPos.x == entityPos.x && lastPos.z == entityPos.z)
				{
					entityCommon()->setDesideredPos(lastPos);
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
					
					entityCommon()->setDesideredPos(entityPos);
				}
			}
		}

		{
			float yaw, pitch, roll;
			kbentity->getForClientDirection(yaw, pitch, roll);
			entityCommon()->setDesideredDirection(yaw, pitch, roll);
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
	bool b = entityCommon()->initEntity(id, pClientApp, this);

	if (!b)
		return false;

	return b;
}

bool GD_OtherEntity::destroyEntity(void)
{
	bool b = entityCommon()->destroyEntity();

	return b;
}
