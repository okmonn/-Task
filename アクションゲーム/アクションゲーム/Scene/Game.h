#pragma once
#include "Scene.h"
class Game :
	public Scene
{
public:
	// コンストラクタ
	Game();
	// デストラクタ
	~Game();

	// 処理
	void UpData(void);
};

