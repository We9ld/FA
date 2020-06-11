// main.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "BrownianMotion.h"
#pragma warning(disable:4996)

//Функция - рандомизатор
int random_from_range(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}


#define MAX_LOADSTRING    100
#define GAMELOOP_TIMER_ID 100

#define IDM_BTN_START  1
#define IDM_BTN_STOP   2
#define IDM_BTN_ADD    3

//Функция для сплита строки на строки по разделителю
void split(std::vector<std::string>& dest, const std::string& str, const char* delim)
{
	char* pTempStr = strdup(str.c_str());
	char* pWord = strtok(pTempStr, delim);
	while (pWord != NULL)
	{
		dest.push_back(pWord);
		pWord = strtok(NULL, delim);
	}

	free(pTempStr);
	
}
/* Глобальные константы */
// Параметры для таймера
constexpr int SPACE_CONTROLS = 200;

constexpr int FPS = 1000 / 60;

constexpr int EDIT_BUFF_SIZE = 10;

// Параметры для окна
constexpr int SCREEN_WIDTH = 1440;
constexpr int SCREEN_HEIGHT = 800;

// Канва
constexpr int CANVAS_LEFT = 10;
constexpr int CANVAS_TOP = 10;
constexpr int CANVAS_RIGHT = 1280;
constexpr int CANVAS_BOTTOM = 720;

// Кнопка "старт"
constexpr int BTN_START_WIDTH = 60;
constexpr int BTN_START_HEIGHT = 35;
constexpr int BTN_START_LEFT_MARGIN = 10;
constexpr int BTN_START_TOP_MARGIN = 10;
constexpr int BTN_START_LEFT = CANVAS_RIGHT;
constexpr int BTN_START_TOP = 0;

// Кнопка "стоп"
constexpr int BTN_STOP_WIDTH = 60;
constexpr int BTN_STOP_HEIGHT = 35;
constexpr int BTN_STOP_LEFT_MARGIN = 10;
constexpr int BTN_STOP_TOP_MARGIN = 10;
constexpr int BTN_STOP_LEFT = CANVAS_RIGHT;
constexpr int BTN_STOP_TOP = BTN_START_TOP + BTN_START_HEIGHT + BTN_START_TOP_MARGIN;

// Кнопка "выход"
constexpr int BTN_EXIT_WIDTH = 60;
constexpr int BTN_EXIT_HEIGHT = 35;
constexpr int BTN_EXIT_LEFT_MARGIN = 10;
constexpr int BTN_EXIT_TOP_MARGIN = 10;
constexpr int BTN_EXIT_LEFT = CANVAS_RIGHT;
constexpr int BTN_EXIT_TOP = BTN_STOP_TOP + BTN_STOP_HEIGHT + BTN_STOP_TOP_MARGIN;

// Кнопка "добавить"
constexpr int BTN_ADD_WIDTH = 60;
constexpr int BTN_ADD_HEIGHT = 35;
constexpr int BTN_ADD_LEFT_MARGIN = 10;
constexpr int BTN_ADD_TOP_MARGIN = 10;
constexpr int BTN_ADD_LEFT = CANVAS_RIGHT;
constexpr int BTN_ADD_TOP = BTN_EXIT_TOP + BTN_EXIT_HEIGHT + BTN_EXIT_TOP_MARGIN + SPACE_CONTROLS;

/* Глобальные константы */

/* Глобальные переменные */
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szFileName[MAX_PATH] = L"";
OPENFILENAME ofn;

// brushes
HPEN pen;
HWND btn_start, btn_stop, btn_exit, btn_add, btn_delete;

// create scene
BrownianScene main_scene(CANVAS_LEFT, CANVAS_TOP, CANVAS_RIGHT, CANVAS_BOTTOM);

int gameloop_timer;

// Позиция мышки
int pos_x, pos_y;

//Перечисление — это тип данных, где любое значение определяется как символьная константа.
enum MouseState {
	NONE,
	IS_DRAG
};

int mouse_state = NONE;

Vector2D center(250, 250), velocity(4.0, 4.0);

// edit buffer for params dialog
wchar_t edit_buff[EDIT_BUFF_SIZE];

