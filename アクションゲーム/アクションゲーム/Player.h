#pragma once
#include "Typedef.h"
#include <vector>
#include <map>

class Player
{
public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	virtual ~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// ��Ԃ̃Z�b�g
	void SetMode(std::string m, bool r = false);
	// ��Ԃ̎擾
	std::string GetMode(void);

	// ���S�_�̎擾
	Position Getcenter(void);

	// ��Ԉꗗ�̎擾
	std::vector<std::string> GetAllMode(void);

private:
	// ������̌���
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// �ǂݍ���
	void Load(void);


	// �摜�f�[�^
	int image;
	
	// �t���[����
	int flam;

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
	int index;

	// ���
	std::string mode;

	// ���S
	Position center;

	// ���]�t���O
	bool reverse;
};

