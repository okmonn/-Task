#pragma once
#include "Object.h"
#include "../etc/Typedef.h"
#include <memory>

class Camera;


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

	// ステータスの取得
	State GetSt(void) const {
		return st;
	}
	// ステータスのセット
	void SetSt(const State& st) {
		this->st = st;
	}

protected:
	// カメラ
	std::weak_ptr<Camera>cam;

	// アニメーション番号
	int index;

	// フレーム
	int flam;

	// 移動速度
	float speed;

	// ローカル座標
	Vec2f lpos;

	// 切り取り矩形座標
	Vec2f rect;

	// ステータス
	State st;
};