int key_goal;
std::shared_ptr<Ball> goal = nullptr;
/* Глобальные переменные */

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Params(HWND, UINT, WPARAM, LPARAM);

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
	LoadStringW(hInstance, IDC_BROWNIANMOTION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BROWNIANMOTION));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BROWNIANMOTION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BROWNIANMOTION);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	case WM_CREATE:
	{
		// init timers
		gameloop_timer = SetTimer(hWnd, GAMELOOP_TIMER_ID, FPS, nullptr);

		// init brushes
		pen = CreatePen(BS_SOLID, 1, BLACK_COLOR);

		// Устанавливаем позицию и размер окна
		SetWindowPos(hWnd, HWND_NOTOPMOST, 10, 10, SCREEN_WIDTH, SCREEN_HEIGHT, NULL);

		if (!main_scene.logfile_is_opened())
			MessageBox(hWnd, L"Log file not opened.", L"Error!", MB_ICONWARNING | MB_OK);

		/* Создание интерфейса */
		btn_start = CreateWindow(
			L"BUTTON",
			L"Start",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			BTN_START_LEFT + BTN_START_LEFT_MARGIN,
			BTN_START_TOP + BTN_START_TOP_MARGIN,
			BTN_START_WIDTH,
			BTN_START_HEIGHT,
			hWnd,
			(HMENU)(IDM_BTN_START),
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		btn_stop = CreateWindow(
			L"BUTTON",
			L"Stop",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			BTN_STOP_LEFT + BTN_STOP_LEFT_MARGIN,
			BTN_STOP_TOP + BTN_STOP_TOP_MARGIN,
			BTN_STOP_WIDTH,
			BTN_STOP_HEIGHT,
			hWnd,
			(HMENU)(IDM_BTN_STOP),
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		btn_exit = CreateWindow(
			L"BUTTON",
			L"Exit",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			BTN_EXIT_LEFT + BTN_EXIT_LEFT_MARGIN,
			BTN_EXIT_TOP + BTN_EXIT_TOP_MARGIN,
			BTN_EXIT_WIDTH,
			BTN_EXIT_HEIGHT,
			hWnd,
			(HMENU)(IDM_EXIT),
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);

		btn_add = CreateWindow(
			L"BUTTON",
			L"Add",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			BTN_ADD_LEFT + BTN_ADD_LEFT_MARGIN,
			BTN_ADD_TOP + BTN_ADD_TOP_MARGIN,
			BTN_ADD_WIDTH,
			BTN_ADD_HEIGHT,
			hWnd,
			(HMENU)(IDM_BTN_ADD),
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);
		/* Конец создания */

		// disable add and delete button
		EnableWindow(btn_add, false);

		// add objects to main scene
		//for (int i = 0; i < 20; i++)
		//{
		//    Vector2D center1(random_from_range(0, 1000), random_from_range(0, 1000));
		//    Ball* b1 = new Ball(center1, velocity, random_from_range(2, 10), i, random_from_range(5, 30));
		//    main_scene.objects.insert(std::make_pair(i, b1));
		//}
	}

	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);

			break;
		case IDM_BTN_ADD:
		{
			// Рандомные значиния для параметров
			int new_id = main_scene.objects.size(),
				radius = random_from_range(15, 45),
				mass = random_from_range(2, 10);

			Vector2D new_center(0, 0);
			Ball* b = new Ball(new_center, velocity, mass, new_id, radius);

			// Нахождение свободного пространства
			bool flag = true;
			do
			{
				b->center.x = random_from_range(CANVAS_LEFT + radius, CANVAS_RIGHT - radius);
				b->center.y = random_from_range(CANVAS_TOP + radius, CANVAS_BOTTOM - radius);

				bool not_inserted = true;
				for (auto curr = main_scene.objects.begin(); curr != main_scene.objects.end(); ++curr)
				{
					std::shared_ptr<Ball> curr_b = curr->second;

					if (b->collide(*curr_b))
					{
						not_inserted = false;
						break;
					}
				}

				if (not_inserted)
					flag = false;
			} while (flag);

			main_scene.objects.insert(std::make_pair(new_id, b));
		}

		break;
		case IDM_BTN_START:
			main_scene.is_freeze = false;

			// disable delete and add buttons
			EnableWindow(btn_add, false);
			break;
		case IDM_BTN_STOP:
			main_scene.is_freeze = true;

			// enable delete and add buttons
			EnableWindow(btn_add, true);
			break;

		case ID_FILE_LOAD:
		{
			// инициализация структуры

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = (LPCWSTR)L"txt";
			ofn.lpstrFile = szFileName;  // no cast needed
			ofn.nMaxFile = MAX_PATH;

			GetOpenFileNameW(&ofn);

			std::ifstream config_fs(ofn.lpstrFile, std::ios_base::in);

			int id;
			double x, y, vx, vy, mass, radius;

			main_scene.objects.clear();

			// Делим файл на строки
			std::string buff;
			std::vector<std::string> parts;
			std::string buffer = "";
			
			while (getline(config_fs, buff))
			{
				
				split(parts, buff, ",");
				id = std::stoi(parts[0]);
				x = std::stod(parts[1]);
				y = std::stod(parts[2]);
				vx = std::stod(parts[3]);
				vy = std::stod(parts[4]);
				mass = std::stod(parts[5]);
				radius = std::stod(parts[6]);

				Vector2D new_center(x, y),
					new_velocity(vx, vy);

				Ball* b = new Ball(new_center, new_velocity, mass, id, radius);
				main_scene.objects.insert(std::make_pair(id, b));
				parts.clear();
			}
		}

		break;
		case ID_FILE_SAVE:
		{
			// инициализируем структуру
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = (LPCWSTR)L"txt";
			ofn.lpstrFile = szFileName;  // no cast needed
			ofn.nMaxFile = MAX_PATH;

			GetSaveFileNameW(&ofn);

			std::ofstream config_fs(ofn.lpstrFile, std::ios_base::out | std::ios_base::trunc);
			//Пишем координаты тел через запятую, новое тело-новая строка
			for (auto curr = main_scene.objects.begin(); curr != main_scene.objects.end(); ++curr)
			{
				std::shared_ptr<Ball> b = curr->second;

				config_fs << b->get_id() << ","
					<< b->center.x << ","
					<< b->center.y << ","
					<< b->velocity.x << ","
					<< b->velocity.y << ","
					<< b->mass << ","
					<< b->radius << "\n";
			}
		}

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

		SelectObject(hdc, pen);

		// render main scene
		main_scene.draw(hdc);

		EndPaint(hWnd, &ps);
	}

	break;
	case WM_LBUTTONUP:
		mouse_state = NONE;

		break;
	case WM_LBUTTONDOWN:
		if (main_scene.is_freeze && goal != nullptr)
		{
			mouse_state = IS_DRAG;
		}

		break;
	case WM_LBUTTONDBLCLK:
		if (main_scene.is_freeze && goal != nullptr)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PARAMS_OBJ_DIALOG), hWnd, Params);
		}

		break;
	case WM_MOUSEMOVE:
	{
		pos_x = GET_X_LPARAM(lParam);
		pos_y = GET_Y_LPARAM(lParam);

		if (main_scene.is_freeze &&
			pos_x > CANVAS_LEFT &&
			pos_x < CANVAS_RIGHT &&
			pos_y > CANVAS_TOP &&
			pos_y < CANVAS_BOTTOM)
		{
			if (mouse_state == NONE)
			{
				goal = nullptr;

				for (auto curr = main_scene.objects.begin(); curr != main_scene.objects.end(); ++curr)
				{
					std::shared_ptr<Ball> b(curr->second);

					if (pos_x > b->center.x - b->radius &&
						pos_x < b->center.x + b->radius &&
						pos_y > b->center.y - b->radius &&
						pos_y < b->center.y + b->radius)
					{
						goal = b;
						key_goal = curr->first;

						break;
					}
				}
			}
			else if (mouse_state == IS_DRAG)
			{
				goal->center.x = pos_x;
				goal->center.y = pos_y;

				if (goal->center.x + goal->radius > CANVAS_RIGHT)
				{
					// проверка пересечения объектом правой границы экрана
					goal->center.x = CANVAS_RIGHT - goal->radius;
				}
				else if (goal->center.x - goal->radius < CANVAS_LEFT)
				{
					// проверка пересечения объектом левой границы экрана
					goal->center.x = CANVAS_LEFT + goal->radius;
				}
				else if (goal->center.y + goal->radius > CANVAS_BOTTOM)
				{
					// проверка пересечения объектом нижней границы экрана
					goal->center.y = CANVAS_BOTTOM - goal->radius;
				}
				else if (goal->center.y - goal->radius < CANVAS_TOP)
				{
					// проверка пересечения объектом верхней границы экрана
					goal->center.y = CANVAS_TOP + goal->radius;
				}
			}

		}
	}

	break;

	case WM_TIMER:
	{
		if (wParam == gameloop_timer)
		{
			main_scene.update();
		}

		// очистка экрана
		InvalidateRect(hWnd, nullptr, true);
	}

	break;
	case WM_DESTROY:
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
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}

	return (INT_PTR)FALSE;
}

