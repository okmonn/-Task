#pragma once
#include <Windows.h>

class Window
{
public:
	// コンストラクタ
	Window(UINT x, UINT y);
	// デストラクタ
	~Window();

	// ウィンドウの生成
	void Create(void);

	// ウィンドウハンドルの取得
	HWND GetHandle(void) const {
		return handle;
	}

	// Xサイズの取得
	UINT GetX(void) const {
		return x;
	}
	// Yサイズの取得
	UINT GetY(void) const {
		return y;
	}

private:
	// ウィンドウプロシージャ
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウ要素のセット
	void SetWindow(void);

	// ウィンドウサイズのセット
	void SetRect(void);


	// ウィンドウハンドル
	HWND handle;

	//  ウィンドウ
	WNDCLASSEX window;

	// サイズ
	RECT rect;

	// Xサイズ
	UINT x;

	// Yサイズ
	UINT y;
};

