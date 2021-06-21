#include "win32_tinyrenderer.h"
#include "GL/glew.h"
#include "GL/wglew.h"
#include <string>
#include <iostream>

#define TARGET_FRAMERATE 60
#define HIDE_CURSOR 1
#define PAUSE_INACTIVE TRUE
#define VSYNC 1 // 1: Activate vsync  --  0: Disable vsync

#define WAS_DOWN_BIT_SHIFT 30
#define IS_DOWN_BIT_SHIFT 31

//Windows Entry point
INT WINAPI WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance, _In_ LPSTR CmdLine, _In_ int ShowCmd)
{
#ifdef DEBUG
    auto DebugConsoleOK = Win32OpenDebugConsole();
    if (DebugConsoleOK != TRUE)
    {
        OUTPUT_DEBUG("Could not open debug console.... (POTENTIAL ERROR)");
    }
#endif

    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    int64 PerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    //Register window class info
    WNDCLASSEXA WndClass = {};
    WndClass.cbSize = sizeof(WNDCLASSEXA);
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //specifies redraw of the window for horizontal and vertical changes in size
    WndClass.lpfnWndProc = Win32TinyRendererWindowProc;  //pointer to implemented window procedure
    WndClass.hInstance = Instance;
    WndClass.hIcon = 0; //Handle to icon class TODO: create icon for window! (hIconSm too!)
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);
    WndClass.lpszClassName = "Win32TinyRenderer";

    //Register window class in OS (needed before using create window)
    if (RegisterClassExA(&WndClass) != 0)
    {
        //Alocate our window state struct, this will be used to store the golbal state of the window.
        win32_window_state *WindowState = new (std::nothrow) win32_window_state;
        if (WindowState != nullptr)
        {
            *WindowState = {};
        }

        //Create window
        HWND Window = CreateWindowExA(0, WndClass.lpszClassName, "TinyRenderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, WindowState);
        //check if we got a valid window handle and state
        if (Window != NULL && WindowState != nullptr)
        {
            //NOTE: Set up of the target framerate of the window, Currently the target will be set in TARGET_FRAMERATE define
            HDC RefreshDC = GetDC(Window);
            //try to get the refresh rate of the monitor from OS
            uint32 Win32RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);
            ReleaseDC(Window, RefreshDC);
            //Our general target framerate is TARGET_FRAMERATE
            uint32 TargetFramerate = TARGET_FRAMERATE;
            //If GetDeviceCaps returned an actual refresh rate of the monitor, we set our target to the monitor refresh rate
            if (Win32RefreshRate > 1)
            {
                TargetFramerate = Win32RefreshRate;
            }
            real32 UpdateHz = TargetFramerate / 1.0F;
            real32 TargetSecondsPerFrame = 1.0F / UpdateHz;

            //Create main renederer state struct and Input struct.
            //IMPORTANT: MUST be initialized to zero!
            tiny_renderer_state TinyRendererState = {};
            tiny_renderer_input Input = {};

            LARGE_INTEGER LastCounter = Win32GetWallClock();
            uint64 LastCycleCount = __rdtsc();

            //NOTE: Main window loop
            WindowState->Running = TRUE;
            while (WindowState->Running == TRUE)
            {
                Input.FrameDt = TargetSecondsPerFrame;
                //NOTE: We need to process our window messages even if our window is inactive!
                Win32ProcessPendingWindowMessages(WindowState, &Input);
                //we want to pause (not update) the app if window is inactive and PAUSE_INACTIVE is set to true
                //if PAUSE_INACTIVE is false, we will continue running the app even if the window is inactive!
                if (WindowState->Active == TRUE || PAUSE_INACTIVE == FALSE)
                {
                    Win32ProcessMouseInput(Window, WindowState, &Input);
                    tiny_renderer_window_info WindowInfo = {};
                    WindowInfo.ClientWidth = WindowState->ClientWidth;
                    WindowInfo.ClientHeight = WindowState->ClientWidth;
                    UpdateAndRender(WindowInfo, TinyRendererState, Input);
                    HDC WindowDC = GetDC(Window);
                    SwapBuffers(WindowDC);
                    ReleaseDC(Window, WindowDC);
                }

                LARGE_INTEGER EndCounter = Win32GetWallClock();
                real32 MSPerFrame = SECONDS_TO_MILLISECONDS(Win32GetSecondsElapsed(LastCounter, EndCounter, PerfCountFrequency));
                LastCounter = EndCounter;

                uint64 EndCycleCount = __rdtsc();
                uint64 CyclesElapsed = EndCycleCount - LastCycleCount;
                LastCycleCount = EndCycleCount;

                real64 MCPF = static_cast<real64>(CyclesElapsed) / static_cast<real64>(Mega(1));

                char TBuffer[DEBUG_TEXT_BUFFER_SIZE];
                sprintf_s(TBuffer, sizeof(TBuffer), "%.02fms/f,  %.02fMc/f\n", MSPerFrame, MCPF);
                OUTPUT_DEBUG(TBuffer);
            }
        }
        else
        {
            //failed to create window...
        }
    }
    else
    {
        //failed to register window class....
    }

    return 0;
}

