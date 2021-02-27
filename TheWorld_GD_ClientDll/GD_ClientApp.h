#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Reference.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

	class Entity_Visuals;

	class GD_ClientApp : public Node, public TheWorld_ClientApp
	{
	
		GODOT_CLASS(GD_ClientApp, Node)

	public:
		static void _register_methods();

		GD_ClientApp();
		~GD_ClientApp();

		//
		// Test
		//
		void debugPrint(String message) { if (m_isDebugEnabled) Godot::print(message); }
		String hello(String target1, String target2, int target3) { return String("Hello, {0} {1} {2}!").format(Array::make(target1, target2, target3)); };
		
		//
		// General App Functions
		//
		void setAppInError(int errorCode) {
			m_erroCodeApp = errorCode;
			m_bAppInError = true;
		}
		bool getAppInError(void) { return m_bAppInError; }
		bool getAppInError(int& erroCodeApp) {
			erroCodeApp = m_erroCodeApp;
			return m_bAppInError;
		}
		bool isDebugEnabled(void) { return m_isDebugEnabled; }
		void setDebugEnabled(bool b = true) { m_isDebugEnabled = b; }
		bool isEditMode(void) { return m_isEditMode; }
		void setEditMode(bool b = true) { m_isEditMode = b; }

		//
		// Godot Standard Functions
		//
		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
		void _input(const Ref<InputEvent> event);

		//
		// Server Interaction
		//
		void setAppMode(int r, bool bForce = false);
		int  getAppMode1(void);
		void setLoginStatus(int i);
		int  getLoginStatus(void);
		bool kbengine_Init(Node* pWorldNode, Node* pMainNode);
		void kbengine_Destroy(void);
		bool kbengine_Login(String accountName, String passwd, String datas, String ip, int port);
		bool kbengine_Logout(void);
		void kbengine_MessagePump(void);
		bool getShutdownRequired(void);
		bool getDoSleepInMainLoop(void);
		int getPlayerID(void);
		bool enterGame(int avatarId);
		bool createAvatar(String avatarName);
		bool removeAvatar(String avatarName);

		// AVATAR
		int getAvatarsCount(void);
		int getAvatarIdByIdx(int idx);
		String getAvatarNameByIdx(int idx);
		String getAvatarNameById(int id);
		// AVATAR

		// ENTITY
		int getEntitiesCount(void);
		int getEntityIdByIdx(int idx);
		KBEntity* getEntityById(int id, bool& bPlayer);
		String getEntityNameById(int id);
		// ENTITY

		Node* getSpaceWorldNode(void);

		// Events
		void onLoginSuccess(void);
		void onLoginFailed(int failCode);
		void onServerClosed(void);
		void onKicked(int failCode);
		void onClearEntities(void);
		void onCreatedEntity(KBEngine::ENTITY_ID eid, bool bPlayer);
		void onEraseEntity(KBEngine::ENTITY_ID eid);
		void onClearAvatars(void);
		void onEraseAvatar(KBEngine::DBID dbid);
		void onUpdateAvatars(void);
		void onPlayerEnterSpace(KBEngine::SPACE_ID spaceId);
		void onPlayerLeaveSpace(KBEngine::SPACE_ID spaceId);
		void onAddSpaceGeoMapping(KBEngine::SPACE_ID spaceId, const char* resPath);

		//
		// Godot Node
		//
		int getEntityNodeCount(void);
		int getEntityNodes(Array& entities);
		Node* getEntityNodeById(int id, bool bIgnoreValid = false);
		Node* getEntityNodeByIdx(int idx, bool bIgnoreValid = false);
		Node* getPlayerNode(bool bIgnoreValid = false);

		//
		// Godot Entity Visuals
		//
#define GD_CLIENTAPP_ENTITYVISUALS_PLAYER	0
#define GD_CLIENTAPP_ENTITYVISUALS_NPC		1
#define GD_CLIENTAPP_ENTITYVISUALS_MONSTER	2
		Entity_Visuals* getEntityVisuals(int iType);

	private:
		Node *m_pSpaceWorldNode;
		bool m_bAppInError;
		int m_erroCodeApp;
		//int m_iProgEntityCamera;
		bool m_isDebugEnabled;
		bool m_isEditMode;

		Entity_Visuals* m_pPlayer_EntityVisuals;
		Entity_Visuals* m_pNPC_EntityVisuals;
		Entity_Visuals* m_pMonster_EntityVisuals;
	};

}

#define GD_CLIENTAPP_ERROR_CREATE_PLAYER_ENTITY				1
#define GD_CLIENTAPP_ERROR_CREATE_OTHER_ENTITY				2
#define GD_CLIENTAPP_ERROR_CREATE_OTHER_ENTITY_CONTAINER	3
#define GD_CLIENTAPP_ERROR_INIT_NODE_ENTITY					4
#define GD_CLIENTAPP_ERROR_ENTITY_PROCESS					5

// World Node
#define	GD_CLIENTAPP_ENTITIES_CONTAINER_NODE	"Entities"
#define	GD_CLIENTAPP_PLAYER_ENTITY_NODE				"PlayerEntity"
#define	GD_CLIENTAPP_OTHER_ENTITY_NODE				"OtherEntity"

#define	GD_CLIENTAPP_ENTITIES_GROUP				"Entities"
#define	GD_CLIENTAPP_PLAYER_GROUP				"Player"
#define	GD_CLIENTAPP_MONSTERS_GROUP				"Monster"
#define	GD_CLIENTAPP_NPCS_GROUP					"NPCs"