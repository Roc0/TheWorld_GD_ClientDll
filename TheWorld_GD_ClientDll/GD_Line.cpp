#include "GD_Line.h"

using namespace godot;

void GD_Line::_register_methods()
{
	register_method("_ready", &GD_Line::_ready);
	register_method("_process", &GD_Line::_process);
	register_method("_physics_process", &GD_Line::_physics_process);
	register_method("init", &GD_Line::init);
}

GD_Line::GD_Line()
{
}

GD_Line::~GD_Line()
{
}

void GD_Line::_init(void)
{
	//Godot::print("GD_Line::Init");
}

void GD_Line::init(Vector3 start, Vector3 end, Color color, float time)
{
	//Godot::print("GD_Line::Init");

	m_start = start;
	m_end = end;
	m_color = color;
	m_time = time;
}

void GD_Line::_ready(void)
{
	//Godot::print("GD_Line::_ready");
}

void GD_Line::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Line::_process");

}

void GD_Line::_physics_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_Line::_physics_process");
}

