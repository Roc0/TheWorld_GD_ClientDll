#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite.hpp>

//****************************************
/*
#include "math/math.h"

#include "G3D/g3dmath.h"
#include "G3D/Vector2.h"
#include "G3D/Vector3.h"
#include "G3D/Vector4.h"
#include "G3D/Matrix3.h"
#include "G3D/Quat.h"

#include "G3D/platform.h"
#include "G3D/g3dmath.h"
#include "G3D/Vector3.h"
#include "G3D/Matrix3.h"

#include "G3D/System.h"
#include "G3D/Vector3.h"
#include "G3D/Vector4.h"

#include "G3D/platform.h"
#ifdef _DEBUG
#pragma comment (linker, "/NODEFAULTLIB:libc.lib")
#pragma comment (linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment (linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment (linker, "/NODEFAULTLIB:libcd.lib")
#pragma comment (linker, "/NODEFAULTLIB:msvcrtd.lib")
#else
#pragma comment(linker, "/NODEFAULTLIB:LIBC.LIB")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#endif

#include "TheWorld_ClientDll.h"
#include "client_lib/event.h"
#include <boost/algorithm/string.hpp>
#include <boost/thread/thread.hpp>

#include "SpaceWorld.h"
#include "Entity.h"
#include "PlayerEntity.h"
#include "OtherEntity.h"
#include "Avatar.h"

#include <string>
#include <queue>
//typedef std::map<int, std::shared_ptr<KBEntity> > ENTITIES;
//typedef std::map<__int64, std::shared_ptr<KBAvatar> > AVATARS;
*/
//****************************************

#include <TheWorld_ClientApp.h>

namespace godot {

class TheWorld_GD_ClientApp : public Node, public TheWorld_ClientApp
{
	
	GODOT_CLASS(TheWorld_GD_ClientApp, Node)

public:
	static void _register_methods();

	TheWorld_GD_ClientApp();
	~TheWorld_GD_ClientApp();

	void _init(); // our initializer called by Godot
	void _ready();
	void _process(float _delta);

	void say(String message)
	{
		Godot::print(message);
	}

	String hello(String target1, String target2, int target3)
	{
		return String("Hello, {0} {1} {2}!").format(Array::make(target1, target2, target3));
	}

	void setAppMode(int r, bool bForce = false);
	int  getAppMode1(void);
	void setLoginStatus(int i);
	int  getLoginStatus(void);
	bool kbengine_Init(void);
	void kbengine_Destroy(void);
	bool kbengine_Login(String accountName, String passwd, String datas, String ip, int port);
	bool kbengine_Logout(void);
	void kbengine_MessagePump(void);
	bool getShutdownRequired(void);
	bool getDoSleepInMainLoop(void);
	int getPlayerID(void);
	bool enterGame(__int64 avatarId);
	bool createAvatar(String avatarName);
	bool removeAvatar(String avatarName);

	// AVATAR
	int getAvatarsCount(void);
	__int64 getAvatarIdByIdx(int idx);
	String getAvatarNameByIdx(int idx);
	String getAvatarNameById(int id);

	// ENTITY
	int getEntitiesCount(void);
	__int64 getEntityIdByIdx(int idx);
	String getEntityNameById(int id);

	void onLoginSuccess(void);
	void onLoginFailed(int failCode);
	void onServerClosed(void);
	void onKicked(int failCode);
	void onUpdateAvatars(void);
	void onPlayerEnterSpace(KBEngine::SPACE_ID spaceId);
	void onPlayerLeaveSpace(KBEngine::SPACE_ID spaceId);
	void onAddSpaceGeoMapping(KBEngine::SPACE_ID spaceId, const char* resPath);

private:

};

}

#endif