LRESULT CALLBACK Win32TinyRendererWindowProc(HWND Window, UINT Msg, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Msg)
    {
    case WM_CREATE:
    {
        if (Win32InitializeOpenGL(Window) == FALSE)
        {
            //Could not initialize opengl..
            //NOTE: Currently there is only opengl, if we fail to initialize it, the application has to close. If we were to support other (like Direct3D), then we could try to fallback to those.
            PostQuitMessage(-1);
        }
        else
        {
            win32_window_state *WindowState;
            CREATESTRUCT *Create = reinterpret_cast<CREATESTRUCT *>(LParam);
            WindowState = reinterpret_cast<win32_window_state *>(Create->lpCreateParams);
            SetWindowLongPtr(Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(WindowState));
            //Get client dimensions
            win32_client_dim ClientDim = Win32GetClientDimension(Window);
            WindowState->ClientWidth = ClientDim.Width;
            WindowState->ClientHeight = ClientDim.Height;
            //Initialize the cursor to the center of the screen...
            SetCursorPos(ClientDim.Width / 2, ClientDim.Height / 2);
        }
    }
    break;
    case WM_ACTIVATEAPP:
    {
        if (WParam == TRUE)
        {
            //Window is being activated
            win32_window_state *WindowState = Win32GetWindowState(Window);
            GetClipCursor(&WindowState->PreviousCursorClip);
            GetWindowRect(Window, &WindowState->WindowCursorClip);
            ClipCursor(&WindowState->WindowCursorClip);
            WindowState->TrapCursor = TRUE;
            WindowState->Active = TRUE;
        }
        else
        {
            //Window is being deactivated
            win32_window_state *WindowState = Win32GetWindowState(Window);
            ClipCursor(&WindowState->PreviousCursorClip);
            WindowState->TrapCursor = FALSE;
            WindowState->Active = FALSE;
        }
        OUTPUT_DEBUG("WM_ACTIVATEAPP\n");
    }
    break;
    case WM_DESTROY:
    {
        //deinitialize opengl rc before shutdown
        HGLRC RenderingContext = wglGetCurrentContext();
        if (RenderingContext != NULL)
        {
            wglDeleteContext(RenderingContext);
        }
        win32_window_state *WindowState = Win32GetWindowState(Window);
        WindowState->Running = FALSE;
        OUTPUT_DEBUG("WM_DESTROY\n");
    }
    break;
    case WM_CLOSE:
    {
        //deinitialize opengl rc before shutdown
        HGLRC RenderingContext = wglGetCurrentContext();
        if (RenderingContext != NULL)
        {
            wglDeleteContext(RenderingContext);
        }
        win32_window_state *WindowState = Win32GetWindowState(Window);
        WindowState->Running = FALSE;
        OUTPUT_DEBUG("WM_CLOSE\n");
    }
    break;
    case WM_SIZE:
    {
        win32_window_state *WindowState = Win32GetWindowState(Window);
        win32_client_dim NewClientDim = Win32GetClientDimension(Window);
        WindowState->ClientHeight = NewClientDim.Height;
        WindowState->ClientWidth = NewClientDim.Width;
        OUTPUT_DEBUG("WM_SIZE\n");
    }
    break;
    case WM_SETCURSOR:
    {
        if (HIDE_CURSOR)
        {
            SetCursor(0);
        }
        else
        {
            DefWindowProcA(Window, Msg, WParam, LParam);
        }
    }
    break;
    case WM_MOUSEMOVE:
    {
        win32_window_state *WindowState = Win32GetWindowState(Window);
        WindowState->MousePosition.x = GET_X_LPARAM(LParam);
        WindowState->MousePosition.y = GET_Y_LPARAM(LParam);
    }
    break;
    default:
    {
        Result = DefWindowProcA(Window, Msg, WParam, LParam);
    }
    break;
    }

    return Result;
}

