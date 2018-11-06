#pragma once
#include "Character.h"
#include <map>
#include <functional>

class Player :
	public Character
{
public:
	// コンストラクタ
	Player(std::weak_ptr<Camera>cam, const Vec2f& pos, const Vec2f& size = { 64.0f, 64.0f });
	// デストラクタ
	~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 初期化
	void Init(void);

	// アニメーションの終了フラグ
	bool CheckAnimEnd(void);

	// 待機
	void Wait(void);

	// 歩き
	void Walk(void);

	// 攻撃
	void Attack(void);

	// しゃがみ
	void Crouch(void);

	// ダメージ
	void Damage(void);


	// 関数ポインタ
	void (Player::*updata)(void);
};
