#include <windows.h>
#include <gl/gl.h>
#include "menu.h"
#include "texturing.h"
#include "character.h"
#include "collision.h"

float ADD_FRAMETIME = 50.0f;
int SCENE = 0;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void changeScene();
void exitProgram();


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

    //Инициализация кнопок
    Menu_AddButton("Start", 100, 100, 400, 100, 8, changeScene);
    Menu_AddButton("Quit", 100, 250, 400, 100, 8, exitProgram);

    //Инициализация текстур
    unsigned int spriteSheet, background, wall;
    createTexture("src/spritesheet.png", &spriteSheet);
    createTexture("src/background.png", &background);

    //Инициализация матрицы коллизий
    float collisionMatrix[] =
    {
        // x1   // x2   // y1   // y2
        880.0f,  1024.0f, 230.0f, 330.0f, //Обломок
        0.0f,    700.0f,  400.0f, 500.0f, //Верхний ярус
       -100.0f,  1200.0f, 0.0f,   150.0f, //Пол
       -200.0f,  0.0f,    0.0f,   800.0f, //Левая граница
        1020.0f, 1100.0f, 0.0f,   800.0f  //Правая граница
    };

    //Инициализация персонажей
    Character* mainCharacter = createCharacter(-20.0f, 300.0f, spriteSheet, 3, 8);
    addVelocity(mainCharacter, 30, 0);                                              //Крутое появление из за края экрана!


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

            switch(SCENE)
            {
            case 0:
                Menu_ShowMenu();                //Отрисовать меню
                break;
            case 1:
                physics(mainCharacter);         //Обработать физику
                playerControl(mainCharacter);   //Обработать управление
                collision(mainCharacter, collisionMatrix, sizeof(collisionMatrix)); //Обработать коллизии

                drawCharacter(mainCharacter);   //Отрисовать персонажа

                //drawVelocityVector(mainCharacter); //Отрисовать вектор скорости персонажа (дебаг)

                break;
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
                    changeScene();
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

void changeScene()          //Совсем маленький костыль, жесть
{
    if (SCENE)
    {
        SCENE = 0;
    }
    else
    {
        SCENE = 1;
    }
}

void exitProgram()
{
    PostQuitMessage(0);
}

