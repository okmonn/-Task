#pragma once
#include "Scene.h"

class Game
{
public:
	// デストラクタ
	~Game();

	// インスタンスの取得
	static Game& Instance(void)
	{
		static Game instance;
		return instance;
	}

	// 初期化
	void Init(void);

	// クラスのインスタンス化
	void Create(void);

	// シーンの移行
	void ChangeScene(Scene* s);

	// フラグのセット
	void ChangeFlag(bool flag);

	// それぞれのクラスの描画
	void Draw(void);

	// それぞれのクラスの処理
	void UpData(void);

	// メインループ
	void Run(void);

	// 終了処理
	void Destroy(void);

private:
	// コンストラクタ
	Game();
	Game(const Game&);

	// インプット
	std::shared_ptr<Input>in;

	// シーンクラス
	std::shared_ptr<Scene>scene;

	bool flag;

};

