#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite.hpp>

//#include <TheWorld_ClientApp.h>

namespace godot {

class TheWorld_GD_ClientApp : public Node	//, public TheWorld_ClientApp
{
	
	GODOT_CLASS(TheWorld_GD_ClientApp, Node)

public:
	static void _register_methods();

	TheWorld_GD_ClientApp();
	~TheWorld_GD_ClientApp();

	void _init(); // our initializer called by Godot
	void _ready();
	void _process(float _delta);

	void say(String message)
	{
		Godot::print(message);
	}

	String hello(String target1, String target2, int target3)
	{
		return String("Hello, {0} {1} {2}!").format(Array::make(target1, target2, target3));
	}

private:
};

}

#endif