void Win32ProcessPendingWindowMessages(win32_window_state *WindowState, tiny_renderer_input *Input)
{
    MSG Message;
    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE) != 0)
    {
        switch (Message.message)
        {

        case WM_QUIT:
        {
            //deinitialize opengl rc before shutdown
            HGLRC RenderingContext = wglGetCurrentContext();
            if (RenderingContext != NULL)
            {
                wglDeleteContext(RenderingContext);
            }
            WindowState->Running = FALSE;
        }
        break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            Win32ProcessKeys(Message, Input);
        }
        break;
        default:
        {
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        break;
        }
    }
}

bool32 Win32InitializeOpenGL(HWND Window)
{
    bool32 InitializationResult = FALSE;

    //continue only if we succeed in setting the pxel format
    if (Win32SetPixelFormat(Window) != FALSE)
    {
        HDC WindowDC = GetDC(Window);
        //Creat opengl context
        HGLRC OpenGLRenderingContext = wglCreateContext(WindowDC);

        //Check if we got valid rendering context
        if (OpenGLRenderingContext != NULL)
        {
            if (wglMakeCurrent(WindowDC, OpenGLRenderingContext) != 0)
            {
                //initialize glew, if we fail, we should stop initialization of opengl, as we need glew right now.
                if (glewInit() == GLEW_OK)
                {
                    //Context attribs, we specify the version we want to use...
                    int32 WGL_ContextAttribs[] = {
                        WGL_CONTEXT_MAJOR_VERSION_ARB, OPENGL_DESIRED_MAJOR_VERION,
                        WGL_CONTEXT_MINOR_VERSION_ARB, OPENGL_DESIRED_MINOR_VERION,
                        WGL_CONTEXT_FLAGS_ARB, 0,
                        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                        0};

                    //Check for "WGL_ARB_create_context" as we may want to specify our own version of opengl
                    if (wglewIsSupported("WGL_ARB_create_context") == GL_TRUE)
                    {
                        //Create context with specified attributes
                        HGLRC OpenGLRenderingContextARB = wglCreateContextAttribsARB(WindowDC, 0, WGL_ContextAttribs);
                        if (OpenGLRenderingContextARB != NULL)
                        {
                            wglMakeCurrent(0, 0);
                            wglDeleteContext(OpenGLRenderingContext);
                            wglMakeCurrent(WindowDC, OpenGLRenderingContextARB);
                        }
                    }
                    else
                    {
                        OUTPUT_DEBUG("----------> wglCreateContextAtribs not specified (WARNING)");
                    }
                    //We scucceded in initializing opengl in our window
                    //Print out opengl version...
                    const auto *GLVersion = glGetString(GL_VERSION);
                    std::string OGLText("OpenGL VERSION: ");
                    std::string GLVersionStr(reinterpret_cast<const char *>(GLVersion));
                    OUTPUT_DEBUG((OGLText + GLVersionStr + "\n").c_str());

                    //NOTE: IMPORTANT: Activates vsync (swap buffers on vertical blank)!
                    wglSwapIntervalEXT(0);

                    InitializationResult = TRUE;
                }
                else
                {
                    //deinitialize the current rendering context
                    wglMakeCurrent(0, 0);
                    wglDeleteContext(OpenGLRenderingContext);
                    OUTPUT_DEBUG("GLEW could not be initialized... (ABORTING)");
                }
            }
        }
        ReleaseDC(Window, WindowDC);
    }
    return InitializationResult;
}

