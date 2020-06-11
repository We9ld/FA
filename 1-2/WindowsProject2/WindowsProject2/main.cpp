// main.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "FlyingFigure.h"

#define MAX_LOADSTRING   100
#define AMOUNT_OBJECTS   4
#define INFO_MAX_LENGTH  250
#define COUNT_PEN_STYLE  5
#define MAX_BORDER_WIDTH 4

#define RENDER_TIMER_EVENT 100
#define STYLE_TIMER_EVENT  101

/* helper function */

COLORREF get_random_color() {
	return RGB(rand() % 256, rand() % 256, rand() % 256);
}

int random_from_range(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

int my_abs(int x) { // !!!
	return x > 0 ? x : -x;
}

/* */

constexpr int FPS = 1000 / 60;
constexpr int INFO_PER_INTERVAL = 2000;

// Global Variables:
int iStatusBarWidths[1];
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int render_timer_id,                            // id of timer that render figure on canvas
style_timer_id;                             // id of timer that update style of figure 

int br_timer;

RECT window_rect;
RECT rcStatusRect;
HWND g_hStatusBar;
HBRUSH brush;
HPEN pen;
COLORREF color = get_random_color();

const std::wstring border_type[] = {
	L"solid",
	L"dashed",
	L"dotted",
	L"dashes and dots",
	L"dashes and double dots"
};

std::wstring text;

int screen_width, screen_height;                // size of screen

unsigned int width = 100;                       // size of figure

Vector2D direction(1, 1),                       // direction
curr_pos(400, 250);                    // current position

int speed = 10;                                 // speed of figure

// states
enum TypeFigure
{
	CIRCLE,
	TRIANGLE,
	RHOMBUS,
	PENTAGON,
};

int curr_figure = CIRCLE;

bool is_moving = true,
is_changing = true,
is_Brownian_motion = false;

// init figures
Circle   circle(width, speed, curr_pos, direction);
Triangle triangle(width, speed, curr_pos, direction);
Rhombus  rhombus(width, speed, curr_pos, direction);
Pentagon pentagon(width, speed, curr_pos, direction);

Figure* objects[] = {
	&circle,
	&triangle,
	&rhombus,
	&pentagon,
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); //Это объявление в начале программы необходимо потому, что в тексте функции WinMain имеются ссылки на функцию WndProc.
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
	LoadStringW(hInstance, IDC_FLYINGFIGURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Проверка инициализации приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLYINGFIGURE));

	MSG msg;

	// Основной цикл сообщений:
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


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;// создаём экземпляр, для обращения к членам класса WNDCLASSEXW
	
	wcex.cbSize = sizeof(WNDCLASSEX); // величина структуры (в байтах)

	wcex.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wcex.lpfnWndProc = WndProc;// указатель на имя пользовательской функции
	wcex.cbClsExtra = 0;// число освобождаемых байтов при создании экземпляра приложения
	wcex.cbWndExtra = 0;// число освобождаемых байтов в конце структуры
	
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLYINGFIGURE));// дескриптор значка
	wcex.hInstance = hInstance;// .... экземпляра приложения
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);// .... курсора мыши
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// .... цвета фона окошка
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FLYINGFIGURE);// указатель на const-строку, содержащюю имя меню, применяемого для класса
	wcex.lpszClassName = szWindowClass;// указатель на const-строку, содержащюю имя класса
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));// .... маленького значка (в трэе)

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

	if (!hWnd)
	{
		return FALSE;
	}

	g_hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)200, hInstance, NULL);

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

	switch (message) {
	case WM_CREATE:
		br_timer = SetTimer(hWnd, 102, 200, nullptr);
		render_timer_id = SetTimer(hWnd, RENDER_TIMER_EVENT, FPS, nullptr);
		style_timer_id = SetTimer(hWnd, STYLE_TIMER_EVENT, INFO_PER_INTERVAL, nullptr);

		// Размер
		GetWindowRect(hWnd, &window_rect);
		GetClientRect(hWnd, &rcStatusRect);
		iStatusBarWidths[0] = rcStatusRect.right;
		screen_width = window_rect.right - window_rect.left;
		screen_height = window_rect.bottom - window_rect.top;

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Выбор в "меню":
		switch (wmId)
		{
		case ID_FIGURE_CIRCLE:
			objects[CIRCLE]->center = objects[curr_figure]->center;
			objects[CIRCLE]->direction = objects[curr_figure]->direction;
			curr_figure = CIRCLE;

			break;
		case ID_FIGURE_TRIANGLE:
			objects[TRIANGLE]->center = objects[curr_figure]->center;
			objects[TRIANGLE]->direction = objects[curr_figure]->direction;
			curr_figure = TRIANGLE;

			break;
		case ID_FIGURE_RHOMBUS:
			objects[RHOMBUS]->center = objects[curr_figure]->center;
			objects[RHOMBUS]->direction = objects[curr_figure]->direction;
			curr_figure = RHOMBUS;

			break;
		case ID_FIGURE_PENTAGON:
			objects[PENTAGON]->center = objects[curr_figure]->center;
			objects[PENTAGON]->direction = objects[curr_figure]->direction;
			curr_figure = PENTAGON;

			break;
		case ID_SET_MOTION:
			for (int i = 0; i < AMOUNT_OBJECTS; ++i)
				objects[i]->speed = is_moving ? 0 : speed;

			is_moving = !is_moving;

			break;
		case ID_SET_CHANGE:
			is_changing = !is_changing;

			break;
		case ID_SET_BROWNIAN_MOTION:
			is_Brownian_motion = !is_Brownian_motion;

			if (!is_Brownian_motion)
			{
				// set default direction
				objects[curr_figure]->direction = direction;
			}

			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);

			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// set brush
		SelectObject(hdc, brush);

		// set pen
		SelectObject(hdc, pen);

		// render information on canvas
		LPCWSTR result_info_msg = text.c_str();
		//TextOut(hdc, (int) screen_width * 0.05, (int) screen_height * 0.9, result_info_msg, wcslen(result_info_msg));
		SendMessage(g_hStatusBar, SB_SETTEXT, 0, (LPARAM)result_info_msg);

		// render object on canvas
		objects[curr_figure]->draw(hdc);

		EndPaint(hWnd, &ps);

		break;
	}
	case WM_TIMER:
		if (wParam == render_timer_id)
		{
		


			if (objects[curr_figure]->center.x + (width / 2) > screen_width) {
				// проверка пересечения объектом правой границы экрана
				objects[curr_figure]->center.x -= objects[curr_figure]->center.x + (width / 2) - screen_width;
				objects[curr_figure]->direction.x *= -1;
			}
			else if (objects[curr_figure]->center.x < width / 2) {
				// проверка пересечения объектом левой границы экрана
				objects[curr_figure]->center.x = width / 2 + 1;
				objects[curr_figure]->direction.x *= -1;
			}
			else if (objects[curr_figure]->center.y + (width / 2) > screen_height) {
				// проверка пересечения объектом нижней границы экрана
				objects[curr_figure]->center.y -= objects[curr_figure]->center.y + (width / 2) - screen_height;
				objects[curr_figure]->direction.y *= -1;
			}
			else if (objects[curr_figure]->center.y < width / 2) {
				// проверка пересечения объектом верхней границы экрана
				objects[curr_figure]->center.y = width / 2 + 1;
				objects[curr_figure]->direction.y *= -1;
			}

			objects[curr_figure]->update();
			
		}
		else if (wParam == style_timer_id)
		{
			std::wstringstream sinfo;

			if (is_changing) {
				// generate random style
				int r = rand() % 256,
					g = rand() % 256,
					b = rand() % 256;

				color = RGB(r, g, b);
				brush = CreateSolidBrush(color);
				sinfo << L"Brush color: " << "RGB(" << r << "," << g << "," << b << ")";

				r = rand() % 256;
				g = rand() % 256;
				b = rand() % 256;

				int w = rand() % MAX_BORDER_WIDTH;
				color = RGB(r, g, b);

				int type_pen = rand() % COUNT_PEN_STYLE;

				pen = CreatePen(type_pen, w, color);
				sinfo << "; " << L"Pen color: " << "RGB(" << r << "," << g << "," << b << ")";
				sinfo << "; " << L"Width: " << w;
				sinfo << "; " << L"Border type: " << border_type[type_pen];

				// generate info string
				text = sinfo.str();
			}

		}
		else if (wParam == br_timer && is_Brownian_motion) {
			// update direction
			objects[curr_figure]->direction.x = random_from_range(-2, 2);
			objects[curr_figure]->direction.y = random_from_range(-2, 2);

			if (objects[curr_figure]->center.x + (width / 2) > screen_width)
				// проверка пересечения объектом правой границы экрана
			{
				objects[curr_figure]->center.x -= objects[curr_figure]->center.x + (width / 2) - screen_width;
			}
			else if (objects[curr_figure]->center.x < width / 2)
				// проверка пересечения объектом левой границы экрана
			{
				objects[curr_figure]->center.x = width / 2 + 1;
			}
			else if (objects[curr_figure]->center.y + (width / 2) > screen_height)
				// проверка пересечения объектом нижней границы экрана
			{
				objects[curr_figure]->center.y -= objects[curr_figure]->center.y + (width / 2) - screen_height;
			}
			else if (objects[curr_figure]->center.y < width / 2)
				// проверка пересечения объектом верхней границы экрана
			{
				objects[curr_figure]->center.y = width / 2 + 1;
			}

			// update position
			objects[curr_figure]->update();
		}

		// clear screen
		InvalidateRect(hWnd, nullptr, true);

		break;
	case WM_SIZE:
		screen_width = LOWORD(lParam);
		screen_height = HIWORD(lParam);
		InvalidateRect(hWnd, nullptr, FALSE);

		SendMessage(g_hStatusBar, SB_SETPARTS, (WPARAM)1, (LPARAM)iStatusBarWidths);
		SendMessage(g_hStatusBar, WM_SIZE, 0, 0);

		break;
	case WM_DESTROY:
		DeleteObject(brush);
		DeleteObject(pen);

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
