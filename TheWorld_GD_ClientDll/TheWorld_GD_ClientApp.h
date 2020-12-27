#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {

class TheWorld_GD_ClientApp : public Node {
	GODOT_CLASS(TheWorld_GD_ClientApp, Node)

private:
	float time_passed;
	float time_emit;
	float amplitude;
	float speed;

public:
	static void _register_methods();

	TheWorld_GD_ClientApp();
	~TheWorld_GD_ClientApp();

	void _init(); // our initializer called by Godot

	void _process(float delta);
	//void set_speed(float p_speed);
	//float get_speed();
};

}

#endif
