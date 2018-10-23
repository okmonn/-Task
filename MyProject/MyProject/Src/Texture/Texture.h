#pragma once
#include "TextureData.h"
#include <string>
#include <map>

class Device;
class List;
class Constant;
class Constant;
class Root;
class Pipe;
class TextureLoader;

class Texture
{
public:
	// �R���X�g���N�^
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, 
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Texture();

	// �ǂݍ���
	long Load(const std::string& fileName, int& i);

	// �`�揀��
	long SetDraw(int& i, const unsigned int& rootNum = 1);

	// �q�[�v�̃Z�b�g
	void SetHeap(int& i, const unsigned int& rootNum = 1);

	// �`��
	void Draw(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY);

	// �`��E�T�C�Y�w��
	void DrawSize(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, const float& alpha, const int& turnX, const int& turnY);

	// �`��E�T�C�Y�E�͈͎w��
	void DrawRectSize(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float& alpha, const int& turnX, const int& turnY);

	// �`��E���W4�_�w��
	void FreelyDraw(int & i, const float & x1, const float & y1, const float & x2, const float & y2, 
		const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY);

	// �`��E���W4�_�E�͈͎w��
	void FreelyDrawRect(int & i, const float & x1, const float & y1, const float & x2, const float & y2,const float & x3, const float & y3, const float & x4, const float & y4, 
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const float & alpha, const int & turnX, const int & turnY);

	// �폜
	void DeleteImg(int& i);

private:
	// ���\�[�X�r���[�̐���
	void CreateView(int* i);

	// ���_���\�[�X�̐���
	long CreateRsc(int* i);

	// �}�b�v
	long Map(int* i);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �萔�o�b�t�@
	std::weak_ptr<Constant>con;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �e�N�X�`�����[�_�[
	std::shared_ptr<TextureLoader>loader;

	// �摜�f�[�^
	std::map<int*, Tex>tex;
};
