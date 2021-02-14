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

		void init(Node* pClientApp, Node* pWorldNode);
		bool isWorldInitialized(void) { return m_isWorldInitialized; }
		Node* getWorldNode(void) { return m_pWorldNode; }
		Node* getWorldCameraNode(void) { return m_pWorldCameraNode; };
		bool enterWorld(void);
		bool exitWorld(void);
		
		//MeshInstance* getMeshInstance(void);
		AABB get_aabbForWorldCameraInitPos(void);

	private:
		Node* m_pClientAppNode;
		MeshInstance* m_pMeshInst;
		GD_WorldCamera* m_pWorldCameraNode;
		Node* m_pWorldNode;
		bool m_isWorldInitialized;
	};

}

