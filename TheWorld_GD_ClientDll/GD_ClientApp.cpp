#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"
#include "GD_PlayerEntity.h"
#include "GD_OtherEntity.h"

#include <Godot.hpp>
#include <Reference.hpp>
#include <SceneTree.hpp>
#include <Position3d.hpp>
#include <Viewport.hpp>

using namespace godot;

void GD_ClientApp::_register_methods()
{
	register_method("_ready", &GD_ClientApp::_ready);
	register_method("_process", &GD_ClientApp::_process);
	register_method("_input", &GD_ClientApp::_input);
	register_method("debug_print", &GD_ClientApp::debugPrint);
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
	register_method("set_debug_enabled", &GD_ClientApp::setDebugEnabled);
	register_method("is_debug_enabled", &GD_ClientApp::isDebugEnabled);
	register_method("set_edit_mode", &GD_ClientApp::setEditMode);
	register_method("is_edit_mode", &GD_ClientApp::isEditMode);

	// AVATAR
	register_method("get_avatar_count", &GD_ClientApp::getAvatarsCount);
	register_method("get_avatar_id_by_idx", &GD_ClientApp::getAvatarIdByIdx);
	register_method("get_avatar_name_by_idx", &GD_ClientApp::getAvatarNameByIdx);
	register_method("get_avatar_name_by_id", &GD_ClientApp::getAvatarNameById);

	// ENTITY
	register_method("get_entity_count", &GD_ClientApp::getEntitiesCount);
	register_method("get_entity_id_by_idx", &GD_ClientApp::getEntityIdByIdx);
	register_method("get_entity_name_by_id", &GD_ClientApp::getEntityNameById);
	register_method("get_hp_by_id", &GD_ClientApp::getHPById);
	register_method("get_mp_by_id", &GD_ClientApp::getMPById);

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
	m_pSpaceWorldNode = NULL;
	m_bAppInError = false;
	m_erroCodeApp = 0;
	//m_iProgEntityCamera = -1;
	m_pPlayer_EntityVisuals = NULL;
	m_pNPC_EntityVisuals = NULL;
	m_pMonster_EntityVisuals = NULL;
	m_isDebugEnabled = false;
	m_isEditMode = false;
}

GD_ClientApp::~GD_ClientApp()
{
	if (m_pPlayer_EntityVisuals)
	{
		delete m_pPlayer_EntityVisuals;
		m_pPlayer_EntityVisuals = NULL;
	}

	if (m_pNPC_EntityVisuals)
	{
		delete m_pNPC_EntityVisuals;
		m_pNPC_EntityVisuals = NULL;
	}

	if (m_pMonster_EntityVisuals)
	{
		delete m_pMonster_EntityVisuals;
		m_pMonster_EntityVisuals = NULL;
	}

	if (m_pSpaceWorldNode)
	{
		//m_pSpaceWorld->queue_free();
		m_pSpaceWorldNode->call_deferred("free");
		m_pSpaceWorldNode = NULL;
	}
}

void GD_ClientApp::_init(void)
{
	//Godot::print("GD_ClientApp::Init");
}

void GD_ClientApp::_ready()
{
	//Godot::print("GD_ClientApp::_ready");
	//get_node(NodePath("/root/Main/Reset"))->connect("pressed", this, "on_Reset_pressed");
}
	
void GD_ClientApp::_input(const Ref<InputEvent> event)
{
	if (event->is_action_pressed("ui_debug_print"))
	{
		Godot::print("*** Scene Tree Begin ***");
		Godot::print("==>");
		//m_pWorldNode->get_tree()->get_root()->print_tree_pretty();
		get_tree()->get_root()->print_tree_pretty();
		Godot::print("<==");
		Godot::print("*** Scene Tree End ***");
	}
	
	if (event->is_action_pressed("ui_toggle_debug_mode"))
	{
		setDebugEnabled(!isDebugEnabled());
	}

	if (event->is_action_pressed("ui_relive"))
	{
		bool bPlayer;
		KBEntity* pPlayer = getEntityById(getPlayerID(), bPlayer);
		if (pPlayer)
		{
			if(pPlayer->getState() == ENTITY_STATE_DEAD)
				kbengine_Relive();
		}
	}
}

void GD_ClientApp::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("GD_ClientApp::_process");
}

