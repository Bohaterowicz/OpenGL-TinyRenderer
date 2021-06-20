#pragma once
#include "tiny_renderer_c.h"

#define SUPPORTED_BUTTONS_COUNT 16
#define SUPPORTED_CONTROLLER_COUNT 5

struct mouse_input
{
    real32 dX;
    real32 dY;
};

struct button_state
{
    uint32 HalfTransisionCount;
    bool32 EndedDown;
};

struct input_controller
{
    union
    {
        button_state Buttons[SUPPORTED_BUTTONS_COUNT];
        struct
        {
            button_state MoveForward;
            button_state MoveBack;
            button_state MoveLeft;
            button_state MoveRight;
            button_state MoveUp;
            button_state MoveDown;

            button_state ActionLeft;
            button_state ActionRig;
            button_state AButton;
            button_state BButton;
            button_state XButton;
            button_state YButt;
            button_state LeftShoulder;
            button_state RightShould;
            button_state Back;
            button_state Start;

            //All buttons must be difines above terminator

            button_state Terminator;
        };
    };
};

struct tiny_renderer_input
{
    mouse_input Mouse;
    input_controller Controller[SUPPORTED_CONTROLLER_COUNT];
};
