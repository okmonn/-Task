#pragma once
#include <Windows.h>

class Window 
{
public:
	// コンストラクタ
	Window(const unsigned int& x, const unsigned int& y);
	// デストラクタ
	~Window();

	// ウィンドウハンドルの取得
	HWND Get(void) const {
		return handle;
	}

private:
	// ウィンドウプロシージャ
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウの設定
	void SetWindow(void);

	// ウィンドウの作成
	void Create(const unsigned int& x, const unsigned int& y);


	// ウィンドウハンドル
	HWND handle;

	// ウィンドウ構造体
	WNDCLASSEX wnd;
};
