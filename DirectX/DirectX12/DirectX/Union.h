#pragma once
#include <d3d12.h>
#include <memory>

class Window;
#ifdef _DEBUG
class Debug;
#endif
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Fence;

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

	// ウィンドウのサイズセット
	void ChangeWindowSize(UINT x, UINT y);

	// クラスのインスタンス化
	void Create(void);

	// メッセージの確認
	bool CheckMsg(void);

	// 描画準備
	void Set(void);

	// 実行
	void Do(void);

private:
	// コンストラクタ
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}

	// 画面サイズX
	UINT x;

	// 画面サイズY
	UINT y;

	// メッセージ
	MSG msg;

	// ビューポート
	D3D12_VIEWPORT viewPort;

	// シザー
	RECT scissor;

	// ウィンドウ
	std::shared_ptr<Window>win;

#ifdef _DEBUG
	std::shared_ptr<Debug>debug;
#endif

	// デバイス
	std::shared_ptr<Device>dev;

	// コマンドキュー
	std::shared_ptr<Queue>queue;

	// コマンドリスト
	std::shared_ptr<List>list;

	// スワップチェイン
	std::shared_ptr<Swap>swap;

	// レンダーターゲット
	std::shared_ptr<Render>render;

	// 深度ステンシル
	std::shared_ptr<Depth>depth;

	// フェンス
	std::shared_ptr<Fence>fence;
};

