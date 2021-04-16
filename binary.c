#include <windows.h>
#include <stdint.h>
#include <signal.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    RECT screen;
    HWND desktop_h = GetDesktopWindow();

    if (desktop_h == NULL)
    {
        return 1;
    }

    GetWindowRect(desktop_h, &screen);
    int res_x = screen.right;
    int res_y = screen.bottom;

    char *window_class_name  = "Demo program";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = window_class_name;
    RegisterClass(&wc);

    int window_width = 120;
    int window_height = 80;

    HWND hwnd = CreateWindowEx(0, window_class_name, window_class_name, WS_OVERLAPPEDWINDOW, 
        res_x/2 - window_width / 2, res_y/2 - window_height/2, window_width, window_height, 
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 1;
    }

    HWND button_h = CreateWindow("BUTTON", "crash", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 20, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MessageBoxA(NULL, pCmdLine, "Forwarded command line args", MB_OK);

    MSG msg = {};
    uint8_t running = 1;

    while (running == 1)
    {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = 0;
            }
            else if (msg.hwnd == button_h && msg.message == WM_LBUTTONUP)
            {
                raise(SIGSEGV);
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            continue;
        }
    }

    return 0;
}