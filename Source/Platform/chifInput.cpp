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
 * File: chifInput.cpp
 * Last Change: Platform: Added main input system (0.15.17)
*/

#include "chifInput.hpp"

#include <array>
#include <cstring>

namespace chif::Platform::Input
{
    std::vector<SDL_Event> events;
    std::vector<Internal_ControllerState> controllers;
    std::unordered_map<SDL_JoystickID, size_t> controller_mapped;

    KeyboardState keyboard;
    MouseState mouse;

    bool prev_keyboard_buttons[BUTTON_ENUM_SIZE] = {};
    std::array<uint32_t, 4> prev_controller_buttons{}; // TODO*: Max Controllers?


    // Main Functions
    void Initialize() {
        if(!SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt")){
            chif::Backlog::Log("Input", "No controller config loaded, add gamecontrollerdb.txt file next to the executable or download it from https://github.com/gabomdq/SDL_GameControllerDB", chif::Backlog::LogLevel::WARNING);
        }

        if (SDL_NumJoysticks() < 1) {
            chif::Backlog::Log("Input", "No joysticks connected.", chif::Backlog::LogLevel::WARNING);
        }        
    }

    void Update()
    {
        mouse.delta_wheel = 0; // Do not accumulate mouse wheel motion delta
        mouse.delta_position = chif::Helpers::Math::Vec2(0, 0); // Do not accumulate mouse position delta

        for(auto& event : events){
            switch(event.type) {
                // Keyboard events
                case SDL_KEYDOWN:             // Key pressed
                {
                    int converted = toCHIFEngine(event.key.keysym.scancode, event.key.keysym.sym);
                    if (converted >= 0) {
                        keyboard.buttons[converted] = true;
                    }
                    break;
                }
                case SDL_KEYUP:               // Key released
                {
                    int converted = toCHIFEngine(event.key.keysym.scancode, event.key.keysym.sym);
                    if (converted >= 0) {
                        keyboard.buttons[converted] = false;
                    }
                    break;
                }
                case SDL_TEXTEDITING:         // Keyboard text editing (composition)
                case SDL_TEXTINPUT:           // Keyboard text input
                case SDL_KEYMAPCHANGED:       // Keymap changed due to a system event such as an
                    //     input language or keyboard layout change.
                    break;


                // mouse events
                case SDL_BUTTON_MIDDLE:
                        mouse.middle_button_press = false;
                        break;
                case SDL_MOUSEWHEEL:           // Mouse wheel motion
                {
                    float delta = static_cast<float>(event.wheel.y);
                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                        delta *= -1;
                    }
                    mouse.delta_wheel += delta;
                    break;
                }


                    // Joystick events
                case SDL_JOYAXISMOTION:          // Joystick axis motion
                case SDL_JOYBALLMOTION:          // Joystick trackball motion
                case SDL_JOYHATMOTION:           // Joystick hat position change
                case SDL_JOYBUTTONDOWN:          // Joystick button pressed
                case SDL_JOYBUTTONUP:            // Joystick button released
                case SDL_JOYDEVICEADDED:         // A new joystick has been inserted into the system
                case SDL_JOYDEVICEREMOVED:       // An opened joystick has been removed
                    break;


                    // Game controller events
                case SDL_CONTROLLERAXISMOTION:          // Game controller axis motion
                {
                    auto controller_get = controller_mapped.find(event.caxis.which);
                    if(controller_get != controller_mapped.end()){
                        float raw = event.caxis.value / 32767.0f;
                        const float deadzone = 0.2;
                        float deadzoned = (raw < -deadzone || raw > deadzone) ? raw : 0;
                        switch(event.caxis.axis){
                            case SDL_CONTROLLER_AXIS_LEFTX:
                                controllers[controller_get->second].state.thumbstick_L.x = deadzoned;
                                break;
                            case SDL_CONTROLLER_AXIS_LEFTY:
                                controllers[controller_get->second].state.thumbstick_L.y = -deadzoned;
                                break;
                            case SDL_CONTROLLER_AXIS_RIGHTX:
                                controllers[controller_get->second].state.thumbstick_R.x = deadzoned;
                                break;
                            case SDL_CONTROLLER_AXIS_RIGHTY:
                                controllers[controller_get->second].state.thumbstick_R.y = deadzoned;
                                break;
                            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                                controllers[controller_get->second].state.trigger_L = (deadzoned > 0.f) ? deadzoned : 0;
                                break;
                            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                                controllers[controller_get->second].state.trigger_R = (deadzoned > 0.f) ? deadzoned : 0;
                                break;
                        }
                        }
                    break;
                }
                case SDL_CONTROLLERBUTTONDOWN:          // Game controller button pressed
                {
                    auto find = controller_mapped.find(event.cbutton.which);
                    if(find != controller_mapped.end()){
                        controllerToCHIFEngine(&controllers[find->second].state.buttons, event.cbutton.button, true);
                    }
                    break;
                }
                case SDL_CONTROLLERBUTTONUP:            // Game controller button released
                {
                    auto find = controller_mapped.find(event.cbutton.which);
                    if(find != controller_mapped.end()){
                        controllerToCHIFEngine(&controllers[find->second].state.buttons, event.cbutton.button, false);
                    }
                    break;
                }
                case SDL_CONTROLLERDEVICEADDED:         // A new Game controller has been inserted into the system
                {
                    chif::Backlog::Log("Input", "Controller added", chif::Backlog::LogLevel::DEBUG);
                    auto& controller = controllers.emplace_back();
                    controller.controller = SDL_GameControllerOpen(event.cdevice.which);
                    if(controller.controller){
                        controller.portID = event.cdevice.which;
                        controller.internalID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller.controller));
                    }
                    controller_map_rebuild();
                    break;
                }
                case SDL_CONTROLLERDEVICEREMOVED:       // An opened Game controller has been removed
                {
                    auto find = controller_mapped.find(event.cdevice.which);
                    if(find != controller_mapped.end()){
                        SDL_GameControllerClose(controllers[find->second].controller);
                        controllers[find->second] = std::move(controllers.back());
                        controllers.pop_back();
                    }
                    controller_map_rebuild();
                    break;
                }
                case SDL_CONTROLLERDEVICEREMAPPED:      // The controller mapping was updated
                    break;
                default:
                    break;
             }
            
                
            // Clone all events for use outside the internal code, e.g. main_SDL2.cpp can benefit from this
            //external_events.push_back(event);
        events.clear();

        //Update rumble every call
        for (auto& controller : controllers){
            SDL_GameControllerRumble(
                controller.controller,
                controller.rumble_l,
                controller.rumble_r,
                60); //Buffer at 60ms
            }
        }


        std::memcpy(prev_keyboard_buttons, keyboard.buttons, sizeof(bool) * 256);

        for (int i = 0; i < controllers.size(); ++i) {
            prev_controller_buttons[i] = controllers[i].state.buttons;
        }

    }

    void ProcessEvent(const SDL_Event &event){
        events.push_back(event);
    }

    // Input Handlers
    int toCHIFEngine(const SDL_Scancode &key, const SDL_Keycode &keyk) {

        // Scancode Conversion Segment
        if(key >= 4 && key <= 29){ // A to Z
            return (key - 4) + chif::Platform::Input::CHARACTER_RANGE_START;
        }
        if(key >= 30 && key <= 39){ // 0 to 9
            return (key - 30) + chif::Platform::Input::DIGIT_RANGE_START;
        }
        if(key >= 58 && key <= 69){ // F1 to F12
            return (key - 58) + chif::Platform::Input::KEYBOARD_BUTTON_F1;
        }
        if(key >= 79 && key <= 82){ // Keyboard directional buttons
            return (82 - key) + chif::Platform::Input::KEYBOARD_BUTTON_UP;
        }
        switch(key){ // Individual scancode key conversion
            case SDL_SCANCODE_SPACE:
                return chif::Platform::Input::KEYBOARD_BUTTON_SPACE;
            case SDL_SCANCODE_LSHIFT:
                return chif::Platform::Input::KEYBOARD_BUTTON_LSHIFT;
            case SDL_SCANCODE_RSHIFT:
                return chif::Platform::Input::KEYBOARD_BUTTON_RSHIFT;
            case SDL_SCANCODE_RETURN:
                return chif::Platform::Input::KEYBOARD_BUTTON_ENTER;
            case SDL_SCANCODE_ESCAPE:
                return chif::Platform::Input::KEYBOARD_BUTTON_ESCAPE;
            case SDL_SCANCODE_HOME:
                return chif::Platform::Input::KEYBOARD_BUTTON_HOME;
            case SDL_SCANCODE_RCTRL:
                return chif::Platform::Input::KEYBOARD_BUTTON_RCONTROL;
            case SDL_SCANCODE_LCTRL:
                return chif::Platform::Input::KEYBOARD_BUTTON_LCONTROL;
            case SDL_SCANCODE_DELETE:
                return chif::Platform::Input::KEYBOARD_BUTTON_DELETE;
            case SDL_SCANCODE_BACKSPACE:
                return chif::Platform::Input::KEYBOARD_BUTTON_BACKSPACE;
            case SDL_SCANCODE_PAGEDOWN:
                return chif::Platform::Input::KEYBOARD_BUTTON_PAGEDOWN;
            case SDL_SCANCODE_PAGEUP:
                return chif::Platform::Input::KEYBOARD_BUTTON_PAGEUP;
			case SDL_SCANCODE_KP_0:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD0;
			case SDL_SCANCODE_KP_1:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD1;
			case SDL_SCANCODE_KP_2:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD2;
			case SDL_SCANCODE_KP_3:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD3;
			case SDL_SCANCODE_KP_4:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD4;
			case SDL_SCANCODE_KP_5:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD5;
			case SDL_SCANCODE_KP_6:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD6;
			case SDL_SCANCODE_KP_7:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD7;
			case SDL_SCANCODE_KP_8:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD8;
			case SDL_SCANCODE_KP_9:
				return chif::Platform::Input::KEYBOARD_BUTTON_NUMPAD9;
			case SDL_SCANCODE_KP_MULTIPLY:
				return chif::Platform::Input::KEYBOARD_BUTTON_MULTIPLY;
			case SDL_SCANCODE_KP_PLUS:
				return chif::Platform::Input::KEYBOARD_BUTTON_ADD;
			case SDL_SCANCODE_SEPARATOR:
				return chif::Platform::Input::KEYBOARD_BUTTON_SEPARATOR;
			case SDL_SCANCODE_KP_MINUS:
				return chif::Platform::Input::KEYBOARD_BUTTON_SUBTRACT;
			case SDL_SCANCODE_KP_DECIMAL:
				return chif::Platform::Input::KEYBOARD_BUTTON_DECIMAL;
			case SDL_SCANCODE_KP_DIVIDE:
				return chif::Platform::Input::KEYBOARD_BUTTON_DIVIDE;
			case SDL_SCANCODE_INSERT:
				return chif::Platform::Input::KEYBOARD_BUTTON_INSERT;
			case SDL_SCANCODE_TAB:
				return chif::Platform::Input::KEYBOARD_BUTTON_TAB;
			case SDL_SCANCODE_GRAVE: 
    			return chif::Platform::Input::KEYBOARD_BUTTON_TILDE;
			case SDL_SCANCODE_LALT:
				return chif::Platform::Input::KEYBOARD_BUTTON_ALT;
			case SDL_SCANCODE_RALT:
				return chif::Platform::Input::KEYBOARD_BUTTON_ALTGR;
        }
        // Keycode Conversion Segment

        if(keyk >= 91 && keyk <= 126){
            return keyk;
        }

        return -1;
    }

    void controllerToCHIFEngine(uint32_t *current, Uint8 button, bool pressed){
        uint32_t btnenum;
        switch(button){
            case SDL_CONTROLLER_BUTTON_DPAD_UP: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_UP; break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_LEFT; break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_DOWN; break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_RIGHT; break;
            case SDL_CONTROLLER_BUTTON_X: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_1; break;
            case SDL_CONTROLLER_BUTTON_A: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_2; break;
            case SDL_CONTROLLER_BUTTON_B: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_3; break;
            case SDL_CONTROLLER_BUTTON_Y: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_4; break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_5; break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_6; break;
            case SDL_CONTROLLER_BUTTON_LEFTSTICK: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_7; break;
            case SDL_CONTROLLER_BUTTON_RIGHTSTICK: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_8; break;
            case SDL_CONTROLLER_BUTTON_BACK: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_9; break;
            case SDL_CONTROLLER_BUTTON_START: btnenum = chif::Platform::Input::GAMEPAD_BUTTON_10; break;
			default: return;
        }
        btnenum = 1 << (btnenum - chif::Platform::Input::GAMEPAD_RANGE_START - 1);
        if(pressed){
            *current |= btnenum;
        }else{
            *current &= ~btnenum;
        }
    }

    void controller_map_rebuild(){
        controller_mapped.clear();
        for(int index = 0; index < controllers.size(); ++index){
            controller_mapped.insert({controllers[index].internalID, index});
        }
    }

    // Controller Utils
    int GetMaxControllerCount() { return controllers.size(); }

    void SetControllerFeedback(const ControllerFeedback& data, int index) {
        if(index < controllers.size()){
            #ifdef SDL2_FEATURE_CONTROLLER_LED
                SDL_GameControllerSetLED(
                    controllers[index].controller,
                    data.led_color.getR(),
                    data.led_color.getG(),
                    data.led_color.getB());
            #endif
            controllers[index].rumble_l = (Uint16)floor(data.vibration_left * 0xFFFF);
            controllers[index].rumble_r = (Uint16)floor(data.vibration_right * 0xFFFF);
        }
    }

    // Input functions
    bool Pressed_Keyboard(int button) {
        return keyboard.buttons[button] && !prev_keyboard_buttons[button];
    }
    
    bool Pressed_Controller(int controllerIndex, int button) {
        if (controllerIndex < 0 || controllerIndex >= controllers.size()) {
            return false; // Invalid controller index
        }
    
        auto& controller = controllers[controllerIndex];
        return (controller.state.buttons & (1 << (button - GAMEPAD_RANGE_START))) != 0;
    }

} // namespace chif::Platform::Input
