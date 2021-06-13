#include "win32_tinyrenderer.h"
#include "tiny_renderer.h"
#include "GL/glew.h"
#include "GL/wglew.h"
#include <string>

#define TARGET_FRAMERATE 60

static win32_window_state WindowState;

//Windows Entry point
INT WINAPI WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PrevInstance, _In_ LPSTR CmdLine, _In_ int ShowCmd)
{

    UINT DesiredSchedulerMS = 1;
    //We specify scheduler granularity to be our desired scheduler granularity (1ms), this is so we can use very precise sleep
    MMRESULT SchedulerResult = timeBeginPeriod(DesiredSchedulerMS);
    //We check that we managed to set our desired scheduler granularity
    bool32 SleepIsGranular = (SchedulerResult == TIMERR_NOERROR);

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
    if (RegisterClassExA(&WndClass))
    {
        //Create window
        HWND Window = CreateWindowExA(0, WndClass.lpszClassName, "TinyRenderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);
        //check if we got a valid window handle
        if (Window)
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
            real32 UpdateHz = TargetFramerate / 1.0f;
            real32 UpdateSencodsPerFrame = 1.0f / UpdateHz;

            //TODO: Timing, and framerate control...

            //NOTE: Main window loop
            WindowState.Running = true;
            while (WindowState.Running)
            {
                ProcessPendingWindowMessages();
                UpdateAndRender();
                HDC WindowDC = GetDC(Window);
                SwapBuffers(WindowDC);
                ReleaseDC(Window, WindowDC);
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
        if (!InitializeOpenGL(Window))
        {
            //Could not initialize opengl..
            //NOTE: Currently there is only opengl, if we fail to initialize it, the application has to close. If we were to support other (like Direct3D), then we could try to fallback to those.
            PostQuitMessage(-1);
        }
    }
    break;
    case WM_DESTROY:
    {
        //deinitialize opengl rc before shutdown
        HGLRC RenderingContext = wglGetCurrentContext();
        if (RenderingContext)
            wglDeleteContext(RenderingContext);
        WindowState.Running = false;
        OUTPUT_DEBUG("WM_DESTROY\n");
    }
    break;
    case WM_CLOSE:
    {
        //deinitialize opengl rc before shutdown
        HGLRC RenderingContext = wglGetCurrentContext();
        if (RenderingContext)
            wglDeleteContext(RenderingContext);
        WindowState.Running = false;
        OUTPUT_DEBUG("WM_CLOSE\n");
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

void ProcessPendingWindowMessages()
{
    MSG Message;
    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch (Message.message)
        {

        case WM_QUIT:
        {
            //deinitialize opengl rc before shutdown
            HGLRC RenderingContext = wglGetCurrentContext();
            if (RenderingContext)
                wglDeleteContext(RenderingContext);
            WindowState.Running = false;
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

bool32 InitializeOpenGL(HWND Window)
{
    bool32 InitializationResult = false;

    PIXELFORMATDESCRIPTOR PixelFormatDesc = {};
    PixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    PixelFormatDesc.nVersion = 1;
    PixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    PixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
    PixelFormatDesc.cDepthBits = 32;
    PixelFormatDesc.cAlphaBits = 8;
    PixelFormatDesc.cDepthBits = 24;
    PixelFormatDesc.cStencilBits = 8;
    PixelFormatDesc.iLayerType = PFD_MAIN_PLANE;
    HDC WindowDC = GetDC(Window);
    uint32 SuggestedPixelFormat = ChoosePixelFormat(WindowDC, &PixelFormatDesc);
    //Check if we got a valid suggestion of pixel format
    if (SuggestedPixelFormat)
    {
        //continue only if we succeed in setting the pxel format
        if (SetPixelFormat(WindowDC, SuggestedPixelFormat, &PixelFormatDesc))
        {
            //Creat opengl context
            HGLRC OpenGLRenderingContext = wglCreateContext(WindowDC);

            //Check if we got valid rendering context
            if (OpenGLRenderingContext)
            {
                if (wglMakeCurrent(WindowDC, OpenGLRenderingContext))
                {
                    //initialize glew, if we fail, we should stop initialization of opengl, as we need glew right now.
                    if (glewInit() == GLEW_OK)
                    {
                        //Context attribs, we specify the version we want to use...
                        int32 WGL_ContextAttribs[] = {
                            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                            WGL_CONTEXT_MINOR_VERSION_ARB, 5,
                            WGL_CONTEXT_FLAGS_ARB, 0,
                            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                            0};

                        //Check for "WGL_ARB_create_context" as we may want to specify our own version of opengl
                        if (wglewIsSupported("WGL_ARB_create_context"))
                        {
                            //Create context with specified attributes
                            HGLRC OpenGLRenderingContextARB = wglCreateContextAttribsARB(WindowDC, 0, WGL_ContextAttribs);
                            if (OpenGLRenderingContextARB)
                            {
                                wglMakeCurrent(0, 0);
                                wglDeleteContext(OpenGLRenderingContext);
                                wglMakeCurrent(WindowDC, OpenGLRenderingContextARB);
                            }
                        }
                        //We scucceded in initializing opengl in our window
                        //Print out opengl version...
                        auto *GLVersion = glGetString(GL_VERSION);
                        std::string OGLText("OpenGL VERSION: ");
                        std::string GLVersionStr(reinterpret_cast<const char *>(GLVersion));
                        OUTPUT_DEBUG((OGLText + GLVersionStr + "\n").c_str());
                        InitializationResult = true;
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
        }
    }
    ReleaseDC(Window, WindowDC);
    return InitializationResult;
}