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

	// �����蔻��
	bool CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);
	// �����蔻��
	bool CheackHit(Positionf& pos1, Positionf& pos2, Positionf& pos3, Positionf& pos4);

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

	bool GetPPP(void)
	{
		return ppp;
	}
	void SetPPP(bool f)
	{
		ppp = f;
	}

	// ���S�t���O�̎擾
	bool GetDie(void);

	// ������t���O�̎擾
	bool GetHit(void);
	// ������t���O�̕ύX
	void SetHit(bool flg);

	Positionf GetCamPos(void);
	CutData GetCut(void);

	bool GetReverse(void);

	void ChangeDir(void);

	bool GetOut(void);

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

	bool ppp;

	// ���x
	Vector2Df vel;

	// �ҋ@�t���O
	bool wait;

	// �W�����v�t���O
	bool fly;

	// ��`�T�C�Y
	int attackSize;

	// ���S�t���O
	bool die;

	// ������t���O
	bool hit;

	Positionf camPos;

	int go;

	bool out;
	bool dir[2];

	// �摜�f�[�^
	std::map<std::string, ImageData>data;

	// �����f�[�^
	std::map < std::string, std::vector<CutData>>cut;

	// �������`�̃f�[�^
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;
};

