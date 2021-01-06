#pragma once

#include <Godot.hpp>
#include <Node.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

class TheWorld_GD_SpaceWorld : public Node
{
	GODOT_CLASS(TheWorld_GD_SpaceWorld, Node)

public:
	static void _register_methods();

	TheWorld_GD_SpaceWorld();
	~TheWorld_GD_SpaceWorld();

	void _init(); // our initializer called by Godot
	void _ready();
	void _process(float _delta);
	
	void say(String message) { Godot::print(message); }

	void setClientApp(Node* pClientApp);
	bool setupWorld(Node* pParent);

private:
	Node* m_pClientApp;
};

}

