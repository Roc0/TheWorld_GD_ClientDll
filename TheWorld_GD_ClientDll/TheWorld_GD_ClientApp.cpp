#include "TheWorld_GD_ClientApp.h"

using namespace godot;

void TheWorld_GD_ClientApp::_register_methods() {
	register_method("_ready", &TheWorld_GD_ClientApp::_ready);
	register_method("_process", &TheWorld_GD_ClientApp::_process);
	register_method("say", &TheWorld_GD_ClientApp::say);
	register_method("hello", &TheWorld_GD_ClientApp::hello);

	register_method("setAppMode", &TheWorld_GD_ClientApp::setAppMode);
	register_method("getAppMode", &TheWorld_GD_ClientApp::getAppMode1);
	register_method("setLoginStatus", &TheWorld_GD_ClientApp::setLoginStatus);
	register_method("getLoginStatus", &TheWorld_GD_ClientApp::getLoginStatus);
	register_method("Init", &TheWorld_GD_ClientApp::kbengine_Init);
	register_method("Destroy", &TheWorld_GD_ClientApp::kbengine_Destroy);
	register_method("Login", &TheWorld_GD_ClientApp::kbengine_Login);
	register_method("Logout", &TheWorld_GD_ClientApp::kbengine_Logout);
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

