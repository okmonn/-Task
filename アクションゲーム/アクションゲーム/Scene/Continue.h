#pragma once
#include "Scene.h"
class Continue :
	public Scene
{
public:
	// コンストラクタ
	Continue(std::weak_ptr<Input>in);
	// デストラクタ
	~Continue();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};
