#include "Window.h"
#include "Icon/Icon.h"
#include <tchar.h>

// コンストラクタ
Window::Window(const unsigned int& x, const unsigned int& y) :
	handle(nullptr)
{
	wnd = {};

	SetWindow();
	Create(x, y);
}

// デストラクタ
Window::~Window()
{
	UnregisterClass(wnd.lpszClassName, wnd.hInstance);
}

// ウィンドウプロシージャ
LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ウィンドウが破棄されたとき
	if (msg == WM_DESTROY)
	{
		//OSに対してアプリケーション終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//規定の処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// ウィンドウの設定
void Window::SetWindow(void)
{
	wnd.cbClsExtra    = 0;
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.cbWndExtra    = 0;
	wnd.hbrBackground = CreateSolidBrush(0x000000);
	wnd.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(ICON_ID));
	wnd.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON_ID));
	wnd.hInstance     = GetModuleHandle(0);
	wnd.lpfnWndProc   = (WNDPROC)WindowProcedure;
	wnd.lpszClassName = _T("DirectX12");
	wnd.lpszMenuName  = _T("DirectX12");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;

	// ウィンドウの登録
	RegisterClassEx(&wnd);
}

// ウィンドウの作成
void Window::Create(const unsigned int& x, const unsigned int& y)
{
	RECT rect = {};
	rect.bottom = y;
	rect.left   = 0;
	rect.right  = x;
	rect.top    = 0;

	//サイズの調節
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	handle = CreateWindow(wnd.lpszClassName, _T("おかもん"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);

	//表示
	ShowWindow(handle, SW_SHOW);
}