Node* GD_ClientApp::getActiveCamera(void)
{
	GD_SpaceWorld*pWorldNode = (GD_SpaceWorld*)getSpaceWorldNode();
	if (!pWorldNode)
		return NULL;

	GD_WorldCamera* pWorldCamera = (GD_WorldCamera*)pWorldNode->getWorldCameraNode();
	if (!pWorldCamera)
		return NULL;

	return pWorldCamera->getActiveCamera();
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

bool GD_ClientApp::kbengine_Init(Node* pWorldNode, Node* pMainNode)
{
	pMainNode->add_child(this);
	
	m_pSpaceWorldNode = GD_SpaceWorld::_new();
	if (m_pSpaceWorldNode)
		pWorldNode->add_child(m_pSpaceWorldNode);
	else
		return false;

	m_pSpaceWorldNode->set_name("SpaceWorld");

	((GD_SpaceWorld*)m_pSpaceWorldNode)->init(this, pWorldNode);
	
	std::string s1 = getenv("KBE_ROOT");
	std::string s2 = getenv("KBE_RES_PATH");
	std::string s3 = getenv("KBE_BIN_PATH");

	setAppMode(TheWorld_ClientApp::InitialMenu);
	return TheWorld_ClientApp::kbengine_Init();
}

void GD_ClientApp::kbengine_Destroy(void)
{
	TheWorld_ClientApp::kbengine_Destroy();

	if (m_pSpaceWorldNode)
	{
		//m_pSpaceWorld->queue_free();
		m_pSpaceWorldNode->call_deferred("free");
		m_pSpaceWorldNode = NULL;
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
	bool bret = TheWorld_ClientApp::kbengine_Login(_name, _pwd, _d, _dl, _ip, port);
	godot::api->godot_free(_name);
	godot::api->godot_free(_pwd);
	godot::api->godot_free(_d);
	godot::api->godot_free(_ip);
	return bret;
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

int GD_ClientApp::getHPById(int id)
{
	bool bPlayer;
	KBEntity* pEntity = getEntity((KBEngine::ENTITY_ID)id, bPlayer);
	if (pEntity)
		return pEntity->getHP();
	else
		return -1;
}

int GD_ClientApp::getMPById(int id)
{
	bool bPlayer;
	KBEntity* pEntity = getEntity((KBEngine::ENTITY_ID)id, bPlayer);
	if (pEntity)
		return pEntity->getMP();
	else
		return -1;
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

	char* _avatarName = avatarName.alloc_c_string();
	bool bret = kbengine_CreateAvatar(std::string(_avatarName));
	godot::api->godot_free(_avatarName);

	return bret;
}

bool GD_ClientApp::removeAvatar(String avatarName)
{
	if (avatarName.length() == 0)
		return false;

	char* _avatarName = avatarName.alloc_c_string();
	bool bret = kbengine_RemoveAvatar(std::string(_avatarName));
	godot::api->godot_free(_avatarName);

	return bret;
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
	if (!m_pSpaceWorldNode)
		return;

	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorldNode)->getWorldNode();
	
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
			GD_PlayerEntity* pPlayer = (GD_PlayerEntity*)pNode;
			String nodeName = GD_CLIENTAPP_PLAYER_ENTITY_NODE;
			char buffer[16];
			nodeName = nodeName + "_" + _itoa(pPlayer->entityCommon()->getId(), buffer, 10);
			pNode->set_name(nodeName);
			if (m_isDebugEnabled)
				Godot::print("onCreatedEntity (renamed): " + pNode->get_name() + " - " + pPlayer->entityCommon()->getEntityName());
			pPlayer->destroyEntity();
			pNode->call_deferred("free");

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
			String entityName = pPlayer->entityCommon()->getEntityName();
			char buffer[16]; _itoa((int)eid, buffer, 10);
			if (m_isDebugEnabled)
			{
				String s = "onCreatedEntity (";	s = s + buffer + "): " + pPlayer->get_name() + " - " + pPlayer->entityCommon()->getEntityName();
				Godot::print(s);
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
			String entityName = pOther->entityCommon()->getEntityName();
			char buffer[16]; _itoa((int)eid, buffer, 10);
			if (m_isDebugEnabled)
			{
				String s = "onCreatedEntity (";	s = s + buffer + "): " + pOther->get_name() + " - " + pOther->entityCommon()->getEntityName();
				Godot::print(s);
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
	Node* entity = getEntityNodeById(eid);
	if (entity)
	{
		GD_Entity_Common* pEntityCommon = NULL;
		GET_ENTITY_COMMON(pEntityCommon, entity);
		if (m_isDebugEnabled)
		{
			char buffer[16]; _itoa(pEntityCommon->getId(), buffer, 10);
			String s = "destroyEntity(";	s = s + buffer + "): " + entity->get_name() + " - " + pEntityCommon->getEntityName();
			Godot::print(s);
		}
		pEntityCommon->destroyEntity();
		entity->call_deferred("free");

		emit_signal("erase_entity", (int)eid);
	}
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

void GD_ClientApp::onEntityEnterWorld(KBEngine::ENTITY_ID eid)
{
}

void GD_ClientApp::onEntityLeaveWorld(KBEngine::ENTITY_ID eid)
{
}

void GD_ClientApp::onEntityEnterSpace(KBEngine::ENTITY_ID eid, KBEngine::SPACE_ID spaceId)
{
}

void GD_ClientApp::onEntityLeaveSpace(KBEngine::ENTITY_ID eid, KBEngine::SPACE_ID spaceId)
{
}

void GD_ClientApp::onPlayerEnterSpace(KBEngine::ENTITY_ID eid, KBEngine::SPACE_ID spaceId)
{
	emit_signal("player_enter_space", (int)spaceId);
}

void GD_ClientApp::onPlayerLeaveSpace(KBEngine::ENTITY_ID eid, KBEngine::SPACE_ID spaceId)
{
	emit_signal("player_leave_space", (int)spaceId);
}

void GD_ClientApp::onAddSpaceGeoMapping(KBEngine::SPACE_ID spaceId, const char* resPath)
{
	emit_signal("add_space_geomapping", (int)spaceId, resPath);
}

void GD_ClientApp::onMaxHPChanged(KBEngine::ENTITY_ID eid, int MaxHP)
{
	Node* entity = getEntityNodeById(eid);
	if (entity)
	{
		GD_Entity_Common* pEntityCommon = NULL;
		GET_ENTITY_COMMON(pEntityCommon, entity);
		pEntityCommon->setMaxHP(MaxHP);
	}
}

void GD_ClientApp::onMaxMPChanged(KBEngine::ENTITY_ID eid, int MaxMP)
{
	Node* entity = getEntityNodeById(eid);
	if (entity)
	{
		GD_Entity_Common* pEntityCommon = NULL;
		GET_ENTITY_COMMON(pEntityCommon, entity);
		pEntityCommon->setMaxMP(MaxMP);
	}
}

void GD_ClientApp::onHPChanged(KBEngine::ENTITY_ID eid, int HP)
{
	Node* entity = getEntityNodeById(eid);
	if (entity)
	{
		GD_Entity_Common* pEntityCommon = NULL;
		GET_ENTITY_COMMON(pEntityCommon, entity);
		pEntityCommon->setHP(HP);
	}
}

void GD_ClientApp::onMPChanged(KBEngine::ENTITY_ID eid, int MP)
{
	Node* entity = getEntityNodeById(eid);
	if (entity)
	{
		GD_Entity_Common* pEntityCommon = NULL;
		GET_ENTITY_COMMON(pEntityCommon, entity);
		pEntityCommon->setMP(MP);
	}
}

void GD_ClientApp::onRecvDamage(KBEngine::ENTITY_ID eid, KBEngine::ENTITY_ID attacker, int skillID, int damageType, int damage)
{
}

void GD_ClientApp::onAttackDamage(KBEngine::ENTITY_ID eid, KBEngine::ENTITY_ID receiver, int skillID, int damageType, int damage)
{
}

Node* GD_ClientApp::getSpaceWorldNode(void)
{
	//Ref<Node> refSpaceWorld = Ref<Node>(m_pSpaceWorld);

	return m_pSpaceWorldNode;
}

Node* GD_ClientApp::getPlayerNode(bool bIgnoreValid)
{

	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorldNode)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	Node* pEntitiesNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);

	Node* pNode = pEntitiesNode->get_node_or_null(GD_CLIENTAPP_PLAYER_ENTITY_NODE);
	if (pNode)
	{
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(pNode);
		if (pPlayerEntityNode != nullptr)
		{
			if (bIgnoreValid)
				return pPlayerEntityNode;
			else
				if (pPlayerEntityNode->entityCommon()->isValid())
					return pPlayerEntityNode;
		}
	}

	return NULL;
}

int GD_ClientApp::getEntityNodeCount(void)
{
	Array entities;
	return getEntityNodes(entities);
}

int GD_ClientApp::getEntityNodes(Array& entities)
{
	SceneTree* scene = get_tree();
	if (scene == NULL)
		return 0;

	entities = scene->get_nodes_in_group(GD_CLIENTAPP_ENTITIES_GROUP);

	return entities.size();
		
	/*Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorldNode)->getWorldNode();
	if (!pWorldNode)
		return 0;

	Node* pNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	if (!pNode)
		return 0;

	return pNode->get_child_count();*/
}

Node* GD_ClientApp::getEntityNodeByIdx(int idx, bool bIgnoreValid)
{
	Array entityNodes;
	int entitiesSize = getEntityNodes(entityNodes);

	if (idx >= entitiesSize)
		return NULL;

	/*Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorldNode)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	//SceneTree* pSceneTree = pWorldNode->get_tree();
	Node* pNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	if (!pNode)
		return NULL;

	Array entityNodes = pNode->get_children();*/

	int idxEntity = 0;
	for (int i = 0; i < entityNodes.size(); i++)
	{
		Node* pEntityNode = entityNodes[i];
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(pEntityNode);
		GD_OtherEntity* pOtherEntityNode = cast_to<GD_OtherEntity>(pEntityNode);
		if (pOtherEntityNode != nullptr || pPlayerEntityNode != nullptr)
		{
			if (bIgnoreValid)
			{
				if (idxEntity == idx)
					return pEntityNode;
				else
					idxEntity++;
			}
			else
			{
				GD_Entity_Common* pEntityCommon = NULL;
				GET_ENTITY_COMMON(pEntityCommon, pEntityNode);
				if (pEntityCommon->isValid())
				{
					if (idxEntity == idx)
						return pEntityNode;
					else
						idxEntity++;
				}
			}
		}
	}

	return NULL;
}

Node* GD_ClientApp::getEntityNodeById(int id, bool bIgnoreValid)
{
	Node* pWorldNode = ((GD_SpaceWorld*)m_pSpaceWorldNode)->getWorldNode();
	if (!pWorldNode)
		return NULL;

	//SceneTree* pSceneTree = pWorldNode->get_tree();
	Node* pNode = pWorldNode->get_node(GD_CLIENTAPP_ENTITIES_CONTAINER_NODE);
	if (!pNode)
		return NULL;

	Array entityNodes = pNode->get_children();
	for (int i = 0; i < entityNodes.size(); i++)
	{
		Node* pEntityNode = entityNodes[i];
		GD_PlayerEntity* pPlayerEntityNode = cast_to<GD_PlayerEntity>(pEntityNode);
		GD_OtherEntity* pOtherEntityNode = cast_to<GD_OtherEntity>(pEntityNode);
		if (pOtherEntityNode != nullptr || pPlayerEntityNode != nullptr)
		{
			GD_Entity_Common* pEntityCommon = NULL;
			GET_ENTITY_COMMON(pEntityCommon, pEntityNode);
			if (pEntityCommon->getId(bIgnoreValid) == id)
				return pEntityNode;
		}
	}

	return NULL;
}

Entity_Visuals* GD_ClientApp::getEntityVisuals(int iType)
{
	Entity_Visuals* pVisuals = NULL;
	
	switch (iType)
	{
	case GD_CLIENTAPP_ENTITYVISUALS_PLAYER:
		if (!m_pPlayer_EntityVisuals)
			m_pPlayer_EntityVisuals = new Entity_Visuals(GD_CLIENTAPP_ENTITYVISUALS_PLAYER);
		pVisuals = m_pPlayer_EntityVisuals;
		break;
	case GD_CLIENTAPP_ENTITYVISUALS_NPC:
		if (!m_pNPC_EntityVisuals)
			m_pNPC_EntityVisuals = new Entity_Visuals(GD_CLIENTAPP_ENTITYVISUALS_NPC);
		pVisuals = m_pNPC_EntityVisuals;
		break;
	case GD_CLIENTAPP_ENTITYVISUALS_MONSTER:
		if (!m_pMonster_EntityVisuals)
			m_pMonster_EntityVisuals = new Entity_Visuals(GD_CLIENTAPP_ENTITYVISUALS_MONSTER);
		pVisuals = m_pMonster_EntityVisuals;
		break;
	default:
		return NULL;
		break;
	}

	return pVisuals;
}