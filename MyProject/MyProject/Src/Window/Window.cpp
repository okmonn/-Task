#include "Window.h"
#include "Icon/Icon.h"
#include <tchar.h>

// �R���X�g���N�^
Window::Window(const unsigned int& x, const unsigned int& y) :
	handle(nullptr)
{
	wnd = {};

	SetWindow();
	Create(x, y);
}

// �f�X�g���N�^
Window::~Window()
{
	UnregisterClass(wnd.lpszClassName, wnd.hInstance);
}

// �E�B���h�E�v���V�[�W��
LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//�E�B���h�E���j�����ꂽ�Ƃ�
	if (msg == WM_DESTROY)
	{
		//OS�ɑ΂��ăA�v���P�[�V�����I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�K��̏������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// �E�B���h�E�̐ݒ�
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

	// �E�B���h�E�̓o�^
	RegisterClassEx(&wnd);
}

// �E�B���h�E�̍쐬
void Window::Create(const unsigned int& x, const unsigned int& y)
{
	RECT rect = {};
	rect.bottom = y;
	rect.left   = 0;
	rect.right  = x;
	rect.top    = 0;

	//�T�C�Y�̒���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	handle = CreateWindow(wnd.lpszClassName, _T("��������"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);

	//�\��
	ShowWindow(handle, SW_SHOW);
}
