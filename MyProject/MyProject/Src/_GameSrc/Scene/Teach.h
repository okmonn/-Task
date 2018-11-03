#pragma once
#include "Scene.h"

class Teach :
	public Scene
{
public:
	// コンストラクタ
	Teach();
	// デストラクタ
	~Teach();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// モデル
	int model;

	// WVP
	float x = 0.f;
	float y = 10.f;
	float z = -15.f;
};
