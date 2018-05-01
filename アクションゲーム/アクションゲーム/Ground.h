#pragma once
#include "Player.h"
#include <memory>

class Ground
{
public:
	// コンストラクタ
	Ground(std::weak_ptr<Player>pl);
	// デストラクタ
	~Ground();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 地面座標の取得
	int GetHeight(void);

private:
	// プレイヤー
	std::weak_ptr<Player>pl;

	// 地面座標
	int y;
};

