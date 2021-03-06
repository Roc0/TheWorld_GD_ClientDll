#include "GD_ClientApp.h"
#include "GD_WorldCamera.h"
#include "GD_SpaceWorld.h"
#include "Utils.h"
#include <string> 

#include <Godot.hpp>
#include <Reference.hpp>
#include <SceneTree.hpp>
#include <Math.hpp>
#include <Input.hpp>
#include <InputEventMouseMotion.hpp>

using namespace godot;

void GD_WorldCamera::_register_methods()
{
	register_method("_ready", &GD_WorldCamera::_ready);
	register_method("_process", &GD_WorldCamera::_process);
	register_method("_physics_process", &GD_WorldCamera::_physics_process);
	register_method("_input", &GD_WorldCamera::_input);
	register_method("is_active_camera", &GD_WorldCamera::isActiveCamera);
	register_method("get_active_camera", &GD_WorldCamera::getActiveCamera);
}

GD_WorldCamera::GD_WorldCamera()
{
	m_pSpaceWorldNode = NULL;
	m_isActive = false;
	m_instanceId = -1;
	m_PlayerCamera = false;
	m_OtherEntityCamera = false;
	m_WorldCamera = false;
	m_updateCameraRequired = false;
	
	// Camera Movement
	m_numMoveStep = 0;
	m_wheelVelocity = 5.0;
	// Camera Movement

	// Camera Rotation
	m_shiftOriCameraOn = false;
	m_shiftVertCameraOn = false;
	m_rotateCameraOn = false;
	m_mouseRelativePosToShiftOriz = Vector2(0, 0);
	m_mouseRelativePosToShiftVert = Vector2(0, 0);
	m_mouseRelativePosToRotate = Vector2(0, 0);
	m_sensitivity = 0.5;
	m_smoothness = 0.5;
	m_yaw = 0.0;
	m_lastYaw = m_yaw;
	m_totalYaw = 0.0;
	m_yawLimit = 360;
	m_pitch = 0.0;
	m_lastPitch = m_pitch;
	m_totalPitch = 0.0;
	m_pitchLimit = 360;
	// Camera Rotation

	resetDebugEnabled();
}

GD_WorldCamera::~GD_WorldCamera()
{
}

void GD_WorldCamera::_init()
{
	//Godot::print("GD_WorldCamera::Init");

	m_instanceId = get_instance_id();
}

void GD_WorldCamera::_ready()
{
	//Godot::print("GD_WorldCamera::_ready");
}

void GD_WorldCamera::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_WorldCamera::_process");

	if (!m_isActive)
	{
		return;
	}
	
	bool b = updateCamera();

	resetDebugEnabled();
}

void GD_WorldCamera::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_WorldCamera::_physics_process");

	if (!m_isActive)
	{
		return;
	}

	Input* input = Input::get_singleton();
	if (input->is_action_pressed("ui_mouse_button_right"))
		m_rotateCameraOn = true;
	else
		m_rotateCameraOn = false;
	
	input = Input::get_singleton();
	if (input->is_action_pressed("ui_mouse_button_left"))
		m_shiftOriCameraOn = true;
	else
		m_shiftOriCameraOn = false;

	input = Input::get_singleton();
	if (input->is_action_pressed("ui_mouse_button_mid"))
		m_shiftVertCameraOn = true;
	else
		m_shiftVertCameraOn = false;
}

void GD_WorldCamera::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_WorldCamera::_input: " + event->as_text());

	if (!m_isActive)
	{
		return;
	}

	InputEventMouseMotion *eventMouseMotion = cast_to<InputEventMouseMotion>(event.ptr());
	if (eventMouseMotion != nullptr)
	{
		//Godot::print("GD_WorldCamera::_input: " + event->as_text());
		if (m_rotateCameraOn)
		{
			m_mouseRelativePosToRotate = eventMouseMotion->get_relative();
			m_updateCameraRequired = true;
		}
		if (m_shiftOriCameraOn)
		{
			m_mouseRelativePosToShiftOriz = eventMouseMotion->get_relative();
			m_updateCameraRequired = true;
		}
		if (m_shiftVertCameraOn)
		{
			m_mouseRelativePosToShiftVert = eventMouseMotion->get_relative();
			m_updateCameraRequired = true;
		}
	}

	if (event->is_action_pressed("ui_mouse_wheel_up"))
	{
		m_numMoveStep++;
		m_updateCameraRequired = true;
	}

	if (event->is_action_pressed("ui_mouse_wheel_down"))
	{
		m_numMoveStep--;
		m_updateCameraRequired = true;
	}
}

bool GD_WorldCamera::isDebugEnabled(void)
{
	if (m_isDebugEnabled == -1)
	{
		m_isDebugEnabled = ((GD_ClientApp*)((GD_SpaceWorld*)m_pSpaceWorldNode)->getClientAppNode())->isDebugEnabled() ? 1 : 0;
	}
	return (m_isDebugEnabled == 1 ? true : false);
}

void GD_WorldCamera::resetDebugEnabled(void)
{
	m_isDebugEnabled = -1;
}


