#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_PlayerEntity.h"
#include "GD_OtherEntity.h"

#include <Reference.hpp>
#include <SceneTree.hpp>

using namespace godot;

void GD_ClientApp::_register_methods()
{
	register_method("_ready", &GD_ClientApp::_ready);
	register_method("_process", &GD_ClientApp::_process);
	register_method("say", &GD_ClientApp::say);
	register_method("hello", &GD_ClientApp::hello);

	register_method("set_app_mode", &GD_ClientApp::setAppMode);
	register_method("get_app_mode", &GD_ClientApp::getAppMode1);
	register_method("set_login_status", &GD_ClientApp::setLoginStatus);
	register_method("get_login_status", &GD_ClientApp::getLoginStatus);
	register_method("init", &GD_ClientApp::kbengine_Init);
	register_method("destroy", &GD_ClientApp::kbengine_Destroy);
	register_method("login", &GD_ClientApp::kbengine_Login);
	register_method("logout", &GD_ClientApp::kbengine_Logout);
	register_method("message_pump", &GD_ClientApp::kbengine_MessagePump);
	register_method("get_do_sleep_in_main_loop", &GD_ClientApp::getDoSleepInMainLoop);
	register_method("get_shutdown_required", &GD_ClientApp::getShutdownRequired);
	register_method("get_player_id", &GD_ClientApp::getPlayerID);
	register_method("create_avatar", &GD_ClientApp::createAvatar);
	register_method("remove_avatar", &GD_ClientApp::removeAvatar);
	register_method("enter_game", &GD_ClientApp::enterGame);

	// AVATAR
	register_method("get_avatar_count", &GD_ClientApp::getAvatarsCount);
	register_method("get_avatar_id_by_idx", &GD_ClientApp::getAvatarIdByIdx);
	register_method("get_avatar_name_by_idx", &GD_ClientApp::getAvatarNameByIdx);
	register_method("get_avatar_name_by_id", &GD_ClientApp::getAvatarNameById);

	// ENTITY
	register_method("get_entity_count", &GD_ClientApp::getEntitiesCount);
	register_method("get_entity_id_by_idx", &GD_ClientApp::getEntityIdByIdx);
	register_method("get_entity_name_by_id", &GD_ClientApp::getEntityNameById);

	register_method("get_space_world", &GD_ClientApp::getSpaceWorldNode);

	//register_signal<GD_ClientApp>((char*)"login_success", "node", GODOT_VARIANT_TYPE_OBJECT, "value", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"login_success", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"login_failed", "fail_code", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"server_closed", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"kicked_from_server", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"clear_entities", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"created_entity", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"erase_entity", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"clear_avatars", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"erase_avatar", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"update_avatars", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"player_enter_space", "space_id", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"player_leave_space", "space_id", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"add_space_geomapping", "space_id", GODOT_VARIANT_TYPE_INT, "res_path", GODOT_VARIANT_TYPE_STRING);
}

GD_ClientApp::GD_ClientApp()
{
	m_pSpaceWorld = NULL;
	m_bAppInError = false;
	m_erroCodeApp = 0;
}

GD_ClientApp::~GD_ClientApp()
{
	if (m_pSpaceWorld)
	{
		//m_pSpaceWorld->queue_free();
		m_pSpaceWorld->call_deferred("free");
		m_pSpaceWorld = NULL;
	}
}

void GD_ClientApp::_init(void)
{
	Godot::print("GD_ClientApp::Init");
}

void GD_ClientApp::_ready()
{
	Godot::print("GD_ClientApp::_ready");
	//get_node(NodePath("/root/Main/Reset"))->connect("pressed", this, "on_Reset_pressed");
}
	
void GD_ClientApp::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_ClientApp::_process");
}

void GD_ClientApp::setAppMode(int r, bool bForce)
{
	TheWorld_ClientApp::setAppMode((TheWorld_ClientApp::_AppMode)r, bForce);
}

int GD_ClientApp::getAppMode1(void)
{
	return (int)TheWorld_ClientApp::getAppMode();
}

void GD_ClientApp::setLoginStatus(int i)
{
	TheWorld_ClientApp::setLoginStatus(i);
}

int  GD_ClientApp::getLoginStatus(void)
{
	return TheWorld_ClientApp::getLoginStatus();
}

bool GD_ClientApp::kbengine_Init(Node* pWorldNode)
{
	m_pSpaceWorld = GD_SpaceWorld::_new();
	if (m_pSpaceWorld)
		pWorldNode->add_child(m_pSpaceWorld);
	else
		return false;

	((GD_SpaceWorld*)m_pSpaceWorld)->init(this, pWorldNode);
	
	std::string s1 = getenv("KBE_ROOT");
	std::string s2 = getenv("KBE_RES_PATH");
	std::string s3 = getenv("KBE_BIN_PATH");

	setAppMode(TheWorld_ClientApp::InitialMenu);
	return TheWorld_ClientApp::kbengine_Init();
}

void GD_ClientApp::kbengine_Destroy(void)
{
	TheWorld_ClientApp::kbengine_Destroy();

	if (m_pSpaceWorld)
	{
		//m_pSpaceWorld->queue_free();
		m_pSpaceWorld->call_deferred("free");
		m_pSpaceWorld = NULL;
	}

}

