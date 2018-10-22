#pragma once
#include "InputData.h"
#include "../Sound/Wave.h"
#include <string>

namespace func {
	// �r�b�g�ϊ�
	int Bit(const int& i);

	// �o�C�g�ϊ�
	int Byte(const int& i);

	// �ΐ�
	int Log(const int& i, const int& n);

	// �ݏ�
	int Pow(const int& i, const int& n);

	// ������̊m�F
	bool CheckChar(const std::string& find, unsigned char * data, const unsigned int& dataNum = 4);

	// ������̑��
	void AddChar(const std::string& add, unsigned char* data, const unsigned int& dataNum = 4);

	// ������̌���
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// �t�H���_���ƃt�@�C�����̘A��
	std::string FileLink(const std::string& path1, const std::string& path2);

	// ���C�h������̕ϊ�
	std::wstring ChangeWString(const std::string& st);

	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWindowSize(const unsigned int& x = 640, const unsigned int& y = 480);

	// �������E�X�^�[�g
	void Start(void);

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �L�[����
	bool CheckKey(const int& i);

	// �g���K�[����
	bool CheckTriger(const int& i);

	// �摜�̓ǂݍ���
	void LoadImg(const std::string& fileName, int& i);

	// �`�揀��
	void Set(void);

	// �|�C���g�`��
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// ���C���`��
	void DrawLine(const float& x1, const float& y1, const float& x2, const float& y2, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �g���C�A���O���`��
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �{�b�N�X�`��
	void DrawBox(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3, const float& x4, const float& y4,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �摜�̕`��
	void DrawImg(int& i, const float& x, const float& y, const float& alpha = 1.0f, const int& turnX = 0, const int& turnY = 0);

	// �摜�̕`��E�T�C�Y�w��
	void DrawSizeImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, 
		const float& alpha = 1.0f, const int& turnX = 0, const int& turnY = 0);

	// �摜�̕`��E�T�C�Y�E�͈͎w��
	void DrawRectImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float& alpha = 1.0f, const int& turnX = 0, const int& turnY = 0);

	// �摜�̕`��E4�_�w��
	void DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
		const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha = 1.0f, const int & turnX = 0, const int & turnY = 0);

	// �摜�̕`��E4�_�E�͈͎w��
	void DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float & alpha = 1.0f, const int & turnX = 0, const int & turnY = 0);

	// �`����s
	void Do(void);

	// �I������
	void End(void);
}
