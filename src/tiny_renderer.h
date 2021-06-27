#pragma once
#include "tiny_renderer_c.h"
#include "opengl_renderer.h"
#include "base_camera.h"
#include "tiny_renderer_input.h"
#include "tiny_renderer_state.h"
#include <memory>

struct tiny_renderer_window_info
{
    uint32 ClientWidth;
    uint32 ClientHeight;
};

void UpdateAndRender(tiny_renderer_window_info &WindowInfo, tiny_renderer_state &TinyRendererState, tiny_renderer_input &Input);
