#include "TheWorld_GD_ClientApp.h"

using namespace godot;

void TheWorld_GD_ClientApp::_register_methods() {
	register_method("_ready", &TheWorld_GD_ClientApp::_ready);
	register_method("_process", &TheWorld_GD_ClientApp::_process);
	register_method("say", &TheWorld_GD_ClientApp::say);
	register_method("hello", &TheWorld_GD_ClientApp::hello);
	register_method("setAppMode", &TheWorld_GD_ClientApp::setAppMode);
}

TheWorld_GD_ClientApp::TheWorld_GD_ClientApp()
{
}

TheWorld_GD_ClientApp::~TheWorld_GD_ClientApp()
{
}

void TheWorld_GD_ClientApp::_init()
{
	Godot::print("TheWorld_GD_ClientApp::Init");
}

void TheWorld_GD_ClientApp::_ready()
{
	Godot::print("TheWorld_GD_ClientApp::_ready");
}
	
void TheWorld_GD_ClientApp::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("TheWorld_GD_ClientApp::_process");
}

void TheWorld_GD_ClientApp::setAppMode(int r, bool bForce)
{
	TheWorld_ClientApp::setAppMode((TheWorld_ClientApp::_AppMode)r, bForce);
}

