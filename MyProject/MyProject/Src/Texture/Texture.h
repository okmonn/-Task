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
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<TextureLoader>loader);
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
	std::weak_ptr<TextureLoader>loader;

	// �摜�f�[�^
	std::map<int*, Tex>tex;
};
