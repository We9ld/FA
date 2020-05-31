 // 5.cpp : Defines the entry point for the application.
//


#include "5.h"
#include "Object.h"
#include <vector>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND radioButton1;
HWND radioButton2;
HWND radioButton3;
HWND radioButton4;
HWND radioButton5;
HWND createButton;

#define ID_RADIO1 888
#define ID_RADIO2 889
#define ID_RADIO3 890
#define ID_RADIO4 891
#define ID_RADIO5 892
#define ID_BUTTON 893
bool addObject = false;
bool move = false;
bool size = false;
int movingObjIndex;


std::vector<Object> objects;

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY5));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY5);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   radioButton1 = CreateWindow(L"button", L"Button",
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 10, 120, 30, hWnd, (HMENU)ID_RADIO1, hInstance, NULL);
   radioButton2 = CreateWindow(L"button", L"Edit",
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 40, 120, 30, hWnd, (HMENU)ID_RADIO2, hInstance, NULL);
   radioButton3 = CreateWindow(L"button", L"Label",
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 70, 120, 30, hWnd, (HMENU)ID_RADIO3, hInstance, NULL);
   radioButton4 = CreateWindow(L"button", L"ComboBox",
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 100, 120, 30, hWnd, (HMENU)ID_RADIO4, hInstance, NULL);
   radioButton5 = CreateWindow(L"button", L"spinButton",
       WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
       10, 130, 120, 30, hWnd, (HMENU)ID_RADIO5, hInstance, NULL);

   createButton = CreateWindow(L"button", L"Create",
       WS_CHILD | WS_VISIBLE,
       10, 170, 120, 30, hWnd, (HMENU)ID_BUTTON, hInstance, NULL);

   objects.push_back(Object(radioButton1, 10, 10));
   objects.push_back(Object(radioButton2, 10, 40));
   objects.push_back(Object(radioButton3, 10, 70));
   objects.push_back(Object(radioButton4, 10, 100));
   objects.push_back(Object(radioButton5, 10, 130)); 

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case ID_BUTTON:
                MessageBox(hWnd, TEXT(""), TEXT("Нажмите ЛКМ там, где нужно создать объект"), MB_OK | MB_ICONASTERISK);
                addObject = true;
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MBUTTONDOWN: {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(hWnd, &p);

        bool isFind = false;
        for (int i = 0; i < objects.size(); ++i) {
            Object o = objects[i];
            if ((p.x >= o.getX() && p.x <= o.getX() + o.getWidth()) &&
                (p.y >= o.getY() && p.y <= o.getY() + o.getHight())) {

                size = true;
                movingObjIndex = i;

                //objects.erase(objects.begin() + i);

                break;
            }
        }
    }
        break;

    case WM_MBUTTONUP: {
        size = false;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_RBUTTONDOWN: {
        
            
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);

            bool isFind = false;
            for (int i = 0; i < objects.size(); ++i) {
                Object o = objects[i];
                if ((p.x >= o.getX() && p.x <= o.getX() + o.getWidth()) &&
                    (p.y >= o.getY() && p.y <= o.getY() + o.getHight())) {
                  
                    move = true;
                    movingObjIndex = i;

                    //objects.erase(objects.begin() + i);

                    break;
                }
            }
        
        break;
    }

    case WM_RBUTTONUP:
        move = false;
        break;

    case WM_MOUSEMOVE: {
        if (size) {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);
            bool isCollide = false;
            int h =  objects[movingObjIndex].getY() - p.y;
            int w =  objects[movingObjIndex].getX() - p.x;
            for (int i = 0; i < objects.size(); ++i) {
                Object o = objects[i];
                if (o != objects[movingObjIndex]) {
                    int width = o.getWidth();
                    int hight = o.getHight();
                    int x = o.getX();
                    int y = o.getY();


                    if (((objects[movingObjIndex].getX() + w >= x && objects[movingObjIndex].getX()<= x + width) &&
                        (objects[movingObjIndex].getY() + h >= y && objects[movingObjIndex].getY() <= y + hight)
                        )) {
                        isCollide = true;
                        break;
                    }
                }
            }
            if (!isCollide) {
               
                if (w < 20) {
                    w = 20;
                }

                if (h < 20) {
                    h = 20;
                }
                MoveWindow(objects[movingObjIndex].getHWND(), objects[movingObjIndex].x, objects[movingObjIndex].y,
                   w ,
                   h , TRUE);

            }
        }

        if (move) {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd, &p);
            bool isCollide = false;
            for (int i = 0; i < objects.size(); ++i) {
                
                Object o = objects[i];
                if (o != objects[movingObjIndex]) {
                    int width = o.getWidth();
                    int hight = o.getHight();
                    int h = objects[movingObjIndex].getHight();
                    int w = objects[movingObjIndex].getWidth();
                    int x = o.getX();
                    int y = o.getY();
                    if (((p.x + w >= x && p.x <= x + width) &&
                        (p.y + h >= y && p.y <= y + hight)
                        )) {
                        isCollide = true;
                        break;
                    }
                }
            }

            if (!isCollide) {
                MoveWindow(objects[movingObjIndex].getHWND(), p.x, p.y,
                    objects[movingObjIndex].getWidth(), objects[movingObjIndex].getHight(), TRUE);
                objects[movingObjIndex].x = p.x;
                objects[movingObjIndex].y = p.y;
            }



        }
    }
        break;


    case WM_LBUTTONDOWN:
        if (addObject) {
            for (size_t i = 0; i < 5; i++) {
                if (BST_CHECKED == SendMessage(objects[i].getHWND(), BM_GETCHECK, 0, 0)) {
                    std::wstring str;
                    switch (i){
                    case 0: 
                        str = L"button";
                        break;

                    case 1 :
                        str = L"edit";
                        break;

                    case 2:
                        str = L"STATIC";
                        break;
                        
                    case 3:
                        str = L"COMBOBOX";
                        break;

                    case 4:
                        str = L"SCROLLBAR"; 
                        break;
                    }

                    POINT p;
                    GetCursorPos(&p);
                    ScreenToClient(hWnd, &p);
                    bool isCorrect = true;
                    for (Object o : objects) {
                        int width = o.getWidth();
                        int hight = o.getHight();
                        int x = o.getX();
                        int y = o.getY();
                        if ((p.x + width  >= x && p.x <= x + width) &&  
                            (p.y + hight >= y && p.y <= y + hight)
                            ) {
                            MessageBox(hWnd, TEXT("Объект наезжает на другой объект"), TEXT(""), MB_OK | MB_ICONASTERISK);
                            isCorrect = false;
                            break;
                        }
                    }
                    if (isCorrect) {
                        objects.push_back(Object(CreateWindow((str.c_str()), (str.c_str()),
                            WS_CHILD | WS_VISIBLE | WS_DISABLED,
                            p.x, p.y, 120, 30, hWnd, (HMENU)0, GetModuleHandle(nullptr), NULL), p.x, p.y));
                        SetWindowText(objects[objects.size() - 1].getHWND(), L"AAAAAAAAAA");
                        addObject = false;
                    }
                    break;

                }
            }
            
            
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
