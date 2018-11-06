#pragma once
#include "InputData.h"
#include <string>
#include <vector>

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
	bool CheckChar(const std::string& find, char * data, const unsigned int& dataNum = 4);

	// ������̑��
	void AddChar(const std::string& add, unsigned char* data, const unsigned int& dataNum = 4);

	// ������̌���
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// �t�H���_���ƃt�@�C�����̘A��
	std::string FileLink(const std::string& path1, const std::string& path2);

	// ���C�h������̕ϊ�
	std::wstring ChangeWString(const std::string& st);

	// �f�B���N�g���̃t�@�C�����擾
	std::vector<std::string> GetDirFile(const std::string& dir);

	// �j���[�g���@
	float Newton(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop = 16);

	// 2���@
	float Bisection(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop = 32);

	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWindowSize(const unsigned int& x = 640, const unsigned int& y = 480);

	// �E�B���h�E�T�C�YX�̎擾
	float GetWinSizeX(void);

	// �E�B���h�E�T�C�YY�̎擾
	float GetWinSizeY(void);

	// �������E�X�^�[�g
	void Start(void);

	// WVP�̍X�V
	void ChangeWVP(const float& eyeX, const float& eyeY, const float& eyeZ,
		const float& targetX, const float& targetY, const float& targetZ, const float& upX = 0.0f, const float& upY = 1.0f, const float& upZ = 0.0f);

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �L�[����
	bool CheckKey(const int& i);

	// �g���K�[����
	bool CheckTriger(const int& i);

	// �T�E���h�̓ǂݍ���
	void LoadSnd(const std::string& fileName, int& i);

	// �T�E���h�̍Đ�
	void Play(int& i, const bool& loop = false);

	// �T�E���h�̒�~
	void Stop(int& i);

	// �摜�̓ǂݍ���
	void LoadImg(const std::string& fileName, int& i);

	// PMD�̓ǂݍ���
	void LoadPmd(const std::string& fileName, int& i);

	// �A�j���[�V�����̃A�^�b�`
	void Attach(const std::string& fileName, int& i);

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
	void DrawBox(const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// �摜�̕`��
	void DrawImg(int& i, const float& x, const float& y, const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// �摜�̕`��E�T�C�Y�w��
	void DrawSizeImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, 
		const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// �摜�̕`��E�T�C�Y�E�͈͎w��
	void DrawRectImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// �摜�̕`��E4�_�w��
	void DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
		const float & x3, const float & y3, const float & x4, const float & y4, const int & turnX = 0, const int & turnY = 0, const float & alpha = 1.0f);

	// �摜�̕`��E4�_�E�͈͎w��
	void DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int & turnX = 0, const int & turnY = 0, const float & alpha = 1.0f);

	// PMD�̃A�j���[�V�������Ԃ̃��Z�b�g
	void ResetAnim(int& i);

	// �A�j���[�V����
	void Animation(int& i, const bool& loop = false, const float& animSpeed = 0.5f);

	// �A�j���[�V�����̏I���m�F
	bool CheckEndAnim(int& i);

	// PMD�̕`��
	void DrawPmd(int& i);
	
	// �`����s
	void Do(void);

	// �T�E���h�̍폜
	void DeleteSnd(int& i);

	// �摜�̍폜
	void DeleteImg(int& i);

	// ���f���̍폜
	void DeleteMdl(int& i);

	// �I������
	void End(void);
}
