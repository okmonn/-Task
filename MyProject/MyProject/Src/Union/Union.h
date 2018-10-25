#pragma once
#include <memory>
#include <string>

class Window;
class Debug;
class Input;
class Sound;
class Device;
class Queue;
class List;
class Swap;
class Fence;
class Render;
class Depth;
class Constant;
class Root;
class Pipe;
class Point;
class Line;
class Triangle;
class Texture;
class Model;

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
	void Start(void);

	// メッセージの確認
	bool CheckMsg(void);

	// WVPの更新
	void ChangeWVP(const float& eyeX, const float& eyeY, const float& eyeZ,
		const float& targetX, const float& targetY, const float& targetZ, const float& upX = 0.0f, const float& upY = 1.0f, const float& upZ = 0.0f);

	// キー入力
	bool CheckKey(const int& i);

	// トリガー入力
	bool CheckTriger(const int& i);

	// サウンドの読み込み
	void LoadSnd(const std::string& fileName, int& i);

	// サウンドの再生
	void Play(int& i, const bool& loop);

	// サウンドの停止
	void Stop(int& i);

	// 画像の読み込み
	void LoadImg(const std::string& fileName, int& i);

	// 描画準備
	void Set(void);

	// ポイント描画
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha);

	// ライン描画
	void DrawLine(const float& x1, const float& y1,const float& x2, const float& y2,  const float& r, const float& g, const float& b, const float& alpha);

	// トライアングル描画
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha);

	// 画像の描画
	void DrawImg(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY);

	// 画像の描画・サイズ指定
	void DrawSizeImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, const float& alpha, const int& turnX, const int& turnY);

	// 画像の描画・サイズ・範囲指定
	void DrawRectImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float& alpha, const int& turnX, const int& turnY);

	// 画像の描画・4点指定
	void DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
		const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY);

	// 画像の描画・4点・範囲指定
	void DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float & alpha, const int & turnX, const int & turnY);

	// 描画実行
	void Do(void);

	// 終了
	void End(void);

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

	// ルートシグネチャのインスタンス
	void CreateRoot(void);

	// パイプラインのインスタンス
	void CreatePipe(void);

	// 描画
	void Draw(void);

	// 頂点のリセット
	void Reset(void);


	// ウィンドウサイズX
	unsigned int x;

	// ウィンドウサイズY
	unsigned int y;

	// ウィンドウ
	std::shared_ptr<Window>win;

	// デバッガ
	std::shared_ptr<Debug>deg;

	// インプット
	std::shared_ptr<Input>input;

	// サウンド
	std::shared_ptr<Sound>snd;

	// デバイス
	std::shared_ptr<Device>dev;

	// コマンドキュー
	std::shared_ptr<Queue>que;

	// コマンドリスト
	std::shared_ptr<List>list;

	// スワップチェイン
	std::shared_ptr<Swap>swap;

	// フェンス
	std::shared_ptr<Fence>fen;

	// レンダーターゲット
	std::shared_ptr<Render>ren;

	// 深度ステンシル
	std::shared_ptr<Depth>dep;

	// 定数バッファ
	std::shared_ptr<Constant>con;

	// ルートシグネチャ
	std::shared_ptr<Root>drwRoot;
	std::shared_ptr<Root>texRoot;
	std::shared_ptr<Root>mdlRoot;

	// パイプライン
	std::shared_ptr<Pipe>pntPipe;
	std::shared_ptr<Pipe>linPipe;
	std::shared_ptr<Pipe>triPipe;
	std::shared_ptr<Pipe>texPipe;
	std::shared_ptr<Pipe>mdlPipe;

	// ポイント
	std::shared_ptr<Point>pnt;

	// ライン
	std::shared_ptr<Line>lin;

	// トライアングル
	std::shared_ptr<Triangle>tri;

	// テクスチャ
	std::shared_ptr<Texture>tex;

	// モデル
	std::shared_ptr<Model>model;
};
