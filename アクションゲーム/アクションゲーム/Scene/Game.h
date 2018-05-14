#pragma once
#include "Scene.h"
#include "../BackGround.h"
#include "../Interface.h"
#include "../Input.h"
#include "../Player.h"
#include "../Deadman.h"
#include "../Ground.h"
#include <memory>
class Game :
	public Scene
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

	// 背景クラス
	std::shared_ptr<BackGround>back;

	// UIクラス
	std::shared_ptr<Interface>ui;

	// インプット
	std::shared_ptr<Input>in;

	// プレイヤークラス
	std::shared_ptr<Player>pl;

	// 敵クラス
	std::shared_ptr<Deadman>man;

	// 地面クラス
	std::shared_ptr<Ground>ground;
};

