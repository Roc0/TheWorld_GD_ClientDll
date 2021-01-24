#include "GD_ClientApp.h"
#include "GD_WorldCamera.h"
#include "Utils.h"
#include <string> 

#include <Math.hpp>
#include <InputEventMouseMotion.hpp>

using namespace godot;

void GD_WorldCamera::_register_methods()
{
	register_method("_ready", &GD_WorldCamera::_ready);
	register_method("_process", &GD_WorldCamera::_process);
	register_method("_physics_process", &GD_WorldCamera::_physics_process);
	register_method("_input", &GD_WorldCamera::_input);
}

GD_WorldCamera::GD_WorldCamera()
{
	// Camera
	m_mouseRelativePos = Vector2(0, 0);
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
	// Camera
}

GD_WorldCamera::~GD_WorldCamera()
{
}

void GD_WorldCamera::_init()
{
	Godot::print("GD_WorldCamera::Init");
}

void GD_WorldCamera::_ready()
{
	Godot::print("GD_WorldCamera::_ready");
}

void GD_WorldCamera::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("TheWorld_GD_SpaceWorld::_process");

	bool b = updateCamera();
}

void GD_WorldCamera::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("TheWorld_GD_SpaceWorld::_physics_process");

	bool b = updateCamera();
}

void GD_WorldCamera::_input(const Ref<InputEvent> event)
{
	//Godot::print("GD_WorldCamera::_input: " + event->as_text());

	InputEventMouseMotion *eventMouseMotion = cast_to<InputEventMouseMotion>(event.ptr());
	if (eventMouseMotion != nullptr)
	{
		//Godot::print("GD_WorldCamera::_input: " + event->as_text());
		m_mouseRelativePos = eventMouseMotion->get_relative();
	}
}

bool GD_WorldCamera::updateCamera()
{
	m_mouseRelativePos *= m_sensitivity;
	m_yaw = m_yaw * m_smoothness + m_mouseRelativePos.x * (1.0 - m_smoothness);
	m_pitch = m_pitch * m_smoothness + m_mouseRelativePos.y * (1.0 - m_smoothness);
	m_mouseRelativePos = Vector2(0, 0);

	if (m_yawLimit < 360)
		m_yaw = clamp(m_yaw, -m_yawLimit - m_totalYaw, m_yawLimit - m_totalYaw);
	if (m_pitchLimit < 360)
		m_pitch = clamp(m_pitch, -m_pitchLimit - m_totalPitch, m_pitchLimit - m_totalPitch);
	m_totalYaw *= m_yaw;
	m_totalPitch *= m_pitch;

	rotate_y(Math::deg2rad(-m_yaw));
	rotate_object_local(Vector3(1, 0, 0), Math::deg2rad(-m_pitch));

	if (m_lastYaw != m_yaw)
		Godot::print("Camera Yaw: " + String(std::to_string(m_yaw).c_str()));

	if (m_lastPitch != m_pitch)
		Godot::print("Camera Pitch: " + String(std::to_string(m_pitch).c_str()));

	m_lastYaw = m_yaw;
	m_lastPitch = m_pitch;

	return true;
}

