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
#include <GlobalConstants.hpp >

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
	m_isMovementRequired = false;
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

	char buffer[256];

	GD_ClientApp* pAppNode = (GD_ClientApp*)entityCommon()->getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();
	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	//bool bPlayer;
	//KBEntity* kbentity = pAppNode->getEntityById(entityCommon()->getId(true), bPlayer);
	KBEntity* kbentity = entityCommon()->getEntity();
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

		MeshInstance* entityTail = (MeshInstance*)get_node("Tail");
		MeshInstance* entityBody = (MeshInstance*)get_node("Body");
		MeshInstance* entityHead = (MeshInstance*)get_node("Head");
		if (!entityTail || !entityBody || !entityHead)
		{
			pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
			return;
		}

		Ref<SpatialMaterial> entityBodyMaterial = entityBody->get_surface_material(0);
		if (entityBodyMaterial.ptr())
		{
			Entity_Visuals* ev = pAppNode->getEntityVisuals(GD_CLIENTAPP_ENTITYVISUALS_PLAYER);
			SpatialMaterial* mat = ev->getEntityShapeMaterial(entityBodyMaterial.ptr(), kbentity->getState());
			entityTail->set_material_override(mat);
			entityBody->set_material_override(mat);
			entityHead->set_material_override(mat);

			entityCommon()->setEntityInitializationComplete(true);
		}
		else
		{
			pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
			return;
		}
	}
	// *******************
	// Update Entity Shape
	// *******************

	Vector3 lastPos = entityCommon()->getLastPos();

	Vector3 currentEntityPos;
	kbentity->getForClientPosition(currentEntityPos.x, currentEntityPos.y, currentEntityPos.z);

	// **********************
	// Set Entity Orientation
	// **********************
	if (!isEqualVectorWithLimitedPrecision(m_direction, Vector3Zero, 4))		// se non è richiesto il movimento
	{
		//if (!isEqualVectorWithLimitedPrecision(currentEntityPos, lastPos, 4))		// se il player si è mosso 1
		{
			Vector3 faceTo = currentEntityPos + m_realVelocity;

			//if (!isEqualVectorWithLimitedPrecision(m_realVelocity, Vector3Zero, 4))		// se il player si è mosso 2
			{
				//Vector3 realDirection = faceTo - currentEntityPos;
				Vector3 realDirection = m_realVelocity;
				float angle = realDirection.angle_to(Vector3UP);
				if (!isEqualWithLimitedPrecision(angle, 0, 4) && !isEqualWithLimitedPrecision(angle, 3.1415, 4))		// se la velocità del player non è verticale
				{
					look_at(faceTo, Vector3UP);
					//((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("angle - " + String(std::to_string(angle).c_str()));
					//((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("m_realVelocity - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));
					//((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("m_direction - " + String(std::to_string(m_direction.x).c_str()) + " " + String(std::to_string(m_direction.y).c_str()) + " " + String(std::to_string(m_direction.z).c_str()));
				}
			}
		}
	}

	// DEBUG
	/*if (!isEqualVectorWithLimitedPrecision(m_direction, Vector3Zero, 4))
	{
		float yaw, pitch, roll;
		kbentity->getForClientDirection(yaw, pitch, roll);
		((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("yaw - " + String(std::to_string(yaw).c_str()));
		float angleRealDirectionToXAxiz = m_direction.angle_to(Vector3X);
		float angleRealDirectionToZAxiz = m_direction.angle_to(Vector3Z);
		((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("realDirection - " + String(std::to_string(m_direction.x).c_str()) + " " + String(std::to_string(m_direction.y).c_str()) + " " + String(std::to_string(m_direction.z).c_str()));
		((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("angleRealDirectionToXAxiz - " + String(std::to_string(angleRealDirectionToXAxiz).c_str()));
		((GD_ClientApp*)entityCommon()->getClientAppNode())->debugPrint("angleRealDirectionToZAxiz - " + String(std::to_string(angleRealDirectionToZAxiz).c_str()));
	}*/
	//faceForward();
	// **********************
	// Set Entity Orientation
	// **********************

	//if (currentEntityPos != lastPos)
	if (!isEqualVectorWithLimitedPrecision(currentEntityPos, lastPos, 4))
		entityCommon()->setLastPos(currentEntityPos);

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

			Color black(0, 0, 0, 1);
			float desideredYaw, desideredPitch, desideredRoll;
			kbentity->getForClientDirection(desideredYaw, desideredPitch, desideredRoll);
			Vector3 normalX(1, 0, 0);	Vector3 normalY(0, 1, 0);
			Vector3 yawDirection = normalX.rotated(normalY, desideredYaw - kPi / 2);
			yawDirection.normalize();
			lineDrawerNode->call("DrawRay", entityPos, yawDirection, black, 0.1);
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

	GD_ClientApp* pAppNode = (GD_ClientApp*)entityCommon()->getClientAppNode();
	GD_SpaceWorld* pSpaceWorldNode = (GD_SpaceWorld*)pAppNode->getSpaceWorldNode();
	if (!pSpaceWorldNode->isWorldInitialized())
		return;

	KBEntity* kbentity = entityCommon()->getEntity();
	if (!kbentity)
	{
		pAppNode->setAppInError(GD_CLIENTAPP_ERROR_ENTITY_PROCESS);
		return;
	}

	// *******************
	// Set Entity Position
	// *******************
	if (m_initPositionFromServer)		// Initital Position
	{
		entityCommon()->CalcPositionOnGround(this);
		float x, y, z;
		kbentity->getForClientPosition(x, y, z);
		Transform t;
		t = get_transform();
		t.origin = Vector3(x, y, z);
		set_transform(t);

		/*float x, y, z;
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
		}*/

		m_initPositionFromServer = false;
	}

	move(_delta);
	// *******************
	// Set Entity Position
	// *******************
}

