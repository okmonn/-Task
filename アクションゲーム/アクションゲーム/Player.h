#pragma once
#include "Typedef.h"

class Player
{
public:
	// コンストラクタ
	Player();
	// デストラクタ
	virtual ~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 画像データ
	int image;
	
	// フレーム数
	int flam;
};