void GD_WorldCamera::activateCamera(void)
{
	GD_WorldCamera* activeCamera = (GD_WorldCamera*)getActiveCamera();
	if (activeCamera)
	{
		activeCamera->set_process_input(false);
		activeCamera->set_process(false);
		activeCamera->set_physics_process(false);
		activeCamera->remove_from_group(GD_ACTIVE_CAMERA_GROUP);
		m_isActive = false;
	}

	make_current();
	set_process_input(true);
	set_process(true);
	set_physics_process(true);
	add_to_group(GD_ACTIVE_CAMERA_GROUP);
	m_isActive = true;
}

bool GD_WorldCamera::isActiveCamera(void)
{
	return m_isActive;
	//return is_in_group(GD_ACTIVE_CAMERA_GROUP);
}

Node* GD_WorldCamera::getActiveCamera(void)
{
	SceneTree* scene = get_tree();
	if (scene == NULL)
		return NULL;
	
	Array camArray = scene->get_nodes_in_group(GD_ACTIVE_CAMERA_GROUP);

	if (camArray.size() > 0)
		return camArray[0];
	else
		return NULL;
}

bool GD_WorldCamera::initPlayerCamera(void)
{
	set_name("Camera");

	m_PlayerCamera = true;

	return true;
}

bool GD_WorldCamera::initOtherEntityCamera(void)
{
	set_name("Camera");

	m_OtherEntityCamera = true;

	return true;
}


bool GD_WorldCamera::initCameraInWorld(Node* pSpaceWorld)
{
	m_pSpaceWorldNode = pSpaceWorld;

	set_name("WorldCamera");

	activateCamera();

	AABB aabb = ((GD_SpaceWorld*)m_pSpaceWorldNode)->get_aabbForWorldCameraInitPos();
	Vector3 aabb_start = aabb.position;
	Vector3 aabb_end = aabb.position + aabb.size;

	real_t zNear = 1.0;
	real_t zFar = (aabb_end.z > 900 ? aabb_end.z + 100 : 1000);
	real_t fov = 45.0;
	set_perspective(fov, zNear, zFar);

	float offsetFromCenterOfAABB = sqrtf(pow(aabb.size.x, 2) + pow(aabb.size.y, 2) + pow(aabb.size.z, 2)) / 2;
	Vector3 cameraPos((aabb_end.x + aabb_start.x) / 2 + offsetFromCenterOfAABB, (aabb_end.y + aabb_start.y) / 2 + offsetFromCenterOfAABB, (aabb_end.z + aabb_start.z) / 2 + offsetFromCenterOfAABB);
	Vector3 lookAt = aabb.position + aabb.size / 2;

	look_at_from_position(cameraPos, lookAt, Vector3(0, 1, 0));

	m_WorldCamera = true;

	return true;
}

bool GD_WorldCamera::updateCamera()
{
	if (isPlayerCamera() || isOtherEntityCamera())
		return true;

	if (!m_updateCameraRequired)
		return true;

	m_updateCameraRequired = false;
	
	if (m_mouseRelativePosToRotate != Vector2(0, 0))
	{
		m_mouseRelativePosToRotate *= m_sensitivity;
		m_yaw = m_yaw * m_smoothness + m_mouseRelativePosToRotate.x * (1.0 - m_smoothness);
		m_pitch = m_pitch * m_smoothness + m_mouseRelativePosToRotate.y * (1.0 - m_smoothness);
		m_mouseRelativePosToRotate = Vector2(0, 0);

		if (m_yawLimit < 360)
			m_yaw = clamp(m_yaw, -m_yawLimit - m_totalYaw, m_yawLimit - m_totalYaw);
		if (m_pitchLimit < 360)
			m_pitch = clamp(m_pitch, -m_pitchLimit - m_totalPitch, m_pitchLimit - m_totalPitch);
		m_totalYaw *= m_yaw;
		m_totalPitch *= m_pitch;

		float pitch = Math::deg2rad(-m_pitch);
		if ((pitch + get_rotation().x) > kPi / 2)
			pitch = (kPi / 2) - get_rotation().x;
		if ((pitch + get_rotation().x) < -(kPi / 2))
			pitch = (-(kPi / 2)) - get_rotation().x;

		rotate_y(Math::deg2rad(-m_yaw));
		rotate_object_local(Vector3(1, 0, 0), pitch);

		/*if (m_lastYaw != m_yaw)
			Godot::print("Camera Yaw: " + String(std::to_string(m_yaw).c_str()));
		if (m_lastPitch != m_pitch)
			Godot::print("Camera Pitch: " + String(std::to_string(m_pitch).c_str()));*/

		m_lastYaw = m_yaw;
		m_lastPitch = m_pitch;
	}

	if (m_mouseRelativePosToShiftOriz != Vector2(0, 0))
	{
		m_mouseRelativePosToShiftOriz *= m_sensitivity;

		translate_object_local(Vector3(-m_mouseRelativePosToShiftOriz.x, 0, 0));

		m_mouseRelativePosToShiftOriz = Vector2(0, 0);
	}
		
	if (m_mouseRelativePosToShiftVert != Vector2(0, 0))
	{
		m_mouseRelativePosToShiftVert *= m_sensitivity;

		translate_object_local(Vector3(0, 0, -m_mouseRelativePosToShiftVert.y));

		m_mouseRelativePosToShiftVert = Vector2(0, 0);
	}

	if (m_numMoveStep)
	{
		translate_object_local(Vector3(0, 0, m_numMoveStep * m_wheelVelocity));
		m_numMoveStep = 0;
	}

	return true;
}

