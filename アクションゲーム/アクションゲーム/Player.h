#pragma once
#include "Typedef.h"
#include "Input.h"
#include <vector>
#include <map>
#include <memory>

class Game;

class Player
{
	friend Game;
public:
	// �R���X�g���N�^
	Player(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	virtual ~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData();

	// ���S���W�̃Z�b�g
	void SetCenter(Position& pos, bool r = false);
	// ��Ԃ̃Z�b�g
	void SetMode(std::string m, bool r = false);
	// ��Ԃ̎擾
	std::string GetMode(void);

	// ���W�̎擾
	Positionf GetPos(void);
	// ���W�̃Z�b�g
	void SetPos(Positionf pos);
	void SetPos(float y);

	// ��Ԉꗗ�̎擾
	std::vector<std::string> GetAllMode(void);

	// ���]�t���O�̎擾
	bool GetReverse(void);

private:
	// ������̌���
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// �ǂݍ���
	void Load(void);

	// �ҋ@�̏���
	void Wait(void);
	// �����̏���
	void Walk();
	// �W�����v�̏���
	void Jump(void);
	// �p���`�̏���
	void Panch(void);
	// �L�b�N�̏���
	void Kick(void);
	// �X���C�f�B���O�̏���
	void Sliding(void);
	// ���Ⴊ�݂̏���
	void Down(void);
	// �_���[�W�̏���
	void Damage(void);

	// ��Ԃ̕ύX
	void ChangeMode(void);



	// �C���v�b�g�N���X
	std::weak_ptr<Input>in;

	// �摜�f�[�^
	int image;
	
	// �t���[����
	UINT flam;

	// ���W
	Positionf pos;

	// ����
	Rect rect;

	// ���[�h�z��
	std::map<std::string, ImageData>data;

	// ���
	std::vector<std::string>fmode;

	// �����f�[�^
	std::map<std::string, std::vector<CutData>>cut;

	// �z��ԍ�
	UINT index;

	// ���
	std::string mode;

	// ���S
	Position center;

	// ���]�t���O
	bool reverse;

	// ���[�v�t���O
	bool loop;

	// ���x
	Vector2Df vel;

	// 
	void (Player::*func)(void);
};

