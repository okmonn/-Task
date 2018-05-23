#pragma once
#include "Stage.h"
#include "Typedef.h"
#include <memory>

class Player;

class Camera
{
public:
	// �R���X�g���N�^
	Camera(std::weak_ptr<Stage>st);
	// �f�X�g���N�^
	virtual ~Camera();

	// ���S�I�u�W�F�N�g�̐ݒ�
	void SetFocus(std::shared_ptr<Player>pl);

	// ���W�̎擾
	Positionf& GetPos(void);

	// �ڎ��T�C�Y�̎擾
	Rect& GetViewSize(void);

	// �`��
	void Draw(void);
	// ����
	void UpData(void);

	// ���W�C��
	Positionf CorrectionPos(Positionf pos);

private:
	// �v���C���[�N���X
	std::weak_ptr<Player>pl;
	
	// �X�e�[�W�N���X
	std::weak_ptr<Stage>st;

	// ���W
	Positionf pos;
};

