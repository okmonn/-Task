#pragma once
#include <memory>

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

	// 描画準備
	void Set(void);

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
	std::shared_ptr<Root>pntRoot;
	std::shared_ptr<Root>texRoot;

	// パイプライン
	std::shared_ptr<Pipe>pntPipe;
	std::shared_ptr<Pipe>texPipe;

	// ポイント
	std::shared_ptr<Point>pnt;
};
