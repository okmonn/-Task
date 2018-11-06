#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Object;

class Camera
{
public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();

	// ターゲットのセット
	void SetTarget(std::weak_ptr<Object>obj);

	// 処理
	void UpData(const Vec2f& area);

	// ローカル座標に変換
	Vec2f ChangeLocal(const Vec2f& pos);

	// 座標の取得
	Vec2f GetPos(void) const {
		return pos;
	}
	// 拡大率の取得
	float GetLarge(void) const {
		return large;
	}

private:
	// 座標の比較
	void ComparisonPos(float & pos, const float & areaSize, const int & winSize);


	// ターゲットオブジェクト
	std::weak_ptr<Object>obj;

	// 座標
	Vec2f pos;

	// 拡大率
	float large;
};
