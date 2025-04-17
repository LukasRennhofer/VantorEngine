/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifInput.hpp
 * Last Change: 
*/

#pragma once

#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>

#include "../Core/BackLog/chifBacklog.h"
#include "../Helpers/chifMath.hpp"
#include "../Helpers/chifColor.hpp"

namespace chif::Platform::Input
{
    // ======== Buttons ========
    enum BUTTON
	{
		BUTTON_NONE = 0,

		DIGIT_RANGE_START = 48, // digit 0

		CHARACTER_RANGE_START = 65, // letter A

		GAMEPAD_RANGE_START = 256, // do not use!

		GAMEPAD_BUTTON_UP,
		GAMEPAD_BUTTON_LEFT,
		GAMEPAD_BUTTON_DOWN,
		GAMEPAD_BUTTON_RIGHT,
		GAMEPAD_BUTTON_1,
		GAMEPAD_BUTTON_2,
		GAMEPAD_BUTTON_3,
		GAMEPAD_BUTTON_4,
		GAMEPAD_BUTTON_5,
		GAMEPAD_BUTTON_6,
		GAMEPAD_BUTTON_7,
		GAMEPAD_BUTTON_8,
		GAMEPAD_BUTTON_9,
		GAMEPAD_BUTTON_10,
		GAMEPAD_BUTTON_11,
		GAMEPAD_BUTTON_12,
		GAMEPAD_BUTTON_13,
		GAMEPAD_BUTTON_14,

		// Check analog presses like a button:
		GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_UP,
		GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_LEFT,
		GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_DOWN,
		GAMEPAD_ANALOG_THUMBSTICK_L_AS_BUTTON_RIGHT,
		GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_UP,
		GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_LEFT,
		GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_DOWN,
		GAMEPAD_ANALOG_THUMBSTICK_R_AS_BUTTON_RIGHT,
		GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON,
		GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON,

		// Xbox mapping of generic codes:
		GAMEPAD_BUTTON_XBOX_X = GAMEPAD_BUTTON_1,
		GAMEPAD_BUTTON_XBOX_A = GAMEPAD_BUTTON_2,
		GAMEPAD_BUTTON_XBOX_B = GAMEPAD_BUTTON_3,
		GAMEPAD_BUTTON_XBOX_Y = GAMEPAD_BUTTON_4,
		GAMEPAD_BUTTON_XBOX_L1 = GAMEPAD_BUTTON_5,
		GAMEPAD_BUTTON_XBOX_LT = GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON,
		GAMEPAD_BUTTON_XBOX_R1 = GAMEPAD_BUTTON_6,
		GAMEPAD_BUTTON_XBOX_RT = GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON,
		GAMEPAD_BUTTON_XBOX_L3 = GAMEPAD_BUTTON_7,
		GAMEPAD_BUTTON_XBOX_R3 = GAMEPAD_BUTTON_8,
		GAMEPAD_BUTTON_XBOX_BACK = GAMEPAD_BUTTON_9,
		GAMEPAD_BUTTON_XBOX_START = GAMEPAD_BUTTON_10,

		// Playstation mapping of generic codes:
		GAMEPAD_BUTTON_PLAYSTATION_SQUARE = GAMEPAD_BUTTON_1,
		GAMEPAD_BUTTON_PLAYSTATION_CROSS = GAMEPAD_BUTTON_2,
		GAMEPAD_BUTTON_PLAYSTATION_CIRCLE = GAMEPAD_BUTTON_3,
		GAMEPAD_BUTTON_PLAYSTATION_TRIANGLE = GAMEPAD_BUTTON_4,
		GAMEPAD_BUTTON_PLAYSTATION_L1 = GAMEPAD_BUTTON_5,
		GAMEPAD_BUTTON_PLAYSTATION_L2 = GAMEPAD_ANALOG_TRIGGER_L_AS_BUTTON,
		GAMEPAD_BUTTON_PLAYSTATION_R1 = GAMEPAD_BUTTON_6,
		GAMEPAD_BUTTON_PLAYSTATION_R2 = GAMEPAD_ANALOG_TRIGGER_R_AS_BUTTON,
		GAMEPAD_BUTTON_PLAYSTATION_L3 = GAMEPAD_BUTTON_7,
		GAMEPAD_BUTTON_PLAYSTATION_R3 = GAMEPAD_BUTTON_8,
		GAMEPAD_BUTTON_PLAYSTATION_SHARE = GAMEPAD_BUTTON_9,
		GAMEPAD_BUTTON_PLAYSTATION_OPTION = GAMEPAD_BUTTON_10,
		GAMEPAD_BUTTON_PLAYSTATION_TOUCHPAD = GAMEPAD_BUTTON_14,
		GAMEPAD_BUTTON_PLAYSTATION_SELECT = GAMEPAD_BUTTON_PLAYSTATION_TOUCHPAD,
		GAMEPAD_BUTTON_PLAYSTATION_START = GAMEPAD_BUTTON_PLAYSTATION_OPTION,