void GD_PlayerEntity::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_PlayerEntity::_input: " + event->as_text());
}

godot::Vector2 GD_PlayerEntity::get_2d_movement(void)
{
	godot::Vector2 desideredMovement;

	Input* input = Input::get_singleton();

	m_isMovementRequired = false;

	if (input->is_action_pressed("ui_up") && !input->is_action_pressed("ui_down") && !input->is_key_pressed(GlobalConstants::KEY_SHIFT) && !input->is_key_pressed(GlobalConstants::KEY_CONTROL) && !input->is_key_pressed(GlobalConstants::KEY_ALT))
	{
		desideredMovement.y = -1;
		m_facingDirectionAngle = 0;
		m_isMovementRequired = true;;
	}
	if (input->is_action_pressed("ui_down") && !input->is_action_pressed("ui_up") && !input->is_key_pressed(GlobalConstants::KEY_SHIFT) && !input->is_key_pressed(GlobalConstants::KEY_CONTROL) && !input->is_key_pressed(GlobalConstants::KEY_ALT))
	{
		desideredMovement.y = 1;
		m_facingDirectionAngle = kPi;
	}

	if (input->is_action_pressed("ui_left") && !input->is_action_pressed("ui_right") && !input->is_key_pressed(GlobalConstants::KEY_SHIFT) && !input->is_key_pressed(GlobalConstants::KEY_CONTROL) && !input->is_key_pressed(GlobalConstants::KEY_ALT))
	{
		desideredMovement.x = -1;
		m_facingDirectionAngle = kPi * 0.5;
	}
	if (input->is_action_pressed("ui_right") && !input->is_action_pressed("ui_left") && !input->is_key_pressed(GlobalConstants::KEY_SHIFT) && !input->is_key_pressed(GlobalConstants::KEY_CONTROL) && !input->is_key_pressed(GlobalConstants::KEY_ALT))
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

void GD_PlayerEntity::move(float _delta)
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

	m_realVelocity = move_and_slide(m_velocity, Vector3UP, true);
	/*if (m_velocity == m_realVelocity)
		Godot::print("UGUALI  - " + String(std::to_string(m_velocity.x).c_str()) + " " + String(std::to_string(m_velocity.y).c_str()) + " " + String(std::to_string(m_velocity.z).c_str()) + " - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));
	else
		Godot::print("DIVERSI - " + String(std::to_string(m_velocity.x).c_str()) + " " + String(std::to_string(m_velocity.y).c_str()) + " " + String(std::to_string(m_velocity.z).c_str()) + " - " + String(std::to_string(m_realVelocity.x).c_str()) + " " + String(std::to_string(m_realVelocity.y).c_str()) + " " + String(std::to_string(m_realVelocity.z).c_str()));*/

	KBEntity* kbentity = entityCommon()->getEntity();

	Transform t;
	t = get_transform();
	kbentity->setForClientPosition(t.origin.x, t.origin.y, t.origin.z);

	float yaw, pitch, roll;
	kbentity->getForClientDirection(yaw, pitch, roll);
	float angleRealDirectionToZAxiz = m_direction.angle_to(Vector3Z);
	kbentity->setForClientDirection(angleRealDirectionToZAxiz, pitch, roll);
}

void GD_PlayerEntity::faceForward(void)
{
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
