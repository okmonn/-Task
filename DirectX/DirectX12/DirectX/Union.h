#pragma once
#include "../etc/Vector2.h"
#include "../etc/Vector3.h"
#include <d3d12.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace fs = std::experimental::filesystem;

class Window;
class Xaudio2;
class MIDI_IN;
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
class Compiler;
class Pipe;
class Constant;
class Point;
class Box;
class Texture;
class PMD;

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

	// ビュー行列のセット
	void ChangeView(const Vec3f& pos, const Vec3f& target, const Vec3f& up);

	// 描画準備
	void Set(void);

	// 実行
	void Do(void);

	// キー入力
	bool CheckKey(UINT index);

	// トリガーキー入力
	bool TriggerKey(UINT index);

	// α値の変更
	void SetAlpha(float alpha) {
		this->alpha = alpha;
	}

	// 点の描画
	void DrawPoint(const Vec2f& pos, const Vec3f& color);

	// ボックスの描画
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Vec3f& color);

	// 画像読み込み
	void LoadImg(UINT& index, const std::string& fileName);

	// 描画
	void Draw(UINT& index, const Vec2f& pos, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定・分割
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rect, const Vec2f& rectSize, UINT turnX = 0, UINT turnY = 0);

	// 画像の消去
	void DeleteImg(UINT& index);

	// PMD読み込み
	void LoadPMD(UINT& index, const std::string& fileName);

	// PMD描画
	void DrawPMD(UINT& index);

	// PMDの消去
	void DeletePMD(UINT& index);

	// WAVE読み込み
	void LoadWave(UINT& index, const std::string& fileName);

	// WAVEの再生
	void PlayWave(UINT& index);

	// WAVEの再生停止
	void StopWave(UINT& index);

	// WAVEの消去
	void DeleteWAVE(UINT& index);

	// MIDIデバイス数の取得
	UINT GetMidiNum(void);

	// MIDIステータスバイトの取得
	UCHAR GetMidiState(void);

	// MIDIデータバイト1の取得
	UCHAR GetMidiData1(void);

	// MIDIデータバイト2の取得
	UCHAR GetMidiData2(void);

	// ディレクトリのファイル列挙
	std::vector<std::string> GetDirFile(const std::string& point);
	std::vector<std::wstring> GetDirFile(const std::wstring& point);

private:
	// コンストラクタ
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}

	// ルートシグネチャの生成
	void CreateRoot(void);

	// パイプラインの生成
	void CreatePipeLine(void);

	// ビューポートのセット
	void ViewPort(void);

	// シザーのセット
	void Scissor(void);

	// バリアのセット
	void Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

	// ファイルを返す
	std::string GetFile(const fs::path& p);
	std::wstring GetFileW(const fs::path& p);


	// 画面サイズX
	UINT x;

	// 画面サイズY
	UINT y;

	// アルファ値
	float alpha;

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

	// MIDI入力
	std::shared_ptr<MIDI_IN>in;

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
	std::shared_ptr<Root>pointRoot;
	std::shared_ptr<Root>texRoot;
	std::shared_ptr<Root>modelRoot;

	// シェーダコンパイル
	std::shared_ptr<Compiler>com;

	// パイプライン
	std::shared_ptr<Pipe>pointPipe;
	std::shared_ptr<Pipe>boxPipe;
	std::shared_ptr<Pipe>texPipe;
	std::shared_ptr<Pipe>modelPipe;

	// 定数バッファ
	std::shared_ptr<Constant>constant;

	// 点
	std::shared_ptr<Point>point;

	// ボックス
	std::shared_ptr<Box>box;

	// テクスチャ
	std::shared_ptr<Texture>tex;

	// PMD
	std::shared_ptr<PMD>pmd;
};
