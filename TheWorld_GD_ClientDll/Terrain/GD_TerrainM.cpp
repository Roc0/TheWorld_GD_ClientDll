#include "GD_TerrainM.h"

using namespace godot;

void GD_TerrainM::_register_methods()
{
	register_method("_ready", &GD_TerrainM::_ready);
	register_method("_process", &GD_TerrainM::_process);
}

GD_TerrainM::GD_TerrainM()
{
}

GD_TerrainM::~GD_TerrainM()
{
}

void GD_TerrainM::_init(void)
{
	//Godot::print("GD_TerrainM::Init");
}

void GD_TerrainM::_ready()
{
	//Godot::print("GD_TerrainM::_ready");
}

void GD_TerrainM::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_TerrainM::_process");
}
