#include "TheWorld_GD_ClientApp.h"
#include "TheWorld_GD_SpaceWorld.h"

#include <ResourceLoader.hpp>
#include <MeshInstance.hpp>
#include <Mesh.hpp>

using namespace godot;

void TheWorld_GD_SpaceWorld::_register_methods()
{
	register_method("say", &TheWorld_GD_SpaceWorld::say);
	register_method("setup_world", &TheWorld_GD_SpaceWorld::setupWorld);
}

TheWorld_GD_SpaceWorld::TheWorld_GD_SpaceWorld()
{
}

TheWorld_GD_SpaceWorld::~TheWorld_GD_SpaceWorld()
{
}

void TheWorld_GD_SpaceWorld::_init()
{
	Godot::print("TheWorld_GD_SpaceWorld::Init");
}

void TheWorld_GD_SpaceWorld::_ready()
{
	Godot::print("TheWorld_GD_SpaceWorld::_ready");
}

void TheWorld_GD_SpaceWorld::_process(float _delta)
{	
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("TheWorld_GD_SpaceWorld::_process");
}

void TheWorld_GD_SpaceWorld::setClientApp(Node* pClientApp)
{
	m_pClientApp = pClientApp;
}

bool TheWorld_GD_SpaceWorld::setupWorld(Node *pParent)
{
	SpaceWorld* pSpaceWorld = ((TheWorld_GD_ClientApp*)m_pClientApp)->getSpaceWorld();
	if (!pSpaceWorld)
		return false;

	Space* pSpace = pSpaceWorld->findSpace();
	if (!pSpace)
		return false;

	ResourceLoader* reLo = ResourceLoader::get_singleton();

	String path; path = path + "res://Meshes/" + pSpace->getResPath() + "/undulating1.obj";
	
	MeshInstance* pMesh = MeshInstance::_new();
	if (!pMesh)
		return false;

	Ref<Mesh> mesh = reLo->load(path);

	pMesh->set_name("TerrainMesh");
	pMesh->set_mesh(mesh);

	pParent->add_child(pMesh);

	return true;
}
