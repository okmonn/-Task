#pragma once
#include <memory>
#include <string>

class Window;
class Debug;
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
class TextureLoader;
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

	// ウィンドウサイズのセット
	void SetWinSize(const unsigned int& x, const unsigned int& y);

	// クラスのインスタンス
	void Start(void);

	// メッセージの確認
	bool CheckMsg(void);

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

	// パイプライン
	std::shared_ptr<Pipe>pntPipe;
	std::shared_ptr<Pipe>linPipe;
	std::shared_ptr<Pipe>triPipe;
	std::shared_ptr<Pipe>texPipe;

	// ポイント
	std::shared_ptr<Point>pnt;

	// ライン
	std::shared_ptr<Line>lin;

	// トライアングル
	std::shared_ptr<Triangle>tri;

	// テクスチャローダー
	std::shared_ptr<TextureLoader>texLoad;

	// テクスチャ
	std::shared_ptr<Texture>tex;
};
