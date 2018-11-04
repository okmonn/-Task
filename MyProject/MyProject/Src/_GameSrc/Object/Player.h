#pragma once
#include "Character.h"

class Player :
	public Character
{
public:
	// コンストラクタ
	Player(std::weak_ptr<Camera>cam);
	// デストラクタ
	~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};
