#pragma once
#include "Object.h"
#include "../etc/Typedef.h"

class Character :
	public Object
{
public:
	// コンストラクタ
	Character();
	// デストラクタ
	virtual ~Character();

	// 描画
	virtual void Draw(void) = 0;

	// ステータスのセット
	void SetState(const State& st);

	// ステータスの取得
	State GetSt(void) const {
		return st;
	}
	// ステータスのセット
	void SetSt(const State& st) {
		this->st = st;
	}

protected:
	// アニメーション番号
	int index;

	// フレーム
	int flam;

	// 移動速度
	float speed;

	// 反転フラグ
	bool reverse;

	// ローカル座標
	Vec2f lpos;

	// 前の座標
	Vec2f oldPos;

	// ステータス
	State st;
};