// Message handler for property box.
INT_PTR CALLBACK Params(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	std::wstringstream ws;
	std::wstring tmp_value;

	HWND h_mass_edit, h_radius_edit, h_vx_edit, h_vy_edit;

	h_mass_edit = GetDlgItem(hDlg, IDC_MASS_EDIT);
	h_radius_edit = GetDlgItem(hDlg, IDC_RADIUS_EDIT);
	h_vx_edit = GetDlgItem(hDlg, IDC_VX_EDIT);
	h_vy_edit = GetDlgItem(hDlg, IDC_VY_EDIT);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// set title of dialog
		ws << "Object ID: " << goal->get_id();
		SetWindowText(hDlg, ws.str().c_str());

		// set mass to mass edit field
		SetWindowText(h_mass_edit, std::to_wstring(goal->mass).c_str());

		// set radius to radius edit field
		SetWindowText(h_radius_edit, std::to_wstring(goal->radius).c_str());

		// set velocity x
		SetWindowText(h_vx_edit, std::to_wstring(goal->velocity.x).c_str());

		// set velocity y
		SetWindowText(h_vy_edit, std::to_wstring(goal->velocity.y).c_str());
	}

	return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_DELETE:
			main_scene.objects.erase(key_goal);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDOK:
		{
			double mass, radius, vx, vy;

			// get and validate mass
			GetWindowText(h_mass_edit, edit_buff, EDIT_BUFF_SIZE);
			tmp_value = edit_buff;

			try
			{
				mass = std::stod(tmp_value);
			}
			catch (const std::invalid_argument& e)
			{
				MessageBox(hDlg, L"Mass must be number.", L"Error!", MB_ICONWARNING | MB_OK);

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			if (mass > 0)
			{
				goal->mass = mass;
			}
			else
			{
				MessageBox(hDlg, L"Mass must be higher zero.", L"Error!", MB_ICONWARNING | MB_OK);
			}

			// получение и проверка радиуса
			GetWindowText(h_radius_edit, edit_buff, EDIT_BUFF_SIZE);
			tmp_value = edit_buff;

			try
			{
				radius = std::stod(tmp_value);
			}
			catch (const std::invalid_argument& e)
			{
				MessageBox(hDlg, L"Radius must be number.", L"Error!", MB_ICONWARNING | MB_OK);

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			if (radius > 0)
			{

				// check if goal ball collide with something ball
				for (auto curr = main_scene.objects.begin(); curr != main_scene.objects.end(); ++curr)
				{
					std::shared_ptr<Ball> b(curr->second);

					if (b != goal && b->collide(*goal))
					{
						MessageBox(hDlg, L"Intersection with a ball.", L"Error!", MB_ICONWARNING | MB_OK);

						EndDialog(hDlg, LOWORD(wParam));
						return (INT_PTR)TRUE;;
					}
				}

				goal->radius = radius;
			}
			else
			{
				MessageBox(hDlg, L"Radius must be higher zero.", L"Error!", MB_ICONWARNING | MB_OK);
			}

			// get and validate velocity x
			GetWindowText(h_vx_edit, edit_buff, EDIT_BUFF_SIZE);
			tmp_value = edit_buff;

			try
			{
				vx = std::stod(tmp_value);
			}
			catch (const std::invalid_argument& e)
			{
				MessageBox(hDlg, L"Velocity X must be number.", L"Error!", MB_ICONWARNING | MB_OK);

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			goal->velocity.x = vx;

			// get and validate velocity y
			GetWindowText(h_vy_edit, edit_buff, EDIT_BUFF_SIZE);
			tmp_value = edit_buff;

			try
			{
				vy = std::stod(tmp_value);
			}
			catch (const std::invalid_argument& e)
			{
				MessageBox(hDlg, L"Velocity Y must be number.", L"Error!", MB_ICONWARNING | MB_OK);

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			goal->velocity.y = vy;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		}

		break;
	}

	return (INT_PTR)FALSE;
}
