#pragma once
#include "Scene.h"
#include "../Camera.h"
#include "../Stage.h"
#include "../BackGround.h"
#include "../Player.h"
#include "../Interface.h"
#include "../Event.h"
#include "../Enemy.h"
#include "../Ground.h"
#include <list>

class Play :
	public Scene
{
public:
	// コンストラクタ
	Play(std::weak_ptr<Input>in);
	// デストラクタ
	~Play();

	// インスタンス化
	void Create(void);

	// 処理
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// フェードイン
	bool FadeIn(void);
	// フェードアウト
	bool FadeOut(void);

	// ステージクラス
	std::shared_ptr<Stage>st;

	// カメラクラス
	std::shared_ptr<Camera>cam;

	// 背景クラス
	std::shared_ptr<BackGround>back;

	// プレイヤークラス
	std::shared_ptr<Player>pl;

	// UIクラス
	std::shared_ptr<Interface>ui;

	// イベント
	std::list<std::shared_ptr<Event>>list;

	// 敵のリスト
	std::list<std::shared_ptr<Enemy>>e_list;

	// 地面クラス
	std::shared_ptr<Ground>ground;

	// 関数ポインタ
	bool (Play::*func)(void);

	int x;
	int ex;

	int image;

	bool mm;

	int flam;
};