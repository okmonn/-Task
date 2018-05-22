#pragma once
#include "../Input.h"
#include <memory>

class Scene
{
public:
	// コンストラクタ
	Scene();
	// デストラクタ
	virtual ~Scene();

	// 描画
	virtual void Draw(void) = 0;
	// 処理
	virtual void UpData(void) = 0;

protected:
	// インプットクラス
	std::weak_ptr<Input>in;

	// 透明度
	int blend;

	// フォントサイズ
	int fSize;

	// 透明開始フラグ
	bool alpha;
};

