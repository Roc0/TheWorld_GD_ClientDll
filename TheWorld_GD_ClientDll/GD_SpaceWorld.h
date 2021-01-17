#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <MeshInstance.hpp>

#include "GD_WorldCamera.h"

#include <TheWorld_ClientApp.h>

namespace godot {

	class GD_SpaceWorld : public Node
	{
		GODOT_CLASS(GD_SpaceWorld, Node)

	public:
		static void _register_methods();

		GD_SpaceWorld();
		~GD_SpaceWorld();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
	
		void say(String message) { Godot::print(message); }

		void setClientApp(Node* pClientApp);
		bool enterWorld(Node* pWorldNode);
		bool exitWorld(Node* pWorldNode);
		
		MeshInstance* getMeshInstance(void);

	private:
		Node* m_pClientApp;
		MeshInstance* m_pMeshInst;
		GD_WorldCamera* m_pWorldCamera;
	};

}