bool32 Win32SetPixelFormat(HWND Window)
{
    PIXELFORMATDESCRIPTOR PixelFormatDesc = {};
    PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    PixelFormatDesc.nVersion = 1;
    PixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    PixelFormatDesc.cColorBits = PIXELFORMAT_COLOR_BITS;
    PixelFormatDesc.cAlphaBits = PIXELFORMAT_ALPHA_BITS;
    PixelFormatDesc.cDepthBits = PIXELFORMAT_DEPTH_BITS;
    PixelFormatDesc.cStencilBits = PIXELFORMAT_STENCIL_BITS;
    PixelFormatDesc.iLayerType = PFD_MAIN_PLANE;
    HDC WindowDC = GetDC(Window);
    uint32 SuggestedPixelFormat = ChoosePixelFormat(WindowDC, &PixelFormatDesc);
    //Check if we got a valid suggestion of pixel format
    if (SuggestedPixelFormat != 0)
    {
        //continue only if we succeed in setting the pxel format
        if (SetPixelFormat(WindowDC, SuggestedPixelFormat, &PixelFormatDesc) != FALSE)
        {
            return TRUE;
        }
    }
    ReleaseDC(Window, WindowDC);
    return FALSE;
}

win32_client_dim Win32GetClientDimension(HWND WindowHandle)
{
    win32_client_dim Result;
    RECT ClientRect;
    GetClientRect(WindowHandle, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return Result;
}

bool32 Win32OpenDebugConsole()
{
    bool32 Result = AllocConsole();
    if (Result == TRUE)
    {
        freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
        std::cout << "-----> Tiny Renderer DEBUG console! <-----" << std::endl;
    }
    return Result;
}

void Win32ProcessMouseInput(HWND Window, win32_window_state *WindowState, tiny_renderer_input *Input)
{
    int32 CenterX = WindowState->ClientWidth / 2;
    int32 CenterY = WindowState->ClientHeight / 2;

    int32 XDiff = WindowState->MousePosition.x - CenterX;
    int32 YDiff = WindowState->MousePosition.y - CenterY;

    Input->Mouse.dX = static_cast<real32>(XDiff);
    Input->Mouse.dY = static_cast<real32>(YDiff);

    if (WindowState->TrapCursor == TRUE)
    {
        POINT CenterPos = {CenterX, CenterY};
        ClientToScreen(Window, &CenterPos);

        SetCursorPos(CenterPos.x, CenterPos.y);
    }
}

void Win32ProcessKeyboardInput(button_state &NewState, bool32 IsDown)
{
    if (NewState.EndedDown != IsDown)
    {
        NewState.EndedDown = IsDown;
        NewState.HalfTransisionCount++;
    }
}

void Win32ProcessKeys(MSG &Message, tiny_renderer_input *Input)
{
    uint32 VKCode = (uint32)Message.wParam;
    bool32 WasDown = ((Message.lParam & (1LL << WAS_DOWN_BIT_SHIFT)) != 0 ? TRUE : FALSE);
    bool32 IsDown = ((Message.lParam & (1LL << IS_DOWN_BIT_SHIFT)) == 0 ? TRUE : FALSE);
    if (WasDown != IsDown)
    {
        if (VKCode == 'W')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveForward, IsDown);
        }
        if (VKCode == 'A')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveLeft, IsDown);
        }
        if (VKCode == 'S')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveBack, IsDown);
        }
        if (VKCode == 'D')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveRight, IsDown);
        }
        if (VKCode == 'Q')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveDown, IsDown);
        }
        if (VKCode == 'E')
        {
            Win32ProcessKeyboardInput(Input->Controller[0].MoveUp, IsDown);
        }
        if (VKCode == VK_ESCAPE)
        {
            PostQuitMessage(0);
            OUTPUT_DEBUG("ESCAPE (QUIT)\n");
        }
    }
}

LARGE_INTEGER Win32GetWallClock()
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return Result;
}

real32 Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End, int64 PerfCountFrequency)
{
    return ((real32)(End.QuadPart - Start.QuadPart) / (real32)PerfCountFrequency);
}