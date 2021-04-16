#include <windows.h>
#include <stdint.h>

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

typedef struct cmd_args_t
{
    char *binary;
    char *binary_args;
} cmd_args;

BOOL parse_args(cmd_args* args, LPSTR cmd_line)
{
    ZeroMemory(args, sizeof(cmd_args));
    int cmd_length = strlen(cmd_line);

    for (int i = 0; i < cmd_length; i++)
    {
        if (*(cmd_line + i) == ' ' || i == cmd_length - 1)
        {
            args->binary = (char*)malloc(i);
            memcpy(args->binary, cmd_line, i);
            args->binary[i] = '\0';
            
            args->binary_args = (char*)malloc(cmd_length - i);
            memcpy(args->binary_args, cmd_line + i, cmd_length - i);
            args->binary_args[cmd_length] = '\0';

            return TRUE;
        }
    }

    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    cmd_args args;
    if (!parse_args(&args, pCmdLine))
    {
        return 0;
    }

    char cmd[MAX_PATH];
    GetModuleFileNameA(NULL, cmd, MAX_PATH);

    for (int i = MAX_PATH - 1; i >= 0; --i)
    {
        if (cmd[i] == '\\')
        {
            ++i;

            if (i + strlen(args.binary) >= MAX_PATH)
            {
                MessageBoxA(NULL, "You have placed your exe in a folder path that is too long.", "Error", MB_OK | MB_ICONERROR);
                return -1;
            }

            for (int j = 0; j < strlen(args.binary); ++j)
            {
                cmd[i + j] = args.binary[j];
            }

            cmd[i + strlen(args.binary)] = ' ';

            for (int j = 0; j < strlen(args.binary_args); ++j)
            {
                cmd[i + strlen(args.binary) + 1 + j] = args.binary_args[j];
            }

            cmd[i + strlen(args.binary) + 1 + strlen(args.binary_args)] = '\0';

            break;
        }
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, (LPSTR)&cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        MessageBoxA(NULL, "Error in starting executable.", "Error", MB_OK | MB_ICONERROR);
    }

    WaitForSingleObject( pi.hProcess, INFINITE );

    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    if (exit_code == 0)
    {
        return 0;
    }

    // Begin crash reporter
    RECT screen;
    HWND desktop_h = GetDesktopWindow();

    if (desktop_h == NULL)
    {
        return 1;
    }

    GetWindowRect(desktop_h, &screen);
    int res_x = screen.right;
    int res_y = screen.bottom;

    char *window_class_name  = "Crash Report";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = window_class_name;
    RegisterClass(&wc);

    int window_width = 700;
    int window_height = 500;

    HWND hwnd = CreateWindowEx(0, window_class_name, window_class_name, WS_OVERLAPPEDWINDOW, 
        res_x/2 - window_width / 2, res_y/2 - window_height/2, window_width, window_height, 
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);

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
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            continue;
        }
    }

    return 0;
}