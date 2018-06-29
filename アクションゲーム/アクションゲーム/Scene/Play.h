#pragma once
#include "Scene.h"
#include "../Camera.h"
#include "../Stage.h"
#include "../BackGround.h"
#include "../Player.h"
#include "../Interface.h"
#include "../Event.h"
#include "../Enemy.h"
#include "../Ground.h"
#include <list>

class Play :
	public Scene
{
public:
	// �R���X�g���N�^
	Play(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	~Play();

	// �C���X�^���X��
	void Create(void);

	// ����
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �t�F�[�h�C��
	bool FadeIn(void);
	// �t�F�[�h�A�E�g
	bool FadeOut(void);

	// �X�e�[�W�N���X
	std::shared_ptr<Stage>st;

	// �J�����N���X
	std::shared_ptr<Camera>cam;

	// �w�i�N���X
	std::shared_ptr<BackGround>back;

	// �v���C���[�N���X
	std::shared_ptr<Player>pl;

	// UI�N���X
	std::shared_ptr<Interface>ui;

	// �C�x���g
	std::list<std::shared_ptr<Event>>list;

	// �G�̃��X�g
	std::list<std::shared_ptr<Enemy>>e_list;

	// �n�ʃN���X
	std::shared_ptr<Ground>ground;

	// �֐��|�C���^
	bool (Play::*func)(void);

	int x;
	int ex;

	int image;

	bool mm;

	int flam;
};