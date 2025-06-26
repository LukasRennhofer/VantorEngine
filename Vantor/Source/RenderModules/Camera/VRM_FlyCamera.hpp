#pragma once

// Renderer
#include "../../Renderer/Camera/VRE_Camera.hpp"
#include "../../Math/Linear/VMA_Vector.hpp"

// Fly Camera from Cell Engine @JoeyDeVries

namespace Vantor::RenderModules
{
    class FlyCamera : public Vantor::Renderer::Camera
    {
	public:
		float Yaw;
		float Pitch;
	
		float MovementSpeed   = 10.0f;
		float MouseSensitivty =  0.1f;
		float Damping         =  5.0f;
	private:
		Vantor::Math::VVector3 m_TargetPosition;
		Vantor::Math::VVector3 m_WorldUp;
        float m_TargetYaw;
        float m_TargetPitch;
	
    public:
        FlyCamera(Vantor::Math::VVector3 position, Vantor::Math::VVector3 forward = Vantor::Math::VVector3(0.0f, 0.0f, -1.0f), Vantor::Math::VVector3 up = Vantor::Math::VVector3(0.0f, 1.0f, 0.0f));

        virtual void Update(float dt);

        virtual void InputKey(float dt, Vantor::Renderer::VECameraMovement direction);
        virtual void InputMouse(float deltaX, float deltaY);
        virtual void InputScroll(float deltaX, float deltaY);
    };
}