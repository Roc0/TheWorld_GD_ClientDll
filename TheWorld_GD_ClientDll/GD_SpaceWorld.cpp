#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"

#include <ResourceLoader.hpp>
#include <MeshInstance.hpp>
#include <Mesh.hpp>
#include <Script.hpp>
#include <Math.hpp>

using namespace godot;

void GD_SpaceWorld::_register_methods()
{
	register_method("say", &GD_SpaceWorld::say);
	register_method("enter_world", &GD_SpaceWorld::enterWorld);
	register_method("exit_world", &GD_SpaceWorld::exitWorld);
	//register_method("get_mesh_instance", &GD_SpaceWorld::getMeshInstance);
}

GD_SpaceWorld::GD_SpaceWorld()
{
	m_pMeshInst = NULL;
	m_pWorldCameraNode = NULL;
	m_pWorldNode = NULL;
	m_isWorldInitialized = false;
}

GD_SpaceWorld::~GD_SpaceWorld()
{
	if (m_pMeshInst)
	{
		m_pMeshInst->call_deferred("free");
		m_pMeshInst = NULL;
	}
	if (m_pWorldCameraNode)
	{
		m_pWorldCameraNode->call_deferred("free");
		m_pWorldCameraNode = NULL;
	}
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

void GD_SpaceWorld::init(Node* pClientApp, Node* pWorldNode)
{
	m_pClientAppNode = pClientApp;
	m_pWorldNode = pWorldNode;
}

/*MeshInstance* GD_SpaceWorld::getMeshInstance(void)
{
	return m_pMeshInst;
}*/

AABB GD_SpaceWorld::get_aabbForWorldCameraInitPos()
{
	return m_pMeshInst->get_aabb();
}

bool GD_SpaceWorld::enterWorld(void)
{
	SpaceWorld* pSpaceWorld = ((GD_ClientApp*)m_pClientAppNode)->getSpaceWorld();
	if (!pSpaceWorld)
		return false;

	Space* pSpace = pSpaceWorld->findSpace();
	if (!pSpace)
		return false;

	ResourceLoader* resLoader = ResourceLoader::get_singleton();

	m_pMeshInst = MeshInstance::_new();
	if (m_pMeshInst)
		m_pWorldNode->add_child(m_pMeshInst);
	else
		return false;

	m_pMeshInst->set_name("TerrainMesh");

	String path; path = path + "res://Meshes/" + pSpace->getResPath() + "/undulating1.obj";
	const Ref<Mesh> mesh = resLoader->load(path);
	m_pMeshInst->set_mesh(mesh);

	m_pMeshInst->create_trimesh_collision();
	
	m_pWorldCameraNode = GD_WorldCamera::_new();
	if (m_pWorldCameraNode)
		m_pWorldNode->add_child(m_pWorldCameraNode);
	else
		return false;

	m_pWorldCameraNode->set_visible(false);
	bool b = m_pWorldCameraNode->initCamera(this);
	if (!b)
		return b;

	m_isWorldInitialized = true;
	
	return true;
}

bool GD_SpaceWorld::exitWorld(void)
{
	m_isWorldInitialized = false;
	
	if (m_pMeshInst)
	{
		m_pMeshInst->call_deferred("free");
		m_pMeshInst = NULL;
	}
	if (m_pWorldCameraNode)
	{
		m_pWorldCameraNode->call_deferred("free");
		m_pWorldCameraNode = NULL;
	}

	return true;
}
