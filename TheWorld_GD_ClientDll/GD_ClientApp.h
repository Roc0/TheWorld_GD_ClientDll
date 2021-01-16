#pragma once

#include <Godot.hpp>
#include <Node.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

	class GD_ClientApp : public Node, public TheWorld_ClientApp
	{
	
		GODOT_CLASS(GD_ClientApp, Node)

	public:
		static void _register_methods();

		GD_ClientApp();
		~GD_ClientApp();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);

		void say(String message) {	Godot::print(message);	}
		String hello(String target1, String target2, int target3) { return String("Hello, {0} {1} {2}!").format(Array::make(target1, target2, target3)); };

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

		Node* getGDSpaceWorld(void);

		void onLoginSuccess(void);
		void onLoginFailed(int failCode);
		void onServerClosed(void);
		void onKicked(int failCode);
		void onUpdateAvatars(void);
		void onPlayerEnterSpace(KBEngine::SPACE_ID spaceId);
		void onPlayerLeaveSpace(KBEngine::SPACE_ID spaceId);
		void onAddSpaceGeoMapping(KBEngine::SPACE_ID spaceId, const char* resPath);

	private:
		Node *m_pSpaceWorld;
	};

}

