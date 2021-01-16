#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_WorldCamera.h"

//using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);
	godot::register_class<godot::GD_ClientApp>();
	godot::register_class<godot::GD_SpaceWorld>();
	godot::register_class<godot::GD_WorldCamera>();
}
