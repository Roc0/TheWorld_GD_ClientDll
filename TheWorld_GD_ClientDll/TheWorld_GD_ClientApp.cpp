#include "TheWorld_GD_ClientApp.h"

using namespace godot;

void TheWorld_GD_ClientApp::_register_methods() {
	register_method("_ready", &TheWorld_GD_ClientApp::_ready);
	register_method("_process", &TheWorld_GD_ClientApp::_process);
	register_method("say", &TheWorld_GD_ClientApp::say);
	register_method("hello", &TheWorld_GD_ClientApp::hello);

	register_method("set_app_mode", &TheWorld_GD_ClientApp::setAppMode);
	register_method("get_app_mode", &TheWorld_GD_ClientApp::getAppMode1);
	register_method("set_login_status", &TheWorld_GD_ClientApp::setLoginStatus);
	register_method("get_login_status", &TheWorld_GD_ClientApp::getLoginStatus);
	register_method("init", &TheWorld_GD_ClientApp::kbengine_Init);
	register_method("destroy", &TheWorld_GD_ClientApp::kbengine_Destroy);
	register_method("login", &TheWorld_GD_ClientApp::kbengine_Login);
	register_method("logout", &TheWorld_GD_ClientApp::kbengine_Logout);
	register_method("message_pump", &TheWorld_GD_ClientApp::kbengine_MessagePump);
	register_method("get_do_sleep_in_main_loop", &TheWorld_GD_ClientApp::getDoSleepInMainLoop);
	register_method("get_shutdown_required", &TheWorld_GD_ClientApp::getShutdownRequired);
	register_method("get_avatar_count", &TheWorld_GD_ClientApp::getAvatarsCount);
	register_method("get_avatar_id", &TheWorld_GD_ClientApp::getAvatarID);
	register_method("get_avatar_name", &TheWorld_GD_ClientApp::getAvatarName);

	//register_signal<TheWorld_GD_ClientApp>((char*)"login_success", "node", GODOT_VARIANT_TYPE_OBJECT, "value", GODOT_VARIANT_TYPE_INT);
	register_signal<TheWorld_GD_ClientApp>((char*)"login_success", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<TheWorld_GD_ClientApp>((char*)"login_failed", "fail_code", GODOT_VARIANT_TYPE_INT);
	register_signal<TheWorld_GD_ClientApp>((char*)"server_closed", "", GODOT_VARIANT_TYPE_NIL);
	register_signal<TheWorld_GD_ClientApp>((char*)"kicked_from_server", "", GODOT_VARIANT_TYPE_NIL);
}

TheWorld_GD_ClientApp::TheWorld_GD_ClientApp()
{
}

TheWorld_GD_ClientApp::~TheWorld_GD_ClientApp()
{
}

void TheWorld_GD_ClientApp::_init()
{
	Godot::print("TheWorld_GD_ClientApp::Init");
}

void TheWorld_GD_ClientApp::_ready()
{
	Godot::print("TheWorld_GD_ClientApp::_ready");
	//get_node(NodePath("/root/Main/Reset"))->connect("pressed", this, "on_Reset_pressed");
}
	
void TheWorld_GD_ClientApp::_process(float _delta)
{
	// To activate _process method add this Node to a Godot Scene
	//Godot::print("TheWorld_GD_ClientApp::_process");
}

void TheWorld_GD_ClientApp::setAppMode(int r, bool bForce)
{
	TheWorld_ClientApp::setAppMode((TheWorld_ClientApp::_AppMode)r, bForce);
}

int TheWorld_GD_ClientApp::getAppMode1(void)
{
	return (int)TheWorld_ClientApp::getAppMode();
}

void TheWorld_GD_ClientApp::setLoginStatus(int i)
{
	TheWorld_ClientApp::setLoginStatus(i);
}

int  TheWorld_GD_ClientApp::getLoginStatus(void)
{
	return TheWorld_ClientApp::getLoginStatus();
}

bool TheWorld_GD_ClientApp::kbengine_Init(void)
{
	std::string s1 = getenv("KBE_ROOT");
	std::string s2 = getenv("KBE_RES_PATH");
	std::string s3 = getenv("KBE_BIN_PATH");

	return TheWorld_ClientApp::kbengine_Init();
}

void TheWorld_GD_ClientApp::kbengine_Destroy(void)
{
	TheWorld_ClientApp::kbengine_Destroy();
}

//bool TheWorld_GD_ClientApp::kbengine_Login(const char* accountname, const char* passwd, const char* datas, KBEngine::uint32 datasize, const char* ip, KBEngine::uint32 port)
bool TheWorld_GD_ClientApp::kbengine_Login(String accountname, String passwd, String data, String ip, int port)
{
	char* _name = accountname.length() == 0 ? NULL : accountname.alloc_c_string();
	char* _pwd = passwd.length() == 0 ? NULL : passwd.alloc_c_string();
	char* _d = data.length() == 0 ? NULL : data.alloc_c_string();
	KBEngine::uint32 _dl = data.length();
	char* _ip = ip.length() == 0 ? NULL : ip.alloc_c_string();
	return TheWorld_ClientApp::kbengine_Login(_name, _pwd, _d, _dl, _ip, port);
}

bool TheWorld_GD_ClientApp::kbengine_Logout(void)
{
	return TheWorld_ClientApp::kbengine_Logout();
}

void TheWorld_GD_ClientApp::kbengine_MessagePump(void)
{
	TheWorld_ClientApp::kbengine_MessagePump(this);
}

bool TheWorld_GD_ClientApp::getShutdownRequired(void)
{
	return TheWorld_ClientApp::getShutdownRequired();
}

bool TheWorld_GD_ClientApp::getDoSleepInMainLoop(void)
{
	return TheWorld_ClientApp::getDoSleepInMainLoop();
}

void TheWorld_GD_ClientApp::onLoginSuccess(void)
{
	emit_signal("login_success");
}

void TheWorld_GD_ClientApp::onLoginFailed(int failCode)
{
	emit_signal("login_failed", failCode);
}

void TheWorld_GD_ClientApp::onServerClosed(void)
{
	emit_signal("server_closed");
}

void TheWorld_GD_ClientApp::onKicked(int failCode)
{
	emit_signal("kicked_from_server");
}

int TheWorld_GD_ClientApp::getAvatarsCount(void)
{
	return getAvatars().size();
}

__int64 TheWorld_GD_ClientApp::getAvatarID(int idx)
{
	int size = getAvatarsCount();

	if (idx >= size)
		return 0;

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
		return 0;
}

String TheWorld_GD_ClientApp::getAvatarName(int idx)
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
