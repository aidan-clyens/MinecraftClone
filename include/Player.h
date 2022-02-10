#pragma once

// Includes
#include "Engine/utils/types.h"
#include "Engine/ECS/DynamicCharacterController.h"
#include "Engine/Object3D.h"
#include "Engine/InputManager.h"
#include "Engine/Camera.h"

// Defines
#define CAMERA_SPEED 5

/* Player
 */
class Player : public Object3D {
    public:
        Player(vec3 starting_position);

        void update(float delta_time);

        Camera *get_camera();
    
    private:
        InputManager *p_input_manager;

        Camera m_camera;
};