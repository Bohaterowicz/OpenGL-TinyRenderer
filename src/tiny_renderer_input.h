#pragma once
#include "tiny_renderer_c.h"

struct mouse_input
{
    real32 dX;
    real32 dY;
};

struct tiny_renderer_input
{
    mouse_input Mouse;
};
