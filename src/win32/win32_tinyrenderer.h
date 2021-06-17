#pragma once
#include "tiny_renderer_c.h"
#include "tiny_renderer.h"
#include <Windows.h>
#include <Windowsx.h>

#ifdef DEBUG
#define OUTPUT_DEBUG(msg) OutputDebugStringA((msg))
#else
#define OUTPUT_DEBUG(msg)
#endif

//////////// PIXEL FORMAT DEFINES ///////////////

#define PIXELFORMAT_COLOR_BITS 32
#define PIXELFORMAT_ALPHA_BITS 8
#define PIXELFORMAT_DEPTH_BITS 24
#define PIXELFORMAT_STENCIL_BITS 8

////////////////////////////////////////////////

////////// OPENGL INIT CONSTANTS //////////////

#define OPENGL_DESIRED_MAJOR_VERION 4
#define OPENGL_DESIRED_MINOR_VERION 5

///////////////////////////////////////////////

struct win32_window_state
{
    bool32 Running;
    uint32 ClientWidth;
    uint32 ClientHeight;
    POINT MousePosition;
};

struct win32_client_dim
{
    uint32 Width;
    uint32 Height;
};

void Win32ProcessPendingWindowMessages(win32_window_state *WindowState, tiny_renderer_input *Input);
LRESULT CALLBACK Win32TinyRendererWindowProc(HWND Window, UINT Msg, WPARAM WParam, LPARAM LParam);
bool32 Win32InitializeOpenGL(HWND Window);
struct win32_client_dim Win32GetClientDimension(HWND WindowHandle);
bool32 Win32SetPixelFormat(HWND Window);
bool32 Win32OpenDebugConsole();
void Win32ProcessMouseInput(HWND Window, win32_window_state *WindowState, tiny_renderer_input *Input);

//Get the window state pointer stored in window handle during CreateWindowEx call
inline win32_window_state *Win32GetWindowState(HWND hwnd)
{
    LONG_PTR Ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    win32_window_state *WindowState = reinterpret_cast<win32_window_state *>(Ptr);
    return WindowState;
}