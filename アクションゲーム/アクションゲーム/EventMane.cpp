#include "EventMane.h"
#include "Block.h"
#include "Typedef.h"

EventMane* EventMane::instance = nullptr;

// コンストラクタ
EventMane::EventMane()
{
}

// デストラクタ
EventMane::~EventMane()
{
}

// インスタンス化
void EventMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new EventMane;
	}
}

// 破棄
void EventMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// ブロックの生成
std::shared_ptr<Event> EventMane::CreateBlock(float x, float y, std::shared_ptr<Player> pl, std::shared_ptr<Camera> cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Block>bl;
	bl = std::make_shared<Block>(pos, pl, cam);
	return bl;
}
