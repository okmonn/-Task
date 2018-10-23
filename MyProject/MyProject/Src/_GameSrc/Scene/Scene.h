#pragma once

class Scene
{
public:
	// コンストラクタ
	Scene() {
	}
	// デストラクタ
	virtual ~Scene() {
	}

	// 描画
	virtual void Draw(void) = 0;

	// 処理
	virtual void UpData(void) = 0;

protected:

};
