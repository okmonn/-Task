#pragma once
#include "Typedef.h"
#include <map>
#include <vector>

class Event
{
public:
	// �R���X�g���N�^
	Event();
	// �f�X�g���N�^
	virtual ~Event();

	// ������̌���
	std::string FindString(const std::string path, const char find, int offset, bool start);

	// �ǂݍ���
	void Load(std::string fileName);

	// �����蔻��
	bool CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);
	// �����蔻��
	bool CheackHit2(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);

	// �`��
	virtual void Draw(void) = 0;

	// ����
	virtual void UpData(void) = 0;

protected:
	// ���W
	Positionf pos;

	// �J�������W
	Positionf camPos;

	// �摜
	int image;

	// ��`�T�C�Y
	int attackSize;

	// ���[�h
	std::string mode;

	// �t���[��
	int flam;

	// �z��ԍ�
	UINT index;

	// �摜�f�[�^
	std::map<std::string, ImageData>data;

	// �����f�[�^
	std::map < std::string, std::vector<CutData>>cut;

	// �������`�̃f�[�^
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;
};

