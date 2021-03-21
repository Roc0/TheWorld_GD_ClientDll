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
#include <Input.hpp>

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
	entityCommon()->setPlayer(true);
	m_facingDirectionAngle = 0;
	m_initPositionFromServer = true;
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
	//return;	// DEBUG_MODE
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_PlayerEntity::_process");

	if (!entityCommon()->isValid())
		return;

	//String entityName = getEntityName();
	//if (entityName == "")
	//	return;

	char buffer[256];

	GD_ClientApp* pAppNode = (GD_ClientApp*)entityCommon()->getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();

	if (!pSpaceWorldNode->isWorldInitialized())
		return;

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
			Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_PLAYER);
			SpatialMaterial* mat = ev->getEntityShapeMaterial(entityShapeMaterial.ptr(), kbentity->getState());
			entityShape->set_material_override(mat);

			entityCommon()->setEntityInitializationComplete(true);
		}
	}
	// *******************
	// Update Entity Shape
	// *******************

	Vector3 lastPos = entityCommon()->getLastPos();

	// *******************
	// Set Entity Position
	// *******************
	if (m_initPositionFromServer)
	{
		float x, y, z;
		kbentity->getForClientPosition(x, y, z);
		if (x != 0 || y != 0 || z != 0)
		{
			AABB aabb = pSpaceWorldNode->get_aabbForWorldCameraInitPos();
			Vector3 aabb_start = aabb.position;
			Vector3 aabb_end = aabb.position + aabb.size;

			Transform t;
			t = get_transform();
			t.origin = Vector3(x, aabb_end.y + 10, z);
			set_transform(t);
		}
		m_initPositionFromServer = false;
	}
	
	Vector3 newEntityPos;

	{
		move(_delta, kbentity);
		float x, y, z;
		kbentity->getForClientPosition(x, y, z);
		newEntityPos.x = x;	newEntityPos.y = y;	newEntityPos.z = z;
	}
	// *******************
	// Set Entity Position
	// *******************

	// **********************
	// Set Entity Orientation
	// **********************
	if (newEntityPos != lastPos)
	{
		//Transform t = get_transform();
		Vector3 faceTo = newEntityPos + m_realVelocity;
		
		if (faceTo != newEntityPos)
		{
			Vector3 realDirection = faceTo - newEntityPos;
			float angle = realDirection.angle_to(Vector3UP);
			if (isEqualWithLimitedPrecision(angle, 0, 6) || isEqualWithLimitedPrecision(angle, 3.141592, 6))
				Godot::print("angle - " + String(std::to_string(angle).c_str()));
			else
			{
				look_at(faceTo, Vector3UP);
				Godot::print("angle - " + String(std::to_string(angle).c_str()));
				Godot::print("m_realVelocity - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));
				if (m_realVelocity.x == m_realVelocity.z)
					Godot::print("AAA");
			}
		}
	}

	//faceForward();
	// **********************
	// Set Entity Orientation
	// **********************

	if (newEntityPos != lastPos)
		entityCommon()->setLastPos(newEntityPos);

	// *****************
	// Set Entity Camera
	// *****************
	if (lastPos == Vector3Zero)
	{
		Node* entityCamera = entityCommon()->getCameraNode();
		if (entityCamera)
			((GD_WorldCamera*)entityCamera)->look_at_from_position(entityCommon()->getLastPos() + Vector3(0, 5, 5), entityCommon()->getLastPos(), Vector3(0, 1, 0));
	}
	// *****************
	// Set Entity Camera
	// *****************

	// *****************
	// Update server POS
	// *****************
	pAppNode->kbengine_UpdateVolatile();
	// *****************
	// Update server POS
	// *****************

	// *************
	// DEBUG Drawing
	// *************
	if (entityCommon()->isDebugEnabled())
	{
		Node* lineDrawerNode = get_node_or_null(NodePath("/root/DrawLine3d"));
		if (lineDrawerNode)
		{
			float x, y, z;
			kbentity->getForClientPosition(x, y, z);
			Vector3 entityPos(x, y, z);
			lineDrawerNode->call("DrawCartesianAxis", entityPos, 1.0);

			/*Color yellow(244.0 / 256.0, 246.0 / 256.0, 10.0 / 256.0);
			lineDrawerNode->call("DrawRay", entityPos, realDirection, yellow, 0.1);

			Color white(1, 1, 1, 1);
			Vector3 realDirectionProjectedOnXZPlane(realDirection.x, 0, realDirection.z);
			realDirectionProjectedOnXZPlane.normalize();
			lineDrawerNode->call("DrawRay", entityPos, realDirectionProjectedOnXZPlane, white, 0.1);*/

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

void GD_PlayerEntity::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_PlayerEntity::_physics_process");
}

void GD_PlayerEntity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_PlayerEntity::_input: " + event->as_text());
}

godot::Vector2 GD_PlayerEntity::get_2d_movement(void)
{
	godot::Vector2 desideredMovement;

	Input* input = Input::get_singleton();

	if (input->is_action_pressed("ui_up") && !input->is_action_pressed("ui_down"))
	{
		desideredMovement.y = -1;
		m_facingDirectionAngle = 0;
	}
	if (input->is_action_pressed("ui_down") && !input->is_action_pressed("ui_up"))
	{
		desideredMovement.y = 1;
		m_facingDirectionAngle = kPi;
	}

	if (input->is_action_pressed("ui_left") && !input->is_action_pressed("ui_right"))
	{
		desideredMovement.x = -1;
		m_facingDirectionAngle = kPi * 0.5;
	}
	if (input->is_action_pressed("ui_right") && !input->is_action_pressed("ui_left"))
	{
		desideredMovement.x = 1;
		m_facingDirectionAngle = kPi * 1.5;
	}

	return desideredMovement.normalized();
}

godot::Vector3 GD_PlayerEntity::moveVertically(godot::Vector3 direction, float _delta)
{
	m_velocity.y += C_GRAVITY * _delta;

	Input* input = Input::get_singleton();
	if (input->is_action_pressed("ui_select") && is_on_floor())
		m_velocity.y = C_JUMP_SPEED;

	if (is_on_floor())
		m_velocity.y = 0;

	direction.y = 0;
	direction.normalize();

	return direction;
}

godot::Vector3 GD_PlayerEntity::h_accel(godot::Vector3 direction, float _delta)
{
	Vector3 velocity_2d = m_velocity;
	velocity_2d.y = 0;

	Vector3 target = direction;
	target *= C_MAX_SPEED;

	float acceleration;
	if (direction.dot(velocity_2d) > 0)
		acceleration = C_ACCEL;
	else
		acceleration = C_DECCEL;

	velocity_2d = velocity_2d.linear_interpolate(target, acceleration * _delta);

	m_velocity.x = velocity_2d.x;
	m_velocity.z = velocity_2d.z;

	return m_velocity;
}

void GD_PlayerEntity::move(float _delta, KBEntity* kbentity)
{
	godot::Vector2 desideredMovement = get_2d_movement();

	GD_ClientApp *pClientApp = (GD_ClientApp*)entityCommon()->getClientAppNode();
	if (!pClientApp)
		return;

	GD_WorldCamera* pActiveCamera = (GD_WorldCamera*)pClientApp->getActiveCamera();
	if (!pActiveCamera)
	{
		pClientApp->setAppInError(GD_CLIENTAPP_ERROR_GET_ACTIVE_CAMERA);
		return;
	}

	Transform cameraGlobalTranform = pActiveCamera->get_global_transform();

	m_direction = Vector3Zero;
	m_direction += cameraGlobalTranform.basis.z.normalized() * desideredMovement.y;
	m_direction += cameraGlobalTranform.basis.x.normalized() * desideredMovement.x;

	m_direction = moveVertically(m_direction, _delta);

	m_velocity = h_accel(m_direction, _delta);

	m_realVelocity = move_and_slide(m_velocity, Vector3(0, 1, 0));
	/*if (m_velocity == m_realVelocity)
		Godot::print("UGUALI  - " + String(std::to_string(m_velocity.x).c_str()) + " " + String(std::to_string(m_velocity.y).c_str()) + " " + String(std::to_string(m_velocity.z).c_str()) + " - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));
	else
		Godot::print("DIVERSI - " + String(std::to_string(m_velocity.x).c_str()) + " " + String(std::to_string(m_velocity.y).c_str()) + " " + String(std::to_string(m_velocity.z).c_str()) + " - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));*/

	Transform t;
	t = get_transform();
	kbentity->setForClientPosition(t.origin.x, t.origin.y, t.origin.z);
}

void GD_PlayerEntity::faceForward(void)
{
	//Node *pShapeNode = get_node("Shape");
	//if (!pShapeNode)
	//	return;
	//((Spatial*)pShapeNode)->look_at(m_facingDirection, Vector3(0, 1, 0));
}

bool GD_PlayerEntity::initEntity(int id, Node* pClientApp)
{
	bool b = entityCommon()->initEntity(id, pClientApp, this);

	if (!b)
		return false;

	add_to_group(GD_CLIENTAPP_PLAYER_GROUP);

	return b;
}

bool GD_PlayerEntity::destroyEntity(void)
{
	bool b = entityCommon()->destroyEntity();

	return b;
}