//bool GD_ClientApp::kbengine_Login(const char* accountName, const char* passwd, const char* datas, KBEngine::uint32 datasize, const char* ip, KBEngine::uint32 port)
bool GD_ClientApp::kbengine_Login(String accountName, String passwd, String data, String ip, int port)
{
	char* _name = accountName.length() == 0 ? NULL : accountName.alloc_c_string();
	char* _pwd = passwd.length() == 0 ? NULL : passwd.alloc_c_string();
	char* _d = data.length() == 0 ? NULL : data.alloc_c_string();
	KBEngine::uint32 _dl = data.length();
	char* _ip = ip.length() == 0 ? NULL : ip.alloc_c_string();
	return TheWorld_ClientApp::kbengine_Login(_name, _pwd, _d, _dl, _ip, port);
}

bool GD_ClientApp::kbengine_Logout(void)
{
	setAppMode(TheWorld_ClientApp::InitialMenu);
	clearEntities();
	clearAvatars();
	return TheWorld_ClientApp::kbengine_Logout();
}

void GD_ClientApp::kbengine_MessagePump(void)
{
	TheWorld_ClientApp::kbengine_MessagePump(this);
}

bool GD_ClientApp::getShutdownRequired(void)
{
	return TheWorld_ClientApp::getShutdownRequired();
}

bool GD_ClientApp::getDoSleepInMainLoop(void)
{
	return TheWorld_ClientApp::getDoSleepInMainLoop();
}

int GD_ClientApp::getAvatarsCount(void)
{
	return getAvatars().size();
}

int GD_ClientApp::getAvatarIdByIdx(int idx)
{
	int size = getAvatarsCount();

	if (idx >= size)
		return -1;

	KBAvatar* pAvatar = NULL;
	AVATARS& avatars = getAvatars();
	AVATARS::iterator iter = avatars.begin();
	int i = 0;
	for (; iter != avatars.end(); iter++)
	{
		if (i == idx)
		{
			pAvatar = iter->second.get();
			break;
		}
		i++;
	}
	if (pAvatar)
		return pAvatar->getAvatarID();
	else
		return -1;
}

String GD_ClientApp::getAvatarNameByIdx(int idx)
{
	int size = getAvatarsCount();

	if (idx >= size)
		return "";

	KBAvatar* pAvatar = NULL;
	AVATARS& avatars = getAvatars();
	AVATARS::iterator iter = avatars.begin();
	int i = 0;
	for (; iter != avatars.end(); iter++)
	{
		if (i == idx)
		{
			pAvatar = iter->second.get();
			break;
		}
		i++;
	}
	if (pAvatar)
		return pAvatar->getAvatarName();
	else
		return "";
}

String GD_ClientApp::getAvatarNameById(int id)
{
	KBAvatar* pAvatar = getAvatar((KBEngine::DBID)id);
	if (pAvatar)
		return pAvatar->getAvatarName();
	else
		return "";
}

int GD_ClientApp::getEntitiesCount(void)
{
	return getEntities().size();
}

int GD_ClientApp::getEntityIdByIdx(int idx)
{
	int size = getEntitiesCount();

	if (idx >= size)
		return -1;

	KBEntity* pEntity = NULL;
	ENTITIES& entitis = getEntities();
	ENTITIES::iterator iter = entitis.begin();
	int i = 0;
	for (; iter != entitis.end(); iter++)
	{
		if (i == idx)
		{
			pEntity = iter->second.get();
			break;
		}
		i++;
	}
	if (pEntity)
		return pEntity->id();
	else
		return -1;
}

KBEntity* GD_ClientApp::getEntityById(int id, bool& bPlayer)
{
	return getEntity((KBEngine::ENTITY_ID)id, bPlayer);
}

String GD_ClientApp::getEntityNameById(int id)
{
	bool bPlayer;
	KBEntity* pEntity = getEntity((KBEngine::ENTITY_ID)id, bPlayer);
	if (pEntity)
		return pEntity->getName();
	else
		return "";
}

int GD_ClientApp::getPlayerID(void)
{
	return kbengine_PlayerID();
}

bool GD_ClientApp::enterGame(int avatarId)
{
	return kbengine_SelectAvatarGame((KBEngine::DBID)avatarId);
}

bool GD_ClientApp::createAvatar(String avatarName)
{
	if (avatarName.length() == 0)
		return false;

	std::string s = avatarName.alloc_c_string();

	return kbengine_CreateAvatar(s);
}

bool GD_ClientApp::removeAvatar(String avatarName)
{
	if (avatarName.length() == 0)
		return false;

	std::string s = avatarName.alloc_c_string();

	return kbengine_RemoveAvatar(s);
}

void GD_ClientApp::onLoginSuccess(void)
{
	emit_signal("login_success");
}

void GD_ClientApp::onLoginFailed(int failCode)
{
	emit_signal("login_failed", failCode);
}

void GD_ClientApp::onServerClosed(void)
{
	emit_signal("server_closed");
}

