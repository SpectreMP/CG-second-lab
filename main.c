#include <windows.h>
#include <gl/gl.h>
#include "menu.h"
#include "texturing.h"
#include "character.h"

float ADD_FRAMETIME = 60.0f;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void Init(){
    Menu_AddButton("Hello", 100, 100, 400, 100, 8, speaker);
    Menu_AddButton("My Name", 100, 250, 400, 100, 8, speaker);
    Menu_AddButton("Is", 100, 400, 400, 100, 8, speaker);
    Menu_AddButton("Giorno", 100, 550, 400, 100, 8, speaker);
    //Character* mainCharacter = createCharacter(300.0f, 300.0f, spritesheet);
}


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1024,
                          768,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    RECT rct;
    GetClientRect(hwnd,&rct);
    glOrtho(0,rct.right, rct.bottom, 0, 1, -1);

    unsigned int spriteSheet, background, wall;
    createTexture("src/spritesheet.png", &spriteSheet);
    createTexture("src/background.png", &background);
    createTexture("src/brick.png", &wall);

    Character* mainCharacter = createCharacter(400.0f, 400.0f, spriteSheet);

    Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            renderImage(1024.0f, 768.0f, 0, 0, background);

            Menu_ShowMenu();

            float xPosition = 500.0f;
            float yPosition = 500.0f;
            float width = 100.0f;
            float height = 100.0f;
            float animationNumber = 2.0f;

            float vertices[] = {
                 // positions                                   // colors           // texture coords
                 xPosition + width, yPosition + height, 0.0f,   1.0f, 1.0f, 1.0f,   0.125f+0.125f*theta, 0.333f * animationNumber,             // top right
                 xPosition + width, yPosition,          0.0f,   1.0f, 1.0f, 1.0f,   0.125+0.125f*theta,  0.333f * (animationNumber - 1.0f),    // bottom right
                 xPosition,         yPosition,          0.0f,   1.0f, 1.0f, 1.0f,   0.0f+0.125f*theta,   0.333f * (animationNumber - 1.0f),    // bottom left
                 xPosition,         yPosition + height, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f+0.125f*theta,   0.333f * animationNumber              // top left
            };


            renderImage(100.0f, 100.0f, 200.0f, 200.0f, wall);

            drawCharacter(mainCharacter);

            if (GetKeyState(VK_LEFT)<0)
            {
                addVelocity(mainCharacter, -10.0f, 0.0f);
                mainCharacter -> turnedAround = true;
            }
            if (GetKeyState(VK_RIGHT)<0)
            {
                addVelocity(mainCharacter, 10.0f, 0.0f);
                mainCharacter -> turnedAround = false;
            }
            if (GetKeyState(VK_UP)<0)
            {
                addVelocity(mainCharacter, 0.0f, 10.0f);
            }
            if (GetKeyState(VK_DOWN)<0)
            {
                addVelocity(mainCharacter, 0.0f, -10.0f);
            }

            SwapBuffers(hDC);

            theta += 1.0f;
            Sleep (1 + ADD_FRAMETIME);

        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_LBUTTONDOWN:
            Menu_MouseDown();
        break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

