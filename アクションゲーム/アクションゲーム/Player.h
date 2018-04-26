#pragma once
#include "Typedef.h"
#include "Input.h"
#include <vector>
#include <map>
#include <memory>

class Player
{
public:
	// �R���X�g���N�^
	Player(std::weak_ptr<Input>in);
	// �f�X�g���N�^
	virtual ~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData();


	void SetCenter(Position& pos, bool r = false);
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

	// 
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
};

