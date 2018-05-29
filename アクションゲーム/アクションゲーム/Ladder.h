#pragma once
#include "Event.h"
#include "Camera.h"
#include "Player.h"
class Ladder :
	public Event
{
public:
	// �R���X�g���N�^
	Ladder(Positionf pos, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam);
	// �f�X�g���N�^
	~Ladder();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �J�����@
	std::weak_ptr<Camera>cam;

	// �v���C���[
	std::weak_ptr<Player>pl;

	// ���W
	Positionf pos;
};

