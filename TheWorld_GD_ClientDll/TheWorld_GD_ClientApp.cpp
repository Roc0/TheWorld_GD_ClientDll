#include "TheWorld_GD_ClientApp.h"

using namespace godot;

void TheWorld_GD_ClientApp::_register_methods() {
	register_method("_process", &TheWorld_GD_ClientApp::_process);
	//register_property<TheWorld_GD_ClientApp, float>("amplitude", &TheWorld_GD_ClientApp::amplitude, 10.0);
	//register_property<TheWorld_GD_ClientApp, float>("speed", &TheWorld_GD_ClientApp::set_speed, &TheWorld_GD_ClientApp::get_speed, 1.0);

	//register_signal<TheWorld_GD_ClientApp>((char *)"position_changed", "node", GODOT_VARIANT_TYPE_OBJECT, "new_pos", GODOT_VARIANT_TYPE_VECTOR2);
}

TheWorld_GD_ClientApp::TheWorld_GD_ClientApp() {
}

TheWorld_GD_ClientApp::~TheWorld_GD_ClientApp() {
	// add your cleanup here
}

void TheWorld_GD_ClientApp::_init() {
	// initialize any variables here
	/*time_passed = 0.0;
	amplitude = 10.0;
	speed = 1.0;*/
}

void TheWorld_GD_ClientApp::_process(float delta) {
	
	/*time_passed += speed * delta;

	Vector2 new_position = Vector2(
		amplitude + (amplitude * sin(time_passed * 2.0)),
		amplitude + (amplitude * cos(time_passed * 1.5))
	);

	set_position(new_position);

	time_emit += delta;
	if (time_emit > 1.0) {
		emit_signal("position_changed", this, new_position);

		time_emit = 0.0;
	}*/
}

/*void TheWorld_GD_ClientApp::set_speed(float p_speed) {
	speed = p_speed;
}

float TheWorld_GD_ClientApp::get_speed() {
	return speed;
}*/
