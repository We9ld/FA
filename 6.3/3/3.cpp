#include "framework.h"
#include "3.h"
#include "Ball.h"
#include <shobjidl.h> 
#include <CommCtrl.h>
#include <vector>
#include <boost/date_time.hpp>

using namespace std;

#pragma warning(disable:4996)
#pragma comment(lib, "ComCtl32.Lib")

#define MAX_LOADSTRING 100
#define ID_TEXTBOX1    1
#define ID_TEXTBOX2    2
#define ID_TEXTBOX3    3
#define ID_BUTTON      4

HINSTANCE hInst;                    
WCHAR szTitle[MAX_LOADSTRING];      
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hStatus;
HWND hRadiusEdit;
HWND hVelocityEdit;
POINT cursorPos;
HWND hMassEdit;
HWND hButton;
Ball* editingBall;
HWND hWnd;

int x_max;
int y_max;
int isStopped = 0;

vector<Ball*> balls;
int BallsCount;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow)) return FALSE;
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

const wchar_t* getWC(const char* c) 
{
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, c, cSize);
    return wc;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY3);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

void startStop() 
{
    if (isStopped) 
    {
        isStopped = 0;
        SetTimer(hWnd, ID_TIMER1, DELTA_TIME, NULL);
    }
    else 
    {
        isStopped = 1;
        KillTimer(hWnd, ID_TIMER1);
    }
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd) return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    static int x, y, size, len, angle, move, isEdit;
    static ofstream ofs;
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EDIT:
            if (isEdit == 0) 
            {
                HMENU h = GetMenu(hWnd);
                int b = EnableMenuItem(h, IDM_STOP, MF_DISABLED | MF_GRAYED);
                DrawMenuBar(hWnd);
                isEdit = 1;
            }
            else 
            {
                isEdit = 0;
                EnableMenuItem(GetMenu(hWnd), IDM_STOP, MF_ENABLED | MF_DEFAULT);
                DrawMenuBar(hWnd);
            }
            startStop();
            break;
        case ID_BUTTON: 
        {
            int len = GetWindowTextLength(hRadiusEdit) + 1;
            wchar_t* text = new wchar_t[len];
            GetWindowText(hRadiusEdit, &text[0], len);
            wstring ws(text);
            delete text;
            string str(ws.begin(), ws.end());
            editingBall->radius = stoi(str);
            len = GetWindowTextLength(hMassEdit) + 1;
            text = new wchar_t[len];
            GetWindowText(hMassEdit, &text[0], len);
            ws = wstring(text);
            delete text;
            str = string(ws.begin(), ws.end());
            editingBall->mass = stoi(str);
            len = GetWindowTextLength(hVelocityEdit) + 1;
            text = new wchar_t[len];
            GetWindowText(hVelocityEdit, &text[0], len);
            std::vector<std::string> results;
            boost::split(results, text, [](char c) {return c == ','; });
            delete text;
            editingBall->velocity.x = stoi(results[0]);
            editingBall->velocity.y = stoi(results[1]);
            InvalidateRect(hWnd, NULL, TRUE);
            ShowWindow(hRadiusEdit, SW_HIDE);
            ShowWindow(hMassEdit, SW_HIDE);
            ShowWindow(hVelocityEdit, SW_HIDE);
            ShowWindow(hButton, SW_HIDE);
            break;
        }
        case IDM_STOP:
            startStop();
            break;
        case IDM_SAVE: 
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileSaveDialog* pFileSave;
                hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                    IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));
                if (SUCCEEDED(hr))
                {
                    hr = pFileSave->Show(NULL);
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        hr = pFileSave->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                            if (SUCCEEDED(hr))
                            {
                                ofstream of(pszFilePath);
                                of << BallsCount << " ";
                                for (Ball* b : balls) 
                                {
                                    of << b->center.x << " "
                                        << b->center.y << " "
                                        << b->velocity.x << " "
                                        << b->velocity.y << " "
                                        << b->mass << " "
                                        << b->radius << " " << endl;
                                }
                                of.close();
                                CoTaskMemFree(pszFilePath);
                            }
                            pItem->Release();
                        }
                    }
                    pFileSave->Release();
                }
                CoUninitialize();
            }
            break;
        }
        case IDM_OPEN: 
        {
            HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
            if (SUCCEEDED(hr))
            {
                IFileOpenDialog* pFileOpen;
                hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                    IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
                if (SUCCEEDED(hr))
                {
                    hr = pFileOpen->Show(NULL);
                    if (SUCCEEDED(hr))
                    {
                        IShellItem* pItem;
                        hr = pFileOpen->GetResult(&pItem);
                        if (SUCCEEDED(hr))
                        {
                            PWSTR pszFilePath;
                            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                            if (SUCCEEDED(hr))
                            {
                                ifstream fi(pszFilePath);
                                fi >> BallsCount;
                                if (balls.size() != 0) balls.clear();
                                double radius;
                                double x, y;
                                double mass;
                                for (int i = 0; i < BallsCount; ++i) 
                                {
                                    fi >> x >> y;
                                    Vector center(x, y);
                                    fi >> x >> y;
                                    Vector velocity(x, y);
                                    fi >> mass >> radius;
                                    balls.push_back(new Ball(hWnd, radius, velocity, center));
                                }
                                fi.close();
                                CoTaskMemFree(pszFilePath);
                            }
                            pItem->Release();
                        }
                    }
                    pFileOpen->Release();
                }
                CoUninitialize();
            }
            break;
        }
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE: 
    {
        ofs.open("log");
        SetTimer(hWnd, ID_TIMER1, DELTA_TIME, NULL);
        BallsCount = 50;
        RECT rect = { 0 };
        GetClientRect(hWnd, &rect);
        x_max = rect.right - rect.left - size;
        y_max = rect.bottom - rect.top - size;
        for (size_t i = 0; i < BallsCount; i++) 
        {
            int size = rand() % 15 + 3;
            Vector velocity(rand() % 10 + 1, rand() % 10 + 1);
            Vector center(rand() % x_max + 2 * size, rand() % y_max + 2 * size + 1);
            balls.push_back(new Ball(hWnd, size, velocity, center));
        }
    }
    break;
    case WM_LBUTTONDOWN:
        if (isEdit) 
        {
            cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(hWnd, &cursorPos);
            for (Ball* b : balls) 
            {
                double distance = sqrt(pow(cursorPos.x - b->center.x, 2) + pow(cursorPos.y - b->center.y, 2));
                if (distance < b->radius)
                {
                    move = 1;
                    editingBall = b;
                }
            }
        }
        break;
    case WM_MOUSEMOVE:
        if (isEdit && move) 
        {
            GetCursorPos(&cursorPos);
            ScreenToClient(hWnd, &cursorPos);
            editingBall->center.x = cursorPos.x;
            editingBall->center.y = cursorPos.y;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_RBUTTONDOWN: 
    {
        if (isEdit) 
        {
            ShowWindow(hRadiusEdit, SW_HIDE);
            ShowWindow(hMassEdit, SW_HIDE);
            ShowWindow(hVelocityEdit, SW_HIDE);
            ShowWindow(hButton, SW_HIDE);
            GetCursorPos(&cursorPos);
            ScreenToClient(hWnd, &cursorPos);
            for (Ball* b : balls)
            {
                double distance = sqrt(pow(cursorPos.x - b->center.x, 2) + pow(cursorPos.y - b->center.y, 2));
                if (distance < b->radius) 
                {
                    hRadiusEdit = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
                        20, 50, 120, 20, hWnd, (HMENU)ID_TEXTBOX1, NULL, NULL);
                    hMassEdit = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
                        20, 80, 120, 20, hWnd, (HMENU)ID_TEXTBOX2, NULL, NULL);
                    hVelocityEdit = CreateWindow(_T("EDIT"), NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE,
                        20, 110, 120, 20, hWnd, (HMENU)ID_TEXTBOX3, NULL, NULL);
                    hButton = CreateWindow(_T("button"), _T("Enter"), WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                        20, 140, 120, 20, hWnd, (HMENU)ID_BUTTON, NULL, NULL);
                    editingBall = b;
                    string str1 = to_string((int)b->radius);
                    wstring ws1(str1.begin(), str1.end());
                    SendMessage(hRadiusEdit, EM_SETSEL, -1, -1);
                    SendMessage(hRadiusEdit, EM_REPLACESEL, 0, (LPARAM)str1.c_str());
                    SetWindowText(hRadiusEdit, ws1.c_str());
                    str1 = to_string((int)b->velocity.x) + "," + to_string((int)b->velocity.y);
                    ws1 = wstring(str1.begin(), str1.end());
                    SendMessage(hVelocityEdit, EM_SETSEL, -1, -1);
                    SendMessage(hVelocityEdit, EM_REPLACESEL, 0, (LPARAM)str1.c_str());
                    SetWindowText(hVelocityEdit, ws1.c_str());
                    str1 = to_string((int)b->mass);
                    ws1 = wstring(str1.begin(), str1.end());
                    SendMessage(hMassEdit, EM_SETSEL, -1, -1);
                    SendMessage(hMassEdit, EM_REPLACESEL, 0, (LPARAM)str1.c_str());
                    SetWindowText(hMassEdit, ws1.c_str());
                    break;
                }
            }
        }
    }
    break;
    case WM_LBUTTONUP: 
    {
        move = 0;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_TIMER: 
    {
        for (Ball* ball : balls) 
        {
            RECT rect = { 0 };
            GetClientRect(hWnd, &rect);
            x_max = rect.right - rect.left - ball->radius / sqrt(2);
            y_max = rect.bottom - rect.top - ball->radius / sqrt(2);
            ball->center = ball->center + ball->velocity;
            int radius = ball->radius;
            ball->checkBorders(0, 0, x_max, y_max);
            for (int i = 0; i < BallsCount - 1; i++)
            {
                for (int j = i + 1; j < BallsCount; j++)
                {
                    if (balls[i]->isCollision(balls[j])) 
                    {
                        while ((balls[i]->isCollision(balls[j]))) 
                        {
                            Vector p1 = balls[i]->center;
                            Vector p2 = balls[j]->center;
                            Vector penetrationDirection = (p2 - p1).normalize();
                            balls[i]->center -= penetrationDirection * 0.5;
                            balls[j]->center += penetrationDirection * 0.5;
                        }
                        ofs << "NUM: " << i << " " << j << "\nCOORD: " << balls[i]->center << "," << balls[j]->center << "\nSPEED1: " << balls[i]->velocity << "," << balls[j]->velocity;
                        Vector p1 = balls[i]->velocity * balls[i]->mass;
                        Vector p2 = balls[j]->velocity * balls[j]->mass;
                        Vector v1 = (p2 * 2.0 + balls[i]->velocity * (balls[i]->mass - balls[j]->mass)) / (balls[i]->mass + balls[j]->mass);
                        Vector v2 = (p1 * 2.0 + balls[j]->velocity * (balls[j]->mass - balls[i]->mass)) / (balls[j]->mass + balls[i]->mass);
                        balls[i]->velocity = v1;
                        balls[j]->velocity = v2;
                        ofs << "\nSPEED2: " << v1 << "," << v2 << endl;
                        balls[i]->checkBorders(0, 0, x_max, y_max);
                        balls[j]->checkBorders(0, 0, x_max, y_max);
                    }
                }
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        for (int i = 0; i < BallsCount; i++) balls[i]->draw(hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: 
    {
        ofs.close();
        while (balls.size()) 
        {
            delete balls[balls.size() - 1];
            balls.pop_back();
        }
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
