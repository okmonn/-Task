#pragma once
#include "Scene.h"
class Over :
	public Scene
{
public:
	// コンストラクタ
	Over(std::weak_ptr<Input>in);
	// デストラクタ
	~Over();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 画像データ
	int image;
};

