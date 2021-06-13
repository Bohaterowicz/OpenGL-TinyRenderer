#pragma once
#include "tiny_renderer_c.h"
#include <Windows.h>

#ifdef DEBUG
#define OUTPUT_DEBUG(msg) OutputDebugStringA((msg))
#else
#define OUTPUT_DEBUG(msg)
#endif

struct win32_window_state
{
    bool32 Running;
};

void ProcessPendingWindowMessages();
LRESULT CALLBACK Win32TinyRendererWindowProc(HWND Window, UINT Msg, WPARAM WParam, LPARAM LParam);
bool32 InitializeOpenGL(HWND Window);