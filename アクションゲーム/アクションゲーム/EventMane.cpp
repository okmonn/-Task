#include "EventMane.h"
#include "Block.h"
#include "Ladder.h"
#include "Typedef.h"

EventMane* EventMane::instance = nullptr;

// �R���X�g���N�^
EventMane::EventMane()
{
}

// �f�X�g���N�^
EventMane::~EventMane()
{
}

// �C���X�^���X��
void EventMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new EventMane;
	}
}

// �j��
void EventMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// �u���b�N�̐���
std::shared_ptr<Event> EventMane::CreateBlock(float x, float y, std::shared_ptr<Player> pl, std::shared_ptr<Camera> cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Block>bl;
	bl = std::make_shared<Block>(pos, pl, cam);
	return bl;
}

// ��q�̐���
std::shared_ptr<Event> EventMane::CreateLadder(float x, float y, std::shared_ptr<Player> pl, std::shared_ptr<Camera> cam)
{
	Positionf pos = { x, y };
	std::shared_ptr<Ladder>la;
	la = std::make_shared<Ladder>(pos, pl, cam);
	return la;
}
