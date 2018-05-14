#pragma once
#include "Typedef.h"
#include <map>
#include <vector>
#include <string>

class Enemy
{
public:
	// �R���X�g���N�^
	Enemy();
	// �f�X�g���N�^
	virtual ~Enemy();

	// ������̌���
	std::string FindString(const std::string path, const char find, int offset = 0, bool start = true);
	// �ǂݍ���
	void Load(std::string s);

	// ���S�_�̐ݒ�
	void SetSenter(Position & pos, bool r);
	// ���W�̐ݒ�
	void SetPos(Positionf pos);
	void SetPos(float y);

	// ���W�̎擾
	Positionf GetPos(void);
	// ��Ԃ̎擾
	std::string GetMode(void);
	// �������̎擾
	CutData GetCutData(std::string m, UINT index);

	// ��Ԃ̃Z�b�g
	virtual void SetMode(std::string m, bool r) = 0;
	// �`��
	virtual void Draw(void) = 0;
	// ����
	virtual void UpData(void) = 0;

protected:
	// �摜�f�[�^
	int image;

	// �t���[��
	int flam;

	// �z��ԍ�
	UINT index;

	// ���W
	Positionf pos;

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

	// �ҋ@�t���O
	bool wait;

	// �W�����v�t���O
	bool fly;

	// ��`�T�C�Y
	int attackSize;

	// �摜�f�[�^
	std::map<std::string, ImageData>data;

	// �����f�[�^
	std::map < std::string, std::vector<CutData>>cut;

	// �������`�̃f�[�^
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;
};

