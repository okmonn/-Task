#pragma once
#include <memory>

class Window;
class Debug;
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Constant;

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

	// ウィンドウサイズのセット
	void SetWinSize(const unsigned int& x, const unsigned int& y);

	// クラスのインスタンス
	void Create(void);

	// ウィンドウサイズXの取得
	unsigned int GetWinX(void) const {
		return x;
	}
	// ウィンドウサイズYの取得
	unsigned int GetWinY(void) const {
		return y;
	}

private:
	// コンストラクタ
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}


	// ウィンドウサイズX
	unsigned int x;

	// ウィンドウサイズY
	unsigned int y;

	// ウィンドウ
	std::shared_ptr<Window>win;

	// デバッガ
	std::shared_ptr<Debug>deg;

	// デバイス
	std::shared_ptr<Device>dev;

	// コマンドキュー
	std::shared_ptr<Queue>que;

	// コマンドリスト
	std::shared_ptr<List>list;

	// スワップチェイン
	std::shared_ptr<Swap>swap;

	// レンダーターゲット
	std::shared_ptr<Render>ren;

	// 深度ステンシル
	std::shared_ptr<Depth>dep;

	// 定数バッファ
	std::shared_ptr<Constant>con;
};