        // Nintendo Switch mapping of generics codes:
        // Switch layout aliases (Pro Controller)
        GAMEPAD_BUTTON_SWITCH_B       = GAMEPAD_BUTTON_1,  // Bottom (A)
        GAMEPAD_BUTTON_SWITCH_A       = GAMEPAD_BUTTON_2,  // Right (B)
        GAMEPAD_BUTTON_SWITCH_Y       = GAMEPAD_BUTTON_3,  // Left (Y)
        GAMEPAD_BUTTON_SWITCH_X       = GAMEPAD_BUTTON_4,  // Top (X)

        GAMEPAD_BUTTON_SWITCH_L       = GAMEPAD_BUTTON_5,  // Left Shoulder (L)
        GAMEPAD_BUTTON_SWITCH_R       = GAMEPAD_BUTTON_6,  // Right Shoulder (R)
        GAMEPAD_BUTTON_SWITCH_L_STICK = GAMEPAD_BUTTON_7,  // L Stick Press
        GAMEPAD_BUTTON_SWITCH_R_STICK = GAMEPAD_BUTTON_8,  // R Stick Press

        GAMEPAD_BUTTON_SWITCH_MINUS   = GAMEPAD_BUTTON_9,  // -
        GAMEPAD_BUTTON_SWITCH_PLUS    = GAMEPAD_BUTTON_10, // +
        GAMEPAD_BUTTON_SWITCH_HOME    = GAMEPAD_BUTTON_11, // Home
        GAMEPAD_BUTTON_SWITCH_CAPTURE = GAMEPAD_BUTTON_12, // Capture
        GAMEPAD_BUTTON_SWITCH_SR      = GAMEPAD_BUTTON_13, // SR (Joy-Con)
        GAMEPAD_BUTTON_SWITCH_SL      = GAMEPAD_BUTTON_14, // SL (Joy-Con)

		GAMEPAD_RANGE_END = 512, // do not use!

		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,

		// Detect mouse scroll like a button:
		MOUSE_SCROLL_AS_BUTTON_UP,
		MOUSE_SCROLL_AS_BUTTON_DOWN,

