#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"

#include <ResourceLoader.hpp>
#include <MeshInstance.hpp>
#include <Mesh.hpp>

using namespace godot;

void GD_SpaceWorld::_register_methods()
{
	register_method("say", &GD_SpaceWorld::say);
	register_method("setup_world", &GD_SpaceWorld::setupWorld);
}

GD_SpaceWorld::GD_SpaceWorld()
{
}

GD_SpaceWorld::~GD_SpaceWorld()
{
}

void GD_SpaceWorld::_init()
{
	Godot::print("GD_SpaceWorld::Init");
}

void GD_SpaceWorld::_ready()
{
	Godot::print("GD_SpaceWorld::_ready");
}

void GD_SpaceWorld::_process(float _delta)
{	
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_SpaceWorld::_process");
}

void GD_SpaceWorld::setClientApp(Node* pClientApp)
{
	m_pClientApp = pClientApp;
}

bool GD_SpaceWorld::setupWorld(Node *pWorldNode)
{
	SpaceWorld* pSpaceWorld = ((GD_ClientApp*)m_pClientApp)->getSpaceWorld();
	if (!pSpaceWorld)
		return false;

	Space* pSpace = pSpaceWorld->findSpace();
	if (!pSpace)
		return false;

	m_pMesh = MeshInstance::_new();
	if (m_pMesh)
		pWorldNode->add_child(m_pMesh);
	else
		return false;

	m_pMesh->set_name("TerrainMesh");

	String path; path = path + "res://Meshes/" + pSpace->getResPath() + "/undulating1.obj";
	ResourceLoader* resLoader = ResourceLoader::get_singleton();
	Ref<Mesh> mesh = resLoader->load(path);
	m_pMesh->set_mesh(mesh);
	
	m_aabb = m_pMesh->get_aabb();

	m_pWorldCamera = GD_WorldCamera::_new();
	if (m_pWorldCamera)
		pWorldNode->add_child(m_pWorldCamera);
	else
		return false;

	m_pWorldCamera->set_name("WorldCamera");
	m_pWorldCamera->set_zfar(1000);

	//m_pWorldCamera->set_pos


	return true;
}
