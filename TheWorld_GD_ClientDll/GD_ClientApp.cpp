#include "GD_ClientApp.h"
#include "GD_SpaceWorld.h"

#include <Reference.hpp>

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

	register_method("get_space_world", &GD_ClientApp::getGDSpaceWorld);

	//register_signal<GD_ClientApp>((char*)"login_success", "node", GODOT_VARIANT_TYPE_OBJECT, "value", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"login_success", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"login_failed", "fail_code", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"server_closed", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"kicked_from_server", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"update_avatars", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<GD_ClientApp>((char*)"player_enter_space", "space_id", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"player_leave_space", "space_id", GODOT_VARIANT_TYPE_INT);
	register_signal<GD_ClientApp>((char*)"add_space_geomapping", "space_id", GODOT_VARIANT_TYPE_INT, "res_path", GODOT_VARIANT_TYPE_STRING);
}

GD_ClientApp::GD_ClientApp()
{
	m_pSpaceWorld = NULL;
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

void GD_ClientApp::_init()
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

bool GD_ClientApp::kbengine_Init(void)
{
	m_pSpaceWorld = GD_SpaceWorld::_new();
	if (!m_pSpaceWorld)
		return false;

	((GD_SpaceWorld*)m_pSpaceWorld)->setClientApp(this);
	
	std::string s1 = getenv("KBE_ROOT");
	std::string s2 = getenv("KBE_RES_PATH");
	std::string s3 = getenv("KBE_BIN_PATH");

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

__int64 GD_ClientApp::getAvatarIdByIdx(int idx)
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

__int64 GD_ClientApp::getEntityIdByIdx(int idx)
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

bool GD_ClientApp::enterGame(__int64 avatarId)
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

Node* GD_ClientApp::getGDSpaceWorld(void)
{
	//Ref<Node> refSpaceWorld = Ref<Node>(m_pSpaceWorld);

	return m_pSpaceWorld;
}
