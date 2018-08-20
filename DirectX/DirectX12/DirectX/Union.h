#pragma once
#include "../etc/Vector2.h"
#include <d3d12.h>
#include <string>
#include <memory>

class Window;
class Xaudio2;
class Input;
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
class Root;
class Pipe;
class Constant;
class Texture;

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

	// キー入力
	bool CheckKey(UINT index);

	// トリガーキー入力
	bool TriggerKey(UINT index);

	// 画像読み込み
	void LoadImg(UINT& index, const std::string& fileName);

	// 描画
	void Draw(UINT& index, const Vec2f& pos, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定・分割
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rect, const Vec2f& rectSize, UINT turnX = 0, UINT turnY = 0);

	// WAVE読み込み
	void LoadWave(UINT& index, const std::string& fileName);

	// WAVEの再生
	void PlayWave(UINT& index);

	// WAVEの再生停止
	void StopWave(UINT& index);

private:
	// コンストラクタ
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}

	// ビューポートのセット
	void ViewPort(void);

	// シザーのセット
	void Scissor(void);

	// バリアのセット
	void Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

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

	// バリア
	D3D12_RESOURCE_BARRIER barrier;

	// ウィンドウ
	std::shared_ptr<Window>win;

	// オーディオ
	std::shared_ptr<Xaudio2>audio;

	// インプット
	std::shared_ptr<Input>input;

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

	// ルートシグネチャ
	std::shared_ptr<Root>root;

	// パイプライン
	std::shared_ptr<Pipe>pipe;

	// 定数バッファ
	std::shared_ptr<Constant>constant;

	// テクスチャ
	std::shared_ptr<Texture>tex;
};
