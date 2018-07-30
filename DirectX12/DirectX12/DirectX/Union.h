#pragma once
#include <Windows.h>
#include <memory>

class Window;
#ifdef _DEBUG
class Debug;
#endif
class Device;
class Allocator;
class Queue;
class List;
class Swap;

class Union
{
public:
	// デストラクタ
	~Union();

	// インスタンス変数の取得
	static Union& Get(void) {
		static Union instance;
		return instance;
	}

	// クラスのインスタンス化
	void Create(void);

	// ウィンドウのサイズセット
	void ChangeWindowSize(unsigned int x, unsigned int y);

	// メッセージの確認
	bool CheckMsg(void);

private:
	// コンストラクタ
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}


	// 画面サイズX
	unsigned int x;

	// 画面サイズY
	unsigned int y;

	// メッセージ
	MSG msg;


	std::shared_ptr<Window>win;
#ifdef _DEBUG
	// デバッグ
	std::shared_ptr<Debug>debug;
#endif
	std::shared_ptr<Device>dev;
	std::shared_ptr<Allocator>allo;
	std::shared_ptr<Queue>queue;
	std::shared_ptr<List>list;
	std::shared_ptr<Swap>swap;
};

