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
	register_method("get_mesh_instance", &GD_SpaceWorld::getMeshInstance);
}

GD_SpaceWorld::GD_SpaceWorld()
{
	m_pMeshInst = NULL;
	m_pWorldCamera = NULL;
}

GD_SpaceWorld::~GD_SpaceWorld()
{
	if (m_pMeshInst)
	{
		m_pMeshInst->call_deferred("free");
		m_pMeshInst = NULL;
	}
	if (m_pWorldCamera)
	{
		m_pWorldCamera->call_deferred("free");
		m_pWorldCamera = NULL;
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

void GD_SpaceWorld::setClientApp(Node* pClientApp)
{
	m_pClientApp = pClientApp;
}

MeshInstance* GD_SpaceWorld::getMeshInstance(void)
{
	return m_pMeshInst;
}

bool GD_SpaceWorld::enterWorld(Node *pWorldNode)
{
	SpaceWorld* pSpaceWorld = ((GD_ClientApp*)m_pClientApp)->getSpaceWorld();
	if (!pSpaceWorld)
		return false;

	Space* pSpace = pSpaceWorld->findSpace();
	if (!pSpace)
		return false;

	ResourceLoader* resLoader = ResourceLoader::get_singleton();

	m_pMeshInst = MeshInstance::_new();
	if (m_pMeshInst)
		pWorldNode->add_child(m_pMeshInst);
	else
		return false;

	m_pMeshInst->set_name("TerrainMesh");

	String path; path = path + "res://Meshes/" + pSpace->getResPath() + "/undulating1.obj";
	const Ref<Mesh> mesh = resLoader->load(path);
	m_pMeshInst->set_mesh(mesh);
	
	m_pWorldCamera = GD_WorldCamera::_new();
	if (m_pWorldCamera)
		pWorldNode->add_child(m_pWorldCamera);
	else
		return false;

	m_pWorldCamera->set_name("WorldCamera");
	m_pWorldCamera->make_current();

	AABB aabb = m_pMeshInst->get_aabb();
	Vector3 aabb_start = aabb.position;
	Vector3 aabb_end = aabb.position + aabb.size;

	real_t zNear = 1.0;
	real_t zFar = (aabb_end.z > 900 ? aabb_end.z + 100 : 1000);
	real_t fov = 45.0;
	m_pWorldCamera->set_perspective(fov, zNear, zFar);
	//m_pWorldCamera->set_zfar(zFar);

	float offsetFromCenterOfAABB = sqrtf(pow(aabb.size.x, 2) + pow(aabb.size.y, 2) + pow(aabb.size.z, 2)) / 2;
	Vector3 cameraPos((aabb_end.x + aabb_start.x) / 2 + offsetFromCenterOfAABB, (aabb_end.y + aabb_start.y) / 2 + offsetFromCenterOfAABB, (aabb_end.z + aabb_start.z) / 2 + offsetFromCenterOfAABB);

	Transform t;	t.origin = cameraPos;
	m_pWorldCamera->set_transform(t);

	return true;
}

bool GD_SpaceWorld::exitWorld(Node* pWorldNode)
{
	if (m_pMeshInst)
	{
		m_pMeshInst->call_deferred("free");
		m_pMeshInst = NULL;
	}
	if (m_pWorldCamera)
	{
		m_pWorldCamera->call_deferred("free");
		m_pWorldCamera = NULL;
	}

	return true;
}
