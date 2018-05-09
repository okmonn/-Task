#pragma once
#include "Typedef.h"

class BackGround
{
public:
	// コンストラクタ
	BackGround();
	// デストラクタ
	virtual ~BackGround();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 背景画像
	int back;
};