		KEYBOARD_BUTTON_UP,
		KEYBOARD_BUTTON_DOWN,
		KEYBOARD_BUTTON_LEFT,
		KEYBOARD_BUTTON_RIGHT,
		KEYBOARD_BUTTON_SPACE,
		KEYBOARD_BUTTON_RSHIFT,
		KEYBOARD_BUTTON_LSHIFT,
		KEYBOARD_BUTTON_F1,
		KEYBOARD_BUTTON_F2,
		KEYBOARD_BUTTON_F3,
		KEYBOARD_BUTTON_F4,
		KEYBOARD_BUTTON_F5,
		KEYBOARD_BUTTON_F6,
		KEYBOARD_BUTTON_F7,
		KEYBOARD_BUTTON_F8,
		KEYBOARD_BUTTON_F9,
		KEYBOARD_BUTTON_F10,
		KEYBOARD_BUTTON_F11,
		KEYBOARD_BUTTON_F12,
		KEYBOARD_BUTTON_ENTER,
		KEYBOARD_BUTTON_ESCAPE,
		KEYBOARD_BUTTON_HOME,
		KEYBOARD_BUTTON_RCONTROL,
		KEYBOARD_BUTTON_LCONTROL,
		KEYBOARD_BUTTON_DELETE,
		KEYBOARD_BUTTON_BACKSPACE,
		KEYBOARD_BUTTON_PAGEDOWN,
		KEYBOARD_BUTTON_PAGEUP,
		KEYBOARD_BUTTON_NUMPAD0,
		KEYBOARD_BUTTON_NUMPAD1,
		KEYBOARD_BUTTON_NUMPAD2,
		KEYBOARD_BUTTON_NUMPAD3,
		KEYBOARD_BUTTON_NUMPAD4,
		KEYBOARD_BUTTON_NUMPAD5,
		KEYBOARD_BUTTON_NUMPAD6,
		KEYBOARD_BUTTON_NUMPAD7,
		KEYBOARD_BUTTON_NUMPAD8,
		KEYBOARD_BUTTON_NUMPAD9,
		KEYBOARD_BUTTON_MULTIPLY,
		KEYBOARD_BUTTON_ADD,
		KEYBOARD_BUTTON_SEPARATOR,
		KEYBOARD_BUTTON_SUBTRACT,
		KEYBOARD_BUTTON_DECIMAL,
		KEYBOARD_BUTTON_DIVIDE,
		KEYBOARD_BUTTON_TAB,
		KEYBOARD_BUTTON_TILDE,
		KEYBOARD_BUTTON_INSERT,
		KEYBOARD_BUTTON_ALT,
		KEYBOARD_BUTTON_ALTGR,

		// must be the last entry
		BUTTON_ENUM_SIZE
	};

    // States
    struct KeyboardState
	{
		bool buttons[BUTTON_ENUM_SIZE] = {}; // it contains pressed buttons as "keyboard/typewriter" like, so no continuous presses
	};
    
	struct MouseState
	{
		chif::Helpers::Math::Vec2 position = chif::Helpers::Math::Vec2(0, 0);
		chif::Helpers::Math::Vec2 delta_position = chif::Helpers::Math::Vec2(0, 0);
		float delta_wheel = 0;
		float pressure = 1.0f;
		bool left_button_press = false;
		bool middle_button_press = false;
		bool right_button_press = false;
	};

	struct ControllerState
	{
		uint32_t buttons = 0;
		chif::Helpers::Math::Vec2 thumbstick_L = chif::Helpers::Math::Vec2(0, 0);
		chif::Helpers::Math::Vec2 thumbstick_R = chif::Helpers::Math::Vec2(0, 0);
		float trigger_L = 0;
		float trigger_R = 0;
	};

	struct ControllerFeedback
	{
		float vibration_left = 0;	// left vibration motor (0: no vibration, 1: maximum vibration)
		float vibration_right = 0;	// right vibration motor (0: no vibration, 1: maximum vibration)
		chif::Helpers::Color led_color;			// led color
	};

    // enum InputType {
    //     KEYBOARD,
    //     GENERIC_CONTROLLER, // e.g native windows controller,
    //     XBOX_CONTROLLER,
    //     SWITCH_CONTROLLER
    // }

    struct Internal_ControllerState
    {
        Sint32 portID;
        SDL_JoystickID internalID;
        SDL_GameController* controller;
        Uint16 rumble_l, rumble_r = 0;
        ControllerState state;
    };

	extern std::vector<SDL_Event> events;
    extern std::vector<Internal_ControllerState> controllers;
    extern std::unordered_map<SDL_JoystickID, size_t> controller_mapped;

    extern KeyboardState keyboard;
    extern MouseState mouse;

    // Main Functions
    void Initialize();
    void Update();
    void ProcessEvent(const SDL_Event &event);

    // Input Handlers
    int toCHIFEngine(const SDL_Scancode &key, const SDL_Keycode &keyk);
    void controllerToCHIFEngine(uint32_t *current, Uint8 button, bool pressed);

    // Controller Utils
    void controller_map_rebuild();
    int GetMaxControllerCount();
    void SetControllerFeedback(const ControllerFeedback& data, int index);

	// Input functions
	bool Pressed_Keyboard(int button);
	bool Pressed_Controller(int index, int buttonBit);
} // namespace chif::Platform::Input
