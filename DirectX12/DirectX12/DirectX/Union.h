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
class Constant;
class RootSignature;
class Compiler;
class Pipe;
class Fence;
class Barrier;

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
	void ChangeWindowSize(UINT x, UINT y);

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


	std::shared_ptr<Window>win;
#ifdef _DEBUG
	// デバッグ
	std::shared_ptr<Debug>debug;
#endif
	std::shared_ptr<Device>dev;
	std::shared_ptr<Queue>queue;
	std::shared_ptr<List>list;
	std::shared_ptr<Swap>swap;
	std::shared_ptr<Render>render;
	std::shared_ptr<Depth>depth;
	std::shared_ptr<Constant>constant;
	std::shared_ptr<RootSignature>root;
	std::shared_ptr<Pipe>pipe;
	std::shared_ptr<Fence>fence;
	std::shared_ptr<Barrier>barrier;
};

