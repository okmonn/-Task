#pragma once
#include "Camera.h"
#include "Event.h"

class EventMane
{
public:
	// �f�X�g���N�^
	~EventMane();

	// �C���X�^���X��
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �C���X�^���X�ϐ��̎擾
	static EventMane* GetInstance(void)
	{
		return instance;
	}

	// �u���b�N�̐���
	std::shared_ptr<Event> CreateBlock(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// ��q�̐���
	std::shared_ptr<Event> CreateLadder(float x, float y, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);

private:
	// �R���X�g���N�^
	EventMane();

	// �C���X�^���X�ϐ�
	static EventMane* instance;
};

