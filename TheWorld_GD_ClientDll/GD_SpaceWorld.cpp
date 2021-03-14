#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_Entity.h"

#include <Godot.hpp>
#include <Reference.hpp>
#include <ResourceLoader.hpp>
#include <Viewport.hpp>
#include <SceneTree.hpp>
#include <MeshInstance.hpp>
#include <Mesh.hpp>
#include <Script.hpp>
#include <Math.hpp>

using namespace godot;

void GD_SpaceWorld::_register_methods()
{
	register_method("_ready", &GD_SpaceWorld::_ready);
	register_method("_process", &GD_SpaceWorld::_process);
	register_method("_physics_process", &GD_SpaceWorld::_physics_process);
	register_method("_input", &GD_SpaceWorld::_input);
	register_method("enter_world", &GD_SpaceWorld::enterWorld);
	register_method("exit_world", &GD_SpaceWorld::exitWorld);
	//register_method("get_mesh_instance", &GD_SpaceWorld::getMeshInstance);

	register_signal<GD_SpaceWorld>((char*)"active_camera_changed", "", GODOT_VARIANT_TYPE_NIL);
}

GD_SpaceWorld::GD_SpaceWorld()
{
	m_pMeshInst = NULL;
	m_pWorldCameraNode = NULL;
	m_pWorldNode = NULL;
	m_isWorldInitialized = false;
	//m_pWorldCaster = NULL;
	m_pClientAppNode = NULL;
	m_iProgEntityCamera = -1;
	m_isCameraSwitchRequired = false;

	resetDebugEnabled();
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
	//Godot::print("GD_SpaceWorld::Init");
}

void GD_SpaceWorld::_ready()
{
	//Godot::print("GD_SpaceWorld::_ready");
}

void GD_SpaceWorld::_process(float _delta)
{	
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_SpaceWorld::_process");

	if (m_isCameraSwitchRequired)
	{
		m_isCameraSwitchRequired = false;

		GD_WorldCamera* pWorldCamera = (GD_WorldCamera*)getWorldCameraNode();
		if (!pWorldCamera)
			return;

		GD_ClientApp* pClientApp = (GD_ClientApp*)m_pClientAppNode;

		m_iProgEntityCamera++;
		int iNumEntities = pClientApp->getEntityNodeCount();
		if (m_iProgEntityCamera >= iNumEntities)
		{
			m_iProgEntityCamera = -1;

			pWorldCamera->activateCamera();
			//Godot::print("World Camera Activated");
			emit_signal("active_camera_changed");
		}
		else
		{
			GD_Entity* entity = (GD_Entity*)pClientApp->getEntityNodeByIdx(m_iProgEntityCamera);
			if (entity)
			{
				GD_WorldCamera* entityCam = (GD_WorldCamera*)entity->getCameraNode();
				if (entityCam)
				{
					entityCam->activateCamera();
					//Godot::print("Entity Camera Activated");
					emit_signal("active_camera_changed");
				}
			}
		}
	}

	if (isDebugEnabled())
	{
		if (m_isWorldInitialized)
		{
			Node * lineDrawerNode = get_node_or_null(NodePath("/root/DrawLine3d"));
			//Node* lineDrawerNode = get_tree()->get_root()->get_node("DrawLine3d");
			if (lineDrawerNode)
			{
				Color red(128.0 / 255, 25.0 / 255, 0, 255.0 / 255);
				AABB aabb = get_aabbForWorldCameraInitPos();
				lineDrawerNode->call("DrawParallelepiped", aabb.position, aabb.position + aabb.size, red);
			}
		}
	}
	
	resetDebugEnabled();
}

void GD_SpaceWorld::_physics_process(float _delta)
{
	// To activate _physics_process method add this Node to a Godot Scene
	//Godot::print("GD_SpaceWorld::_physics_process");
}

void GD_SpaceWorld::_input(const Ref<InputEvent> event)
{
	if (event->is_action_pressed("ui_focus_next"))
		m_isCameraSwitchRequired = true;
}

void GD_SpaceWorld::init(Node* pClientApp, Node* pWorldNode)
{
	m_pClientAppNode = pClientApp;
	m_pWorldNode = pWorldNode;
	//m_pWorldCaster = (RayCast*)m_pWorldNode->get_node("WorldCaster");
}

bool GD_SpaceWorld::isDebugEnabled(void)
{
	if (m_isDebugEnabled == -1)
	{
		m_isDebugEnabled = ((GD_ClientApp*)m_pClientAppNode)->isDebugEnabled() ? 1 : 0;
	}
	return (m_isDebugEnabled == 1 ? true : false);
}

void GD_SpaceWorld::resetDebugEnabled(void)
{
	m_isDebugEnabled = -1;
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
	bool b = m_pWorldCameraNode->initCameraInWorld(this);
	if (!b)
		return b;

	emit_signal("active_camera_changed");

	if (isDebugEnabled())
	{
		Godot::print("EnterWorld Tree");
		//m_pWorldNode->get_tree()->get_root()->print_tree_pretty();
		m_pWorldNode->print_tree_pretty();
	}
	
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
