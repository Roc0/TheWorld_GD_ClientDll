#pragma once

#include <Godot.hpp>
#include <Camera.hpp>
#include <InputEvent.hpp>

#include <TheWorld_ClientApp.h>

namespace godot {

	class GD_WorldCamera : public Camera
	{
		GODOT_CLASS(GD_WorldCamera, Camera)

	public:
		static void _register_methods();

		GD_WorldCamera();
		~GD_WorldCamera();

		void _init(); // our initializer called by Godot
		void _ready();
		void _process(float _delta);
		void _physics_process(float _delta);
		void _input(const Ref<InputEvent> event);
		bool initCameraInWorld(Node *pSpaceWorld);
		bool initPlayerCamera(Node *pEntityNode);
		bool initOtherEntityCamera(Node* pEntityNode);
		bool updateCamera(void);
		//bool isCameraInWorldMode(void) { return m_isCamerInWorldMode; }
		//void setCameraInWorldMode(bool b, Transform* pNewPos = NULL);
		void activateCamera(bool bActivate = true);
		bool isPlayerCamera() { return m_PlayerCamera; }
		bool isOtherEntityCamera() { return m_OtherEntityCamera; }
		bool isWorldCamera() { return m_WorldCamera; }

		static Node* getActiveCamera(void);

		bool m_isActive;
		int64_t m_instanceId;

		static int64_t s_ActiveCameraInstanceId;
		static Node* s_pActiveCameraNode;

	private:
		Node* m_pSpaceWorldNode;
		//bool m_isCamerInWorldMode;
		Transform m_lastCameraPosInWorld;
		bool m_PlayerCamera;
		bool m_OtherEntityCamera;
		bool m_WorldCamera;
	

		// Camera Movement
		int m_numMoveStep;
		float m_wheelVelocity;
		// Camera Movement

		// Camera Rotation
		bool m_shiftOriCameraOn;
		bool m_shiftVertCameraOn;
		bool m_rotateCameraOn;
		Vector2 m_mouseRelativePosToRotate;
		Vector2 m_mouseRelativePosToShiftOriz;
		Vector2 m_mouseRelativePosToShiftVert;
		float m_sensitivity;			// 0.0 - 1.0
		float m_smoothness;				// 0.0 - 0.999 tick 0.001
		float m_yaw;
		float m_lastYaw;
		float m_totalYaw;
		int m_yawLimit;
		float m_pitch;
		float m_lastPitch;
		float m_totalPitch;
		int m_pitchLimit;
		// Camera Rotation
	};

}

