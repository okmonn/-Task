#pragma once
class Scene
{
public:
	// コンストラクタ
	Scene();
	// デストラクタ
	virtual ~Scene();

	// 処理
	virtual void UpData(void) = 0;

private:
};

