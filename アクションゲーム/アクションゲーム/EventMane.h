#pragma once
#include "Camera.h"
#include "Event.h"

class EventMane
{
public:
	// デストラクタ
	~EventMane();

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// インスタンス変数の取得
	static EventMane* GetInstance(void)
	{
		return instance;
	}

	// ブロックの生成
	std::shared_ptr<Event> CreateBlock(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// 梯子の生成
	std::shared_ptr<Event> CreateLadder(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);

private:
	// コンストラクタ
	EventMane();

	// インスタンス変数
	static EventMane* instance;
};