void GD_ClientApp::onKicked(int failCode)
{
	emit_signal("kicked_from_server");
}

void GD_ClientApp::onClearEntities(void)
{
	int iNumEntities = getEntitiesCount();
	for (int i = 0; i < iNumEntities; i++)
	{
		int id = getEntityIdByIdx(i);
		if (id >= 0)
			onEraseEntity(id);
	}
	
	emit_signal("clear_entities");
}

void GD_ClientApp::onCreatedEntity(KBEngine::ENTITY_ID eid, bool bPlayer)
{
	if (!m_pSpaceWorld)
		return;

	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorld)->getWorldNode();
	
	String sEntitiesPath = GD_CLIENTAPP_ENTITIES_CONTAINER_NODE;
	NodePath entitiesPath(sEntitiesPath);
	Node* entitiesNode = pWorldNode->get_node(entitiesPath);
	if (!entitiesNode)
	{
		entitiesNode = Spatial::_new();
		if (!entitiesNode)
		{
			setAppInError(GD_CLIENTAPP_ERROR_CREATE_OTHER_ENTITY_CONTAINER);
			return;
		}
		pWorldNode->add_child(entitiesNode);
		entitiesNode->set_name(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	}

	if (bPlayer)
	{
		Node* pNode = getPlayerNode();
		if (pNode)
		{
			
			String nodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;
			char buffer[16];
			nodeName = nodeName + "_" + _itoa(((GD_Entity*)pNode)->get_id(), buffer, 10);
			pNode->set_name(nodeName);
		}

		GD_PlayerEntity* pPlayer = GD_PlayerEntity::_new();
		if (pPlayer)
		{
			entitiesNode->add_child(pPlayer);
			bool b = pPlayer->initEntity((int)eid, this);
			if (!b)
			{
				setAppInError(GD_CLIENTAPP_ERROR_INIT_NODE_ENTITY);
				return;
			}
		}
		else
		{
			setAppInError(GD_CLIENTAPP_ERROR_CREATE_PLAYER_ENTITY);
			return;
		}
	}
	else
	{
		GD_OtherEntity* pOther = GD_OtherEntity::_new();
		if (pOther)
		{
			entitiesNode->add_child(pOther);
			bool b = pOther->initEntity((int)eid, this);
			if (!b)
			{
				setAppInError(GD_CLIENTAPP_ERROR_INIT_NODE_ENTITY);
				return;
			}
		}
		else
		{
			setAppInError(GD_CLIENTAPP_ERROR_CREATE_OTHER_ENTITY);
			return;
		}
	}
	
	emit_signal("created_entity", (int)eid, bPlayer);
}

void GD_ClientApp::onEraseEntity(KBEngine::ENTITY_ID eid)
{
	GD_Entity* entity = (GD_Entity*)getEntityNode(eid);
	if (entity)
	{
		entity->destroyEntity();
		entity->call_deferred("free");
	}
	
	emit_signal("erase_entity", (int)eid);
}

void GD_ClientApp::onClearAvatars(void)
{
	emit_signal("clear_avatars");
}

void GD_ClientApp::onEraseAvatar(KBEngine::DBID dbid)
{
	emit_signal("erase_avatar", (int)dbid);
}


void GD_ClientApp::onUpdateAvatars(void)
{
	emit_signal("update_avatars");
}

void GD_ClientApp::onPlayerEnterSpace(KBEngine::SPACE_ID spaceId)
{
	emit_signal("player_enter_space", (int)spaceId);
}

void GD_ClientApp::onPlayerLeaveSpace(KBEngine::SPACE_ID spaceId)
{
	emit_signal("player_leave_space", (int)spaceId);
}

void GD_ClientApp::onAddSpaceGeoMapping(KBEngine::SPACE_ID spaceId, const char* resPath)
{
	emit_signal("add_space_geomapping", (int)spaceId, resPath);
}

Node* GD_ClientApp::getSpaceWorldNode(void)
{
	//Ref<Node> refSpaceWorld = Ref<Node>(m_pSpaceWorld);

	return m_pSpaceWorld;
}

Node* GD_ClientApp::getPlayerNode(void)
{

	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorld)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	Node* pEntitiesNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);

	Node* pNode = pEntitiesNode->get_node(GD_CLIENTAPP_PLAYER_ENTITY_NODE);
	if (pNode)
	{
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(pNode);
		if (pPlayerEntityNode != nullptr && pPlayerEntityNode->isValid())
		{
			return pPlayerEntityNode;
		}
	}

	return NULL;
}

Node* GD_ClientApp::getEntityNode(int id)
{
	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorld)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	//SceneTree* pSceneTree = pWorldNode->get_tree();
	Node* pNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	Array entityNodes = pNode->get_children();
	for (int i = 0; i < entityNodes.size(); i++)
	{
		GD_Entity* pEntityNode = entityNodes[i];
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(pNode);
		GD_OtherEntity* pOtherEntityNode = cast_to<GD_OtherEntity>(pNode);
		if (pOtherEntityNode != nullptr || pPlayerEntityNode != nullptr)
		{
			if (pEntityNode->get_id() == id)
				return pEntityNode;
		}
	}

	return NULL;
}