#include "GD_ClientApp.h"
#include "GD_WorldCamera.h"

using namespace godot;

void GD_WorldCamera::_register_methods()
{
	//register_method("say", &GD_SpaceWorld::say);
	//register_method("setup_world", &GD_SpaceWorld::setupWorld);
}

GD_WorldCamera::GD_WorldCamera()
{
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
}
