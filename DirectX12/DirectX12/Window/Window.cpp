#include "Window.h"
#include "../Icon/Icon.h"
#include <tchar.h>

// コンストラクタ
Window::Window(UINT x, UINT y) : x(x), y(y), handle(nullptr)
{
	window = {};
	rect = {};

	Create();
}

// デストラクタ
Window::~Window()
{
	//ウィンドウクラスの消去,メモリの解放
	UnregisterClass(window.lpszClassName, window.hInstance);
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

// ウィンドウ要素のセット
void Window::SetWindow(void)
{

	window.cbClsExtra    = 0;
	window.cbSize        = sizeof(WNDCLASSEX);
	window.cbWndExtra    = 0;
	window.hbrBackground = CreateSolidBrush(0x000000);
	window.hCursor       = LoadCursor(NULL, IDC_ARROW);
	window.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(ICON_ID));
	window.hIconSm       = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(ICON_ID));
	window.hInstance     = GetModuleHandle(0);
	window.lpfnWndProc   = (WNDPROC)WindowProcedure;
	window.lpszClassName = _T("DirectX12");
	window.lpszMenuName  = _T("DirectX12");
	window.style         = CS_HREDRAW;

	// ウィンドウの登録
	RegisterClassEx(&window);
}

// ウィンドウサイズのセット
void Window::SetRect(void)
{
	rect.bottom = y;
	rect.left   = 0;
	rect.right  = x;
	rect.top    = 0;

	// サイズの補正
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
}

// ウィンドウの生成
void Window::Create(void)
{
	SetWindow();
	SetRect();

	handle = CreateWindow(window.lpszClassName, _T("おかもん"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, window.hInstance, nullptr);

	//表示させたいウィンドウハンドル,表示の指定(SW_からなる定義使用)
	ShowWindow(handle, SW_SHOW);
}
