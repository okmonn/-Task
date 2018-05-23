#pragma once
#include "Camera.h"

class BackGround
{
public:
	// コンストラクタ
	BackGround(std::weak_ptr<Camera>cam);
	// デストラクタ
	virtual ~BackGround();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// カメラクラス
	std::weak_ptr<Camera>cam;

	// 背景画像
	int back;

	// 座標
	Positionf pos;

	float z;
};

